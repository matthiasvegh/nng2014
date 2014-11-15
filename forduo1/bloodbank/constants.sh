#!/bin/bash

for ((i = 1 ; i<=14 ; i++)); do
    for ((l = 1 ; l<=14 ; l++)); do
		echo "outer: $i inner: $l"
        STRIDE1=$i STRIDE2=$l make
    done
done
