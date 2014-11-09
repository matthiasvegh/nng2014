#!/bin/bash

if [ -n "$1" ]; then
	seed=$1
else
	seed=0
fi

for ((i=0; i<1000; ++i)); do
	name=tiles3_gen_$i
	echo $name >&2
	targetName=${name}.target
	outputName=${name}.out
	if ./build-clang_release/bin/findTarget tiles3.txt $((seed + i)) >$targetName; then
		./build-clang_release/bin/padlolap tiles3.txt $targetName >$outputName
	fi
done

echo
echo
for f in *.out; do
	echo $(head -n1 $f) $f
done | sort -n

