#!/usr/bin/env python3
# encoding: utf-8

# Sample test generator

import sys
from random import seed, randint

# sets up seed to ensure that test cases are generated deterministically
# based on the input arguments
seed('|'.join(sys.argv[1:]))

n = int(sys.argv[2])
print(n)

for i in range(n):
    a, b = (1, 1)
    while a == b:
        a = randint(1, 3)
        b = randint(1, 3)
    print(a, b)
