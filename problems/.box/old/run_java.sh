#!/bin/sh

#java -Xms512m -Xmx512m -Xss20m -cp `dirname $0` `basename $0 .jexe`
java -Xss20m -cp `dirname $0` `basename $0 .jexe` $*
