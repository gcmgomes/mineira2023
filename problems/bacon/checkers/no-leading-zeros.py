#!/usr/bin/env python2.7
# encoding: utf-8

import sys

for line in sys.stdin.xreadlines():
    for token in line.split():
        if token.startswith('0') and token != '0':
            sys.exit(1)
