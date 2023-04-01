#!/bin/sh
name=`basename $0 .exe`
first_to_upper=`echo $name | cut -c 1-1 | tr [:lower:] [:upper:]`
rest=`echo $name | cut -c 2-`
kotlin_name="$first_to_upper${rest}Kt"
java -Xms1024m -Xmx1024m -Xss100m -cp `dirname $0`/$name.jar $kotlin_name $*
