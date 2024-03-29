#!/usr/bin/env python3
# encoding: utf-8

import os
import os.path

def delete_file(fn):
    "Delete file `fn` or return silently if it doesn't exist."
    try:
        os.unlink(fn)
    except OSError:
        pass

def change_extension(input_fn, new_extension):
    basename, extension = os.path.splitext(input_fn)
    return basename + '.' + new_extension
