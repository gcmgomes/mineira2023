#!/usr/bin/env python3
# encoding: utf-8

import sys
import os

BOCA_MAX_FILE_SIZE_IN_BYTES = 1 * 1024 * 1024 # 1mb

sys.stdin.seek(0, os.SEEK_END)
size_in_bytes = sys.stdin.tell()

if size_in_bytes > .95 * BOCA_MAX_FILE_SIZE_IN_BYTES:
    sys.exit(1)
