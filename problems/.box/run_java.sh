#!/bin/sh

java -Xms1024m -Xmx1024m -Xss100m -cp `dirname $0` `basename $0 .exe` $*
