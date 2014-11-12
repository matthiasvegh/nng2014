#!/bin/bash

basename=$1

if [ -n "$2" ]; then
	seed=$2
else
	seed=0
fi

for ((i=0; i<1000; ++i)); do
	name=${basename}_gen_$i
	echo $name >&2
	targetName=${name}.target
	outputName=${name}.out
	if ./build-clang_release/bin/findTarget ${basename}.txt $((seed + i)) >$targetName; then
		./build-clang_release/bin/padlolap ${basename}.txt $targetName >$outputName
	fi
done

echo
echo
for f in *.out; do
	echo $(head -n1 $f) $f
done | sort -n

