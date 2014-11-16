#!/bin/bash

for ((i = 1 ; i<=14 ; i++)); do
    for ((l = 2 ; l<=14 ; l++)); do
        STRIDE1=$i STRIDE2=$l make
    done
done
