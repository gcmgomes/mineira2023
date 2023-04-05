#!/usr/bin/env python3

# Sample test generator

import sys
import os
import string
from random import seed, randint, sample

# first argument must be seed
seed(sys.argv[1])
n = int(sys.argv[2])
m = int(sys.argv[3])
max_val = int(sys.argv[4])

print(n, m)
for i in range(0, n):
    line = []
    for j in range(0, m):
        line.append(randint(1, max_val))
    print(*line)
