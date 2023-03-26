#!/usr/bin/env python

# Sample test generator

import sys,os,string
from random import seed, randint, sample

# first argument must be seed
seed(sys.argv[1])
max_n = int(sys.argv[3])
max_k = int(sys.argv[2])

n = randint(1, max_n)
k = randint(1, max_k)
print("{} {}".format(n, k))
