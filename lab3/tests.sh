for i in {2,4,6,8,10,12,14,16}; do ./run.sh $i 10000 > res-$i-10000.txt; done
for i in {2,4,6,8,10,12,14,16}; do ./run.sh $i 1000000 > res-$i-1000000.txt; done