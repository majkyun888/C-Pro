#!/bin/sh
source ~/.bash_profile
prj_home=$( cd "$( dirname $0 )" && pwd )


cd ${prj_home}
${prj_home}/stop.sh 
pid_file=${prj_home}/pid
data_dir=${prj_home}/data/


sleep 2


if [ ! -d "$data_dir" ]; then
	mkdir ${data_dir}
fi

export PATH=.:/home/wyc/work/ystech_hfpmd/linux/bin/:$PATH
export LD_LIBRARY_PATH=.:/opt/gcc-5.4.0/lib64:$LD_LIBRARY_PATH


./HfpQuoteAPI_Demo "HFP_ID=A12BDE805AB8C68C10F99DCF48F5B1BD" \
                   "HFP_LICENSE=EJOrD6LZPZCjCMHcOIqqCJKoBM9bE68jCJ4rD31cOJCsOZXbwB2YvRI7vfsmHqT6Kp8mQJLjRMbbQaLFE7TEPt5XKJPlHb0uP6mmUNTlKZStJJOsE3PgJNH9TdKmDs9KCMjqCL1nIaHnRJSnTainDMLkDdTrDsCsUMLOJKTKDMetPt1NC4WmR6LOQ7CuIN0tH4bPRKnrC6z9TJT2L35GOsSpQ4CpGqbhKNGoD31cJsHDDaaoKd1rCK99CdWtLJCmCpSmGMTLD55vCJ8nOZT1KZLBI5X8IJ5ODc4mOMrEMJ5gD6TPDpSn" \
                   "HFP_FRONT_IP=58.215.39.211" \
                   "HFP_FRONT_PORT=59000" \
                  #  <&- &
                                   
echo $! > ${pid_file}
