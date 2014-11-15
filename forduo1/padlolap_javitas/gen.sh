#!/bin/bash

basename=$1

if [ -n "$2" ]; then
	seed=$2
else
	seed=0
fi

best=
bestFiles=

for ((i=0; i<1000; ++i)); do
	currentSeed=$((seed + i))
	name=${basename}_gen_$currentSeed
	echo $name >&2
	targetName=${name}.target
	outputName=${name}.out
	if ./build-clang_release/bin/findTarget ${basename}.txt $currentSeed >$targetName; then
		./build-clang_release/bin/padlolap ${basename}.txt $targetName >$outputName

		value=$(head -n1 $outputName)
		if [ -z "$best" ] || [ "$best" -gt "$value" ]; then
			best=$value
			rm -f $bestFiles
			bestFiles="$outputName $targetName"
		else
			rm $outputName $targetName
		fi
	fi
done

echo
echo
for f in *.out; do
	echo $(head -n1 $f) $f
done | sort -n

