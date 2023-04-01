#!/usr/bin/env python3

# Sample test generator

import sys,os,string
from random import seed, randint, sample

# first argument must be seed
seed(sys.argv[1])

n = int(sys.argv[2])
print(n)
for i in range(0, n):
  r = randint(0, 256)
  g = randint(0, 256)
  b = randint(0, 256)
  print(r, g, b)
