#!/bin/bash

prj_home=$( cd "$( dirname $0 )" && pwd )
pid_file=${prj_home}/pid
cd ${prj_home}

data_dir=${prj_home}/data/


if [ -e ${pid_file} ]; then
    echo "kill `cat ${pid_file}`"
    kill -9 $(cat ${pid_file})
    rm -f ${pid_file}
    sleep 2
fi

