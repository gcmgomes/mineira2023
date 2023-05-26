#!/bin/bash


PROBLEMS="../../problems"

declare -a pcodes=(A B C D E F G H I J K L M)
declare -a pnames=(pyramid balls long_city umbrellas board tree_pruning gravitational horse_race full_meal photographs cake periodic_partition maze_bolt)

for p in `seq 0 20`; do
    if [ ! ${pnames[$p]} ]
    then
	exit 1
    fi
    if [ ! -e $PROBLEMS/${pnames[$p]} ]
    then
	echo 'cannot find problem' $PROBLEMS/${pnames[$p]}
	exit 1
    fi
    echo
    echo 'Problem' ${pcodes[$p]} '('${pnames[$p]}')'

    solution=$(eval ls ${PROBLEMS}/${pnames[$p]}/solutions/good/*reference*.exe)
    if [ ! -e ${solution} ]
    then
 	echo 'Cannot find reference solution '}
 	#exit -1
    fi
    for i in `seq 1 200`; do
 	if [ ! -e problems/${pcodes[$p]}/input/ ]
	then
	    echo "cannot find input data"
	    exit 1
	fi
 	if [ -e problems/${pcodes[$p]}/input/${pcodes[$p]}_$i ]
 	then
 	    echo -n ${pcodes[$p]}_$i " "
 	    ${solution} < problems/${pcodes[$p]}/input/${pcodes[$p]}_$i  | diff -q -  problems/${pcodes[$p]}/output/${pcodes[$p]}_$i
 	fi
    done
    echo 
done

echo ${PROBLEMS}