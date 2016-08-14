/*
   Copyright (C) 2014  nieyong

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
*/

/*
   LD_PRELOAD library to make bind and connect to use a virtual
   IP address as localaddress. Specified via the enviroment
   variable BIND_ADDR.

   Compile on Linux with:
   gcc -nostartfiles -fpic -shared bindp.c -o libindp.so -ldl -D_GNU_SOURCE
   or just use make be easy:
   make

   Example in bash to make inetd only listen to the localhost
   lo interface, thus disabling remote connections and only
   enable to/from localhost:

   BIND_ADDR="127.0.0.1" BIND_PORT="49888" LD_PRELOAD=./libindp.so curl http://192.168.190.128

   OR:
   BIND_ADDR="127.0.0.1" LD_PRELOAD=./libindp.so curl http://192.168.190.128

   Example in bash to use your virtual IP as your outgoing
   sourceaddress for ircII:

   BIND_ADDR="your-virt-ip" LD_PRELOAD=./bind.so ircII

   Note that you have to set up your servers virtual IP first.

   Add SO_REUSEPORT support within Centos7 or Linux OS with kernel >= 3.9, for the applictions with multi-process support just listen one port now
   REUSE_ADDR=1 REUSE_PORT=1 LD_PRELOAD=./libindp.so python server.py &
   RREUSE_ADDR=1 EUSE_PORT=1 LD_PRELOAD=./libindp.so java -server -jar your.jar &

   email: nieyong@staff.weibo.com
   web:   http://www.blogjava.net/yongboy
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dlfcn.h>
#include <errno.h>

int (*real_bind)(int, const struct sockaddr *, socklen_t);
int (*real_connect)(int, const struct sockaddr *, socklen_t);

unsigned long int bind_addr_saddr = 0;
struct sockaddr_in local_sockaddr_in[] = { 0 };

unsigned int bind_port_saddr = 0;
unsigned int reuse_port = 0;
unsigned int reuse_addr = 0;
unsigned int ip_transparent = 0;

void _init (void){
    const char *err;

    real_bind = dlsym (RTLD_NEXT, "bind");
    if ((err = dlerror ()) != NULL) {
        fprintf (stderr, "dlsym (bind): %s\n", err);
    }

    real_connect = dlsym (RTLD_NEXT, "connect");
    if ((err = dlerror ()) != NULL) {
        fprintf (stderr, "dlsym (connect): %s\n", err);
    }

    char *bind_addr_env;
    if ((bind_addr_env = getenv ("BIND_ADDR"))) {
        bind_addr_saddr = inet_addr (bind_addr_env);
        local_sockaddr_in->sin_family = AF_INET;
        local_sockaddr_in->sin_addr.s_addr = bind_addr_saddr;
        local_sockaddr_in->sin_port = htons (0);
    }

    char *bind_port_env;
    if ((bind_port_env = getenv ("BIND_PORT"))) {
        bind_port_saddr = atoi(bind_port_env);
        local_sockaddr_in->sin_port = htons (bind_port_saddr);
    }

    char *reuse_addr_env;
    if ((reuse_addr_env = getenv ("REUSE_ADDR"))) {
        reuse_addr = atoi(reuse_addr_env);
    }

    char *reuse_port_env;
    if ((reuse_port_env = getenv ("REUSE_PORT"))) {
        reuse_port = atoi(reuse_port_env);
    }

    char *ip_transparent_env;
    if ((ip_transparent_env = getenv ("IP_TRANSPARENT"))) {
        ip_transparent = atoi(ip_transparent_env);
    }
}

int bind (int fd, const struct sockaddr *sk, socklen_t sl){
    static struct sockaddr_in *lsk_in;

    lsk_in = (struct sockaddr_in *)sk;

    if(bind_addr_saddr)
        lsk_in->sin_addr.s_addr = bind_addr_saddr;

    if (bind_port_saddr)
        lsk_in->sin_port = htons (bind_port_saddr);

    if (reuse_addr){
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
    }

#ifdef SO_REUSEPORT
    if (reuse_port){
        setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &reuse_port, sizeof(reuse_port));
    }
#endif

    if (ip_transparent){
        int opt =1;
        setsockopt(fd, SOL_IP, IP_TRANSPARENT, &ip_transparent, sizeof(ip_transparent));
    }

    return real_bind (fd, sk, sl);
}

int connect (int fd, const struct sockaddr *sk, socklen_t sl){
    static struct sockaddr_in *rsk_in;

    rsk_in = (struct sockaddr_in *)sk;

    if ((rsk_in->sin_family == AF_INET) && (bind_addr_saddr || bind_port_saddr)) {
        bind (fd, (struct sockaddr *)local_sockaddr_in, sizeof (struct sockaddr));
    }

    return real_connect (fd, sk, sl);
}
