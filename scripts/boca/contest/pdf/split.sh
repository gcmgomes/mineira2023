#!/bin/sh

PDF="../../../contest/contest_no_number.pdf"

gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=2 -dLastPage=2 -sDEVICE=pdfwrite -o info.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=3 -dLastPage=4 -sDEVICE=pdfwrite -o A.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=5 -dLastPage=5 -sDEVICE=pdfwrite -o B.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=6 -dLastPage=7 -sDEVICE=pdfwrite -o C.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=8 -dLastPage=8 -sDEVICE=pdfwrite -o D.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=9 -dLastPage=9 -sDEVICE=pdfwrite -o E.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=10 -dLastPage=11 -sDEVICE=pdfwrite -o F.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=12 -dLastPage=14 -sDEVICE=pdfwrite -o G.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=15 -dLastPage=15 -sDEVICE=pdfwrite -o H.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=16 -dLastPage=16 -sDEVICE=pdfwrite -o I.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=17 -dLastPage=17 -sDEVICE=pdfwrite -o J.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=18 -dLastPage=19 -sDEVICE=pdfwrite -o K.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=20 -dLastPage=20 -sDEVICE=pdfwrite -o L.pdf  ${PDF}
gs -q -dBATCH -dSAFER -dNOPAUSE -dFirstPage=21 -dLastPage=23 -sDEVICE=pdfwrite -o M.pdf  ${PDF}
