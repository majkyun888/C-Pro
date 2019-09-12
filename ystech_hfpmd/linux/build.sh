#!/bin/bash
set -e

export PATH=/opt/gcc-5.4.0/bin:$PATH
export LD_LIBRARY_PATH=/opt/gcc-5.4.0/lib64:$LD_LIBRARY_PATH
make cleanall
make depends
make
#make verify_dependencies

