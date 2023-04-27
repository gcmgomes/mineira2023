#!/usr/bin/env python3

# Sample test generator

import sys
from random import seed, randint

# first argument must be seed
seed(sys.argv[1])

n = int(sys.argv[2])
print(n)
for i in range(0, n):
  r = randint(0, 255)
  g = randint(0, 255)
  b = randint(0, 255)
  print(r, g, b)
