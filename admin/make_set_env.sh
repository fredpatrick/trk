#!/bin/sh
echo '#! /bin/csh -f'     
echo export LD_LIBRARY_PATH=${1}
shift
while [ $# -gt 0 ]
do
    echo export  LD_LIBRARY_PATH='${LD_LIBRARY_PATH}:'${1}
    shift
done
