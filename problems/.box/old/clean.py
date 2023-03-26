#!/usr/bin/env python2.7
# encoding: utf-8

from glob import glob
from util import delete_file
import os
import os.path
import subprocess

def clean_backups(name):
    u"Delete backup files from problem `name`'s tree."
    for dirpath, _, backup_fns in os.walk('%s/' % name):
        for backup_fn in backup_fns:
            if backup_fn.endswith('~'):
                delete_file(os.path.join(dirpath, backup_fn))

def clean_build(name):
    subprocess.check_call(['rm', '-rf', '%s/build/tests/' % name])

def box_clean(name):
    u"Clean up problem tree, deleting all generated files."
    for pdf_fn in glob('%s/documents/*.pdf' % name):
        delete_file(pdf_fn)
    for log_fn in glob('%s/documents/*.log' % name):
        delete_file(log_fn)
    delete_file('%s/%s.in' % (name, name))
    delete_file('%s/%s.sol' % (name, name))
    for sol_fn in glob('%s/solutions/*/*.exe' % name):
        delete_file(sol_fn)
    for sol_fn in glob('%s/solutions/*/*.jexe' % name):
        delete_file(sol_fn)
    for sol_fn in glob('%s/solutions/*/*.class' % name):
        delete_file(sol_fn)
    for sol_fn in glob('%s/attic/*.exe' % name):
        delete_file(sol_fn)
    for sol_fn in glob('%s/attic/*.jexe' % name):
        delete_file(sol_fn)
    for sol_fn in glob('%s/attic/*.class' % name):
        delete_file(sol_fn)
    for sol_fn in glob('%s/checkers/*.exe' % name):
        delete_file(sol_fn)
    for sol_fn in glob('%s/checkers/*.jexe' % name):
        delete_file(sol_fn)
    for sol_fn in glob('%s/checkers/*.class' % name):
        delete_file(sol_fn)
    clean_backups(name)
    clean_build(name)

