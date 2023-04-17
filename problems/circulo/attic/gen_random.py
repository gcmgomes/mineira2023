#!/usr/bin/env python3

# Sample test generator

import sys
import os
import string
from random import seed, randint, shuffle

# first argument must be seed
seed(sys.argv[1])
n = int(sys.argv[2])
max_color_count = int(sys.argv[3])
rem = n
i = 1
cris = []
while rem > 0:
    cur = min(randint(1, max_color_count), rem)
    rem -= cur
    cris.extend([i]*cur)
    i += 1
shuffle(cris)
print(n)
print(*cris)
