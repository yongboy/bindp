bindp
=====

Binding applications to a specific ip and port within Centos



LD\_PRELOAD library to make bind and connect to use a virtual IP address as localaddress and a usable port as localport. Specified via the enviroment variable BIND\_ADDR.

Compile on Linux with:

    gcc -nostartfiles -fpic -shared bindp.c -o bindp.so -ldl -D_GNU_SOURCE
    
How to use it:

    BIND_ADDR="your ip" BIND_PORT="your port" LD_PRELOAD=/your_path/bindp.so the real command ...


Example in bash to make inetd only listen to the localhost
lo interface, thus disabling remote connections and only
enable to/from localhost:

    BIND_ADDR="127.0.0.1" BIND_PORT="49888" LD_PRELOAD=/your_path/bindp.so curl http://192.168.190.128

OR:

    BIND_ADDR="127.0.0.1" LD_PRELOAD=/your_path/bindp.so curl http://192.168.190.128

Just want to change the nginx's port:
        
    BIND_PORT="8888" LD_PRELOAD=/your_path/bindp.so /usr/sbin/nginx -c /etc/nginx/nginx.conf

Example in bash to use your virtual IP as your outgoing
sourceaddress for ircII:

    BIND_ADDR="your-virt-ip" LD_PRELOAD=/your_path/bind.so ircII

Note that you have to set up your servers virtual IP first.

Enjoy it.
