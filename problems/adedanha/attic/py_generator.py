#!/usr/bin/env python3
# encoding: utf-8

# Sample test generator

import sys
from random import seed, randint

# first argument must be seed
seed(sys.argv[1])

max_n = int(sys.argv[2])
max_k = int(sys.argv[3])

n = randint(1, max_n)
print(n)

for i in range(n - 1):
  k = randint(0, max_k)
  print(k, end='')
  if (i < n - 2):
    print(' ', end='')
  else:
    print()