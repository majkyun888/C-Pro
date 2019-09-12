#!/bin/sh

cd /build_root
./build.sh $2
chown -R $1:$1 bin bin-centos

