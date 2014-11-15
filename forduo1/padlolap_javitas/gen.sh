#!/bin/bash

basename=$1
seed=$2

best=
bestFiles=
i=0

while true; do
	if [ -n "$seed" ]; then
		currentSeed=$((seed + i))
		name=${basename}_gen_seed_$currentSeed
	else
		currentSeed=
		name=${basename}_gen_$i
	fi
	echo "$i --> $name" >&2
	targetName=${name}.target
	outputName=${name}.out
	if ./build-clang_release/bin/findTarget ${basename}.txt $currentSeed >$targetName; then
		./build-clang_release/bin/padlolap ${basename}.txt $targetName >$outputName

		value=$(head -n1 $outputName)
		echo "value = $value (best=$best [$bestFiles])"
		if [ -z "$best" ] || [ "$best" -gt "$value" ]; then
			best=$value
			rm -f $bestFiles
			bestFiles="$outputName $targetName"
		else
			rm $outputName $targetName
		fi
	else
		rm $targetName
	fi
	((++i))
done

