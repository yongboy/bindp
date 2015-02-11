bindp
=====

Binding applications to a specific ip and port within Centos

LD\_PRELOAD library to make bind and connect to use a virtual IP address as localaddress and a usable port as localport. Specified via the enviroment variable `BIND\_ADDR`/`BIND_PORT`.

Compile on Linux with:

    make    

How to use it:

    BIND_ADDR="your ip" BIND_PORT="your port" LD_PRELOAD=/your_path/libindp.so the real command ...


Example in bash to make inetd only listen to the localhost
lo interface, thus disabling remote connections and only
enable to/from localhost:

    BIND_ADDR="127.0.0.1" BIND_PORT="49888" LD_PRELOAD=/your_path/libindp.so curl http://192.168.190.128

OR:

    BIND_ADDR="127.0.0.1" LD_PRELOAD=/your_path/libindp.so curl http://192.168.190.128

Just want to change the nginx's port:
        
    BIND_PORT=8888 LD_PRELOAD=/your_path/libindp.so /usr/sbin/nginx -c /etc/nginx/nginx.conf

Example in bash to use your virtual IP as your outgoing
sourceaddress for ircII:

    BIND_ADDR="your-virt-ip" LD_PRELOAD=/your_path/bind.so ircII

Note that you have to set up your server's virtual IP first.

Now add the `SO_REUSEPORT` support within Centos7 or Linux OS with kernel >= 3.9, for the old applictions with multi-process just listen the same port now:

    REUSE_PORT=1 LD_PRELOAD=./libbindp.so python server.py &

OR

    REUSE_PORT=1 BIND_PORT=9999 LD_PRELOAD=./libbindp.so java -server -jar your.jar &

With libindp.so's support, you can run your app multi-instance just for you need.

The bindp just support single port applictions, if you want the professional version, please contact me(yongboy(AT)gmail.com).

Enjoy it :))