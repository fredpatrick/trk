#!/bin/bash
cd /home/fredpatrick/wrk/trkbbb/bin
echo $LD_LIBRARY_PATH
source ./setup_trk.sh
ps
./trkdriver
