bindp
=====

## Intro

With LD\_PRELOAD and `bindp`, you can do:

- For server application
    - Assign ip and port for listening
    - Add SO_REUSEADDR/SO_REUSEPORT for existing application
- For socket client
    - Assign special ip and port for connection
    - Add SO_REUSEPORT for reuse the ip add port

## Compile

Compile on Linux with:

    make

## Usage

### IP & Port

How to use it:

    REUSE_ADDR=1 REUSE_PORT=1 BIND_ADDR="your ip" BIND_PORT="your port" LD_PRELOAD=/your_path/libindp.so The_Command_Here ...


Example in bash to make inetd only listen to the localhost
lo interface, thus disabling remote connections and only
enable to/from localhost:

    BIND_ADDR="127.0.0.1" BIND_PORT="49888" LD_PRELOAD=/your_path/libindp.so curl http://192.168.190.128

OR:

    BIND_ADDR="127.0.0.1" LD_PRELOAD=/your_path/libindp.so curl http://192.168.190.128

Just want to change the nginx's listen port:

    BIND_PORT=8888 LD_PRELOAD=/your_path/libindp.so /usr/sbin/nginx -c /etc/nginx/nginx.conf

Example in bash to use your virtual IP as your outgoing
sourceaddress for ircII:

    BIND_ADDR="your-virt-ip" LD_PRELOAD=/your_path/bind.so ircII

Note that you have to set up your server's virtual IP first.

### SO_REUSEADDR/SO_REUSEPORT

Now, I add the `SO_REUSEADDR`/`SO_REUSEPORT` support within Centos7 or Linux OS with kernel >= 3.9, for the old applictions with multi-process just listen the same port now:

    REUSE_ADDR=1 REUSE_PORT=1 LD_PRELOAD=./libindp.so python server.py &

OR

    REUSE_ADDR=1 REUSE_PORT=1 BIND_PORT=9999 LD_PRELOAD=./libindp.so java -server -jar your.jar &

With libindp.so's support, you can run your app multi-instance just for you need.

And, for socket client's connect you can also reuse the same client's ip and port:

    REUSE_PORT=1 BIND_ADDR="10.10.10.10" BIND_PORT=49999 LD_PRELOAD=/the_path/libindp.so nc 10.10.10.11 10001

    REUSE_PORT=1 BIND_ADDR="10.10.10.10" BIND_PORT=49999 LD_PRELOAD=/the_path/libindp.so nc 10.10.10.11 10011

Enjoy it :))
