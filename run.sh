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
  /usr/bin/time -a -o run.log ./read_and_calculate $N data.txt >> run.log
done
./analysis_log run.log > average_time.txt
time=$(cat average_time.txt)
echo "Agerage Time: $time"
mv run.log ..
mv average_time.txt ..
rm -rf run1.log
touch run1.log
for ((i=1;i<=$1;i++))
do
  /usr/bin/time -a -o run1.log ./read_and_calculate1 $N data.txt >> run1.log
done
./analysis_log run1.log > average_time1.txt
time1=$(cat average_time1.txt)
echo "Agerage Time: $time1"
mv run1.log ..
mv average_time1.txt ..