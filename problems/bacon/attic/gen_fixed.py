#!/usr/bin/env python

import sys,os,string
from random import seed, randint, sample

# first argument is always seed
seed(sys.argv[1])
T=int(sys.argv[2])
c = [0]*5
for i in xrange(5):
    c[i] = int(sys.argv[i+3])

print T
for i in xrange(4):
    print c[i],
print c[4]
