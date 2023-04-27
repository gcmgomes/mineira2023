#!/usr/bin/env python3
# encoding: utf-8

import sys

for line in sys.stdin:
    if '  ' in line:
        sys.exit(1)
