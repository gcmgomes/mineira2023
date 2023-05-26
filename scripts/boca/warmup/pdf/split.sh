#!/bin/sh

PDF="../../../contest/warmup_no_number.pdf"

gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=2 -dLastPage=2 -sDEVICE=pdfwrite -o info.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=3 -dLastPage=4 -sDEVICE=pdfwrite -o A.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=5 -dLastPage=5 -sDEVICE=pdfwrite -o B.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=6 -dLastPage=6 -sDEVICE=pdfwrite -o C.pdf  ${PDF}
