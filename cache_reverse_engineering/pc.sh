#!/bin/bash

counter=6 # 2^6 = 64B
now=$(date +"%T")

while [ $counter -le  18 ] # 256k = 256*1024 = 2^18
do
    stride=$((2**$counter)) time=$now ./pc.py
    ((counter++))
done

