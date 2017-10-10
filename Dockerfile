from ubuntu:16.04

RUN apt-get update && apt-get install -y curl git wget binutils less libterm-readline-gnu-perl g++ libgflags-dev libgoogle-glog-dev vim build-essential dh-autoreconf pkg-config libssl-dev libboost-all-dev libevent-dev libdouble-conversion-dev cmake libprotobuf-dev libsasl2-dev libkrb5-dev net-tools telnet

RUN curl -SL http://s3.amazonaws.com/dev.hortonworks.com/HDP/ubuntu16/2.x/BUILDS/2.6.3.0-179/tars/hbase/hbase_native_client.tar.gz | tar xzC /tmp \
      && mv /tmp/release/lib/* /usr/local/lib/ \
      && mv /tmp/release/include/* /usr/local/include/ \
      && rm -r /tmp/release

RUN mkdir /root/build

# ZooKeeper
RUN curl -SL https://dist.apache.org/repos/dist/release/zookeeper/zookeeper-3.4.6/zookeeper-3.4.6.tar.gz | tar xzC /root/build/
RUN cd /root/build/zookeeper-3.4.6/src/c/ && ./configure && make -j8 && make install

# Folly
RUN curl -SL https://github.com/facebook/folly/archive/v2017.09.25.00.tar.gz | tar xzC /root/build/
RUN cd /root/build/folly-2017.09.25.00/folly \
      && autoreconf -ivf \
      && ./configure \
      && make -j8 \
      && make install \
      && make clean

# Wangle
RUN curl -SL https://github.com/facebook/wangle/archive/v2017.09.04.00.tar.gz | tar xzC /root/build
RUN cd /root/build/wangle-2017.09.04.00/wangle \
      && cmake . \
      && make -j8 \
      && make install \
      && make clean


ENTRYPOINT ["/bin/bash"]
