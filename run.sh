#!/bin/bash
# usage: bash run.sh 10
N=150000000
if [ ! -d "build" ]; then
  mkdir build
fi
cd build
cmake ..
make -j8
if [ ! -f "data.txt" ]; then
  ./data_generator $N data.txt
fi
rm -rf run.log
touch run.log
for ((i=1;i<=$1;i++))
do
  /usr/bin/time -a -o run.log ./read_and_calculate data.txt >> run.log
done
./analysis_log run.log > average_time.txt
time=$(cat average_time.txt)
echo "Agerage Time: $time"
mv run.log ..
mv average_time.txt ..