#!/bin/bash

PROBLEMS="../../problems"
declare -a pcodes=(A B C)
declare -a pnames=(warmup_origami warmup_money warmup_account)

if [ ! -e solutions ]
then
    mkdir solutions
else
    rm -rf solutions/*
fi

for p in `seq 0 2`; do
    if [ ! -e $PROBLEMS/${pnames[$p]} ]
    then
	echo 'cannot find problem' $PROBLEMS/${pnames[$p]}
	exit 1
    fi
    echo
    echo 'Problem' ${pcodes[$p]} '('${pnames[$p]}')'
    for s in $PROBLEMS/${pnames[$p]}/solutions/good/*{c,cpp,java,kt,py}; do
	cp  -v $s solutions/${pcodes[$p]}-`basename $s`
    done
    for s in $PROBLEMS/${pnames[$p]}/solutions/slow/*{c,cpp,java,kt,py}; do
	cp  -v $s solutions/${pcodes[$p]}-SLOW-`basename $s`
    done
    for s in $PROBLEMS/${pnames[$p]}/solutions/wrong/*{c,cpp,java,kt,py}; do
	cp  -v $s solutions/${pcodes[$p]}-WRONG-`basename $s`
    done

done
