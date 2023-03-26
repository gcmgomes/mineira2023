#!/bin/bash

rm -rf *.png *.jpg *.html input/* output/* solutions/good/* solutions/slow/*
./get_data.py $1
./compile_statement.py $1

cp $1/solutions/good/*.{cpp,cc,py,java} solutions/good/
cp $1/solutions/slow/*.{cpp,cc,py,java} solutions/slow/

name=`basename $1`
zip  $name.zip *.html *.yaml *.jpg *.png input/* output/* solutions/good/* solutions/slow/*
cp $name.zip ~/Dropbox/Mineira2018/URI/
