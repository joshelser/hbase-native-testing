from ubuntu:16.04

# libprotobuf-dev is pulling Protobuf2.6.1 -- need consistency between this and what the client library was built with
RUN apt-get update && apt-get install -y curl git wget binutils less libterm-readline-gnu-perl g++ libgflags-dev libgoogle-glog-dev vim build-essential dh-autoreconf pkg-config libssl-dev libboost-all-dev libevent-dev libdouble-conversion-dev cmake libprotobuf-dev protobuf-compiler libsasl2-dev libkrb5-dev net-tools telnet

# Download the HWX-built HBase client library, install it, and set up some directories
RUN curl -SL http://s3.amazonaws.com/dev.hortonworks.com/HDP/ubuntu16/2.x/BUILDS/2.6.3.0-179/tars/hbase/hbase_native_client.tar.gz | tar xzC /tmp \
      && mv /tmp/release/lib/* /usr/local/lib/ \
      && mv /tmp/release/include/* /usr/local/include/ \
      && rm -r /tmp/release \
      && mkdir /root/build

# ZooKeeper
RUN curl -SL https://dist.apache.org/repos/dist/release/zookeeper/zookeeper-3.4.6/zookeeper-3.4.6.tar.gz | tar xzC /root/build/ \
      && cd /root/build/zookeeper-3.4.6/src/c/ \
      && ./configure \
      && make -j8 \
      && make install \
      && rm -rf /root/build/zookeeper-3.4.6

# Folly
RUN curl -SL https://github.com/facebook/folly/archive/v2017.09.04.00.tar.gz | tar xzC /root/build/ \
      && cd /root/build/folly-2017.09.04.00/folly \
      && autoreconf -ivf \
      && ./configure \
      && make -j8 \
      && make install \
      && rm -fr /root/build/folly-2017.09.04.00

# Wangle
RUN curl -SL https://github.com/facebook/wangle/archive/v2017.09.04.00.tar.gz | tar xzC /root/build \
      && cd /root/build/wangle-2017.09.04.00/wangle \
      && cmake . \
      && make -j8 \
      && make install \
      && rm -fr /root/build/wangle-2017.09.04.00

# GTest/GMock
RUN curl -SL https://github.com/google/googletest/archive/release-1.8.0.tar.gz | tar xzC /root/build \
      && cd /root/build/googletest-release-1.8.0 \
      && cmake . \
      && make \
      && make install \
      && rm -r /root/build/googletest-release-1.8.0

ENTRYPOINT ["/bin/bash"]
