#!/usr/bin/env python

# Sample test generator

import sys,os,string
from random import seed, randint, sample

# first argument must be seed
seed(sys.argv[1])
MIN=int(sys.argv[2])
MAX=int(sys.argv[3])

n = randint(MIN,MAX)
print n
for i in range(n):
    print randint(1,n)
