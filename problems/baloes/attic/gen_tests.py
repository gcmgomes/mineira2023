#!/usr/bin/env python2.7
# encoding: utf-8

import os
import shutil
import subprocess
import sys

from glob import glob
from tempfile import NamedTemporaryFile
from StringIO import StringIO


ROOT= os.environ['ROOT']
TESTDIR=os.path.join(os.path.join(ROOT, 'build'), 'tests')
ATTICDIR=os.path.join(ROOT, 'attic')
PLAN='testplan.txt'


def usage():
    basename = os.path.basename(sys.argv[0])
    print >> sys.stderr, 'Usage:'
    print >> sys.stderr,'\t%s \n' % basename
    sys.exit(1)

def _main():
    plan_fn=os.path.join(ATTICDIR,PLAN)
    if not os.path.exists(plan_fn):
        print >> sys.stderr, "Could not find %s file" % plan_fn
        usage()
    tmp=open(plan_fn, 'r')
    lines=tmp.readlines()
    tmp.close()
    last_test=-1
    for l in lines:
        l=l.strip()
        if l=='' or l[0]=='#': continue
        (test,command)=l.split(';')
        test=test.strip()
        command=command.strip()
        tks=command.split()
        test=int(test)
        if test!=last_test: # start new test group
            last_test=test
            case_num=0
            tdir=os.path.join(TESTDIR,'%d' % test)
            if not os.path.exists(tdir):
                os.mkdir(tdir)
                
        case_num+=1
        case_fn="%d.in" % case_num
        output_fn=os.path.join(tdir, case_fn)
        if tks[0] == 'copy':
            src = os.path.join(ROOT, tks[1])
            shutil.copyfile(src, output_fn)
            continue
        cmd=[]
        cmd.append(os.path.join(ATTICDIR, tks[0])) # fix path to executable
        cmd.append('%s-%d' % (tdir,case_num))
        for t in tks[1:]:
            cmd.append(t)

        with open(output_fn, 'w') as output_file:
            result=subprocess.call(cmd,
                                   stdout = output_file,
                                   stderr = None)
            if result!=0:
                print >> sys.stderr, command, "execution failed!"
                sys.exit(1)

if __name__ == '__main__':
    _main()
