```
docker build . -t hbase-native
docker run --network=host --rm -it hbase-native
```

Add the hostname of your machine to /etc/hosts with the IP shown as the default route in the output of `route` (e.g. 192.168.35.1 for me)

The following should work to build the code:

```
$ git clone https://github.com/joshelser/hbase-native-testing && cd hbase-native-testing
$ g++ hbase-sample.cc -o hbase-sample -rdynamic -Wl,-Bstatic -lprotobuf -Wl,-Bdynamic -lboost_thread -lboost_system -lboost_filesystem -lboost_chrono -lboost_date_time -lboost_atomic -lpthread -lsasl2 -lgflags -lkrb5 -lHBaseClient -Wl,-Bstatic -lzookeeper_mt  -Wl,-Bdynamic -lpthread /usr/local/lib/libfolly.so -lglog /usr/local/lib/libwangle.a -lssl -lcrypto /usr/local/lib/libfolly.so -lglog /usr/local/lib/libwangle.a -lssl -lcrypto -Wl,-Bstatic -lprotobuf -Wl,-Bdynamic -lboost_thread -lboost_system -lboost_filesystem -lboost_chrono -lboost_date_time -lboost_atomic -lpthread -lsasl2 -lgflags -lkrb5 -Wl,-Bstatic -lzookeeper_mt -Wl,-Bdynamic -Wl,-rpath,/usr/local/lib -std=c++14  -I/usr/local/include -L/usr/local/lib
$ LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib" ./hbase-sample --zookeeper hw10447.local:2181 --znode /hbase-112263
```
