#!/usr/bin/env bash

make cache
(( stride = ${stride:-64} ))

for (( i = $stride ; i<=0x10000000; i = i*2 )); do 
	echo -ne "$i\t"
 	./cache $stride $(( $i/$stride )) $(( 0x10000000 / i )) | awk '{print $NF}'
done
