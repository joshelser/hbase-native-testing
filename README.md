```
docker build . -t hbase-native
docker run --network=host --rm -it hbase-native
```

Add the hostname of your machine to /etc/hosts with the IP shown as the default gateway in the output of `route` (e.g. 192.168.35.1 for me)

This expects a version of HBase running on the host machine (or accesible to the internet). Be sure that the HBase
servers have bound all interfaces (e.g. `hbase.master.ipc.address=0.0.0.0` and `hbase.regionserver.ipc.address=0.0.0.0`
in hbase-site.xml). You will also need to create a table in this instance, e.g. `echo "create 'load_test_table', 'f'" | hbase shell`.

The following should work to build the code:

```
$ git clone https://github.com/joshelser/hbase-native-testing && cd hbase-native-testing
$ g++ hbase-sample.cc -o hbase-sample -rdynamic -Wl,-Bstatic -lprotobuf -Wl,-Bdynamic -lboost_thread -lboost_system -lboost_filesystem -lboost_chrono -lboost_date_time -lboost_atomic -lpthread -lsasl2 -lgflags -lkrb5 -lHBaseClient -Wl,-Bstatic -lzookeeper_mt  -Wl,-Bdynamic -lpthread /usr/local/lib/libfolly.so -lglog /usr/local/lib/libwangle.a -lssl -lcrypto /usr/local/lib/libfolly.so -lglog /usr/local/lib/libwangle.a -lssl -lcrypto -Wl,-Bstatic -lprotobuf -Wl,-Bdynamic -lboost_thread -lboost_system -lboost_filesystem -lboost_chrono -lboost_date_time -lboost_atomic -lpthread -lsasl2 -lgflags -lkrb5 -Wl,-Bstatic -lzookeeper_mt -Wl,-Bdynamic -Wl,-rpath,/usr/local/lib -std=c++14  -I/usr/local/include -L/usr/local/lib
$ LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib" ./hbase-sample --zookeeper hw10447.local:2181 --znode /hbase-112263
```
