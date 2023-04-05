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
length = int(sys.argv[5])
is_col = int(sys.argv[6])

matriz = []
for i in range(0, n):
    line = []
    for j in range(0, m):
        line.append(randint(1, max_val))
    matriz.append(line)

if is_col:
    line_begin = randint(0, n - length)
    cool_col = randint(0, m-1)
    prev = randint(1, max_val)
    for i in range(0, length):
        prev += 1 - 2*randint(0, 1)
        matriz[line_begin+i][cool_col] = prev
else:
    cool_line = randint(0, n-1)
    col_begin = randint(0, m - length)
    prev = randint(1, max_val)
    for i in range(0, length):
        prev += 1 - 2*randint(0, 1)
        matriz[cool_line][col_begin+i] = prev

print(n, m)
for l in matriz:
    print(*l)
