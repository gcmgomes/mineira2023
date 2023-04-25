#!/usr/bin/env python3
# encoding: utf-8

import sys

for line in sys.stdin:
    for token in line.split():
        if token.startswith('0') and token != '0':
            sys.exit(1)
