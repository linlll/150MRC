#!/bin/bash

# usage: bash run.sh 20

if [ ! -d "build" ]; then
  mkdir build
fi
cd build
cmake ..
make -j8
if [ ! -f "data.txt" ]; then
  ./data_generator
fi
rm -rf run.log
touch run.log
for ((i=1;i<=$1;i++))
do
  /usr/bin/time -a -o run.log ./read_and_calculate >> run.log
done
./analysis_log > average_time.txt
mv run.log ..
mv average_time.txt ..