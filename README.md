bindp
=====

Binding applications to a specific ip and port within Centos



LD\_PRELOAD library to make bind and connect to use a virtual IP address as localaddress and a usable port as localport. Specified via the enviroment variable BIND\_ADDR.

Compile on Linux with:

    gcc -nostartfiles -fpic -shared bindp.c -o bindp.so -ldl -D_GNU_SOURCE


Example in bash to make inetd only listen to the localhost
lo interface, thus disabling remote connections and only
enable to/from localhost:

    BIND_ADDR="127.0.0.1" BIND_PORT="49888" LD_PRELOAD=./bindp.so curl http://192.168.190.128

OR:

    BIND_ADDR="127.0.0.1" LD_PRELOAD=./bindp.so curl http://192.168.190.128

Example in bash to use your virtual IP as your outgoing
sourceaddress for ircII:

    BIND_ADDR="your-virt-ip" LD_PRELOAD=./bind.so ircII

Note that you have to set up your servers virtual IP first.

Enjoy it.
