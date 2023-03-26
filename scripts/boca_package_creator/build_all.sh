#!/bin/bash

for problem in ../../problems/*;
do
   name=`basename $problem`

   echo "Do you want to build $name?"
   select answer in "yes" "no";
   do
      case $answer in
         "yes")
            cd $problem
            #../../bin/box build
            ../../bin/box check
            cd -
         
            ./boca_packer.py @ $problem

            echo "Done"
            echo ""
            echo ""
            break
            ;;
         "no")
            echo "Ok. It is up to you!"
            break
            ;;
         *)
            echo "Wrong option! Select 1 or 2."
            exit 1
            ;;
      esac
   done;
done;
