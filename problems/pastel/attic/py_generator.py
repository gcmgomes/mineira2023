#!/usr/bin/env python3
# encoding: utf-8

# Sample test generator

import sys
from random import seed, randint

# sets up seed to ensure that test cases are generated deterministically
# based on the input arguments
seed('|'.join(sys.argv[1:]))

MIN=int(sys.argv[1])
MAX=int(sys.argv[2])

n = randint(MIN, MAX)
print(n)
for i in range(n):
    print(randint(1, n))
