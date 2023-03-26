#!/usr/bin/env python2.7
# encoding: utf-8

import getopt
import os
import os.path
import shutil
import subprocess
import sys
import math

from glob import glob
from tempfile import NamedTemporaryFile

import ui
from ui import OK, WARNING, ERROR
import hints
import clean
from util import delete_file, change_extension
from run import run_solution
from tex import build_pdf

def program_available(program):
    u"Check if `program` is available in the system PATH."
    with open('/dev/null', 'w') as dev_null:
        return subprocess.call(['which', program], stdout = dev_null) == 0

def box_build_package(pkg):
    u"Build Boca package."

    curdir=os.getcwd()
    if pkg=='contest':
        print '\nBuilding packages for contest session...'
        os.chdir('../boca/contest')
        try:
            shutil.rmtree('../../packages/contest')
        except:
            pass
    else:
        print '\nBuilding packages for warmup session...'
        os.chdir('../boca/warmup')
        try:
            shutil.rmtree('../../packages/warmup')
        except:
            pass
    if subprocess.call('../bin/build_packages.py', shell = True) == 0:
        ui.end_task('OK')
    else:
        ui.end_task('failed!')
    print '\nChecking...'
    if subprocess.call('./diff.sh', shell = True) == 0:
        try:
            os.mkdir('../../packages')
        except:
            pass
        if pkg=='contest':
            shutil.copytree('packages','../../packages/contest')
        else:
            shutil.copytree('packages','../../packages/warmup')
        ui.end_task('OK')
    else:
        ui.end_task('failed!')
    os.chdir(curdir)

def build_binaries(directory):
    u"Build all source code in `directory`."
    def build_binary(src_fn):
        u"Build the binary for `src_fn`."
        ui.start_task(src_fn)
        exe_fn = change_extension(src_fn, 'exe')
        if src_fn.endswith('.c'):
            cmd_line = 'gcc -O2 -lm -std=c99 -o %s %s' % (exe_fn, src_fn)
        elif src_fn.endswith('.cpp'):
            cmd_line = 'g++ -std=c++11 -O2 -o %s %s' % (exe_fn, src_fn)
        elif src_fn.endswith('.java'):
            exe_fn = change_extension(src_fn, 'jexe')
            cmd_line = ('javac %s && ln -sf `pwd`/.box/run_java.sh %s && '
                        'chmod a+x %s' % (src_fn, exe_fn, exe_fn))
        else:
            cmd_line = 'ln -sf `pwd`/%s %s && chmod a+x %s' % (src_fn, exe_fn, exe_fn)

        if subprocess.call(cmd_line, shell = True) == 0:
            ui.end_task('OK')
        else:
            ui.end_task('failed')

    for src_fn in glob(os.path.join(directory, '*.c')):
        build_binary(src_fn)
    for src_fn in glob(os.path.join(directory, '*.cpp')):
        build_binary(src_fn)
    for src_fn in glob(os.path.join(directory, '*.java')):
        build_binary(src_fn)
    for src_fn in glob(os.path.join(directory, '*.py')):
        build_binary(src_fn)

def mycomp(a,b):
    al=a.split('/')
    bl=b.split('/')
    if int(al[3])>int(bl[3]):
        return 1
    elif int(al[3])<int(bl[3]):
        return -1
    else:
        return 0
        tmpa=al[4].replace(".in","")
        tmpb=bl[4].replace(".in","")
        if int(tmpa)>int(tmpb):
            return 1
        elif int(tmpa)<int(tmpb):
            return -1
        else:
            return 0

def input_files(name):
    tmp=glob('%s/build/tests/*/*.in' % name)
    tmp.sort(cmp=mycomp)
    return tmp

def sample_input_files(name):
    tmp=glob('%s/documents/sample-*.in' % name)
    tmp.sort()
    return tmp

def output_files(name):
    tmp=glob('%s/build/tests/*/*.sol' % name)
    tmp.sort(cmp=mycomp)
    return tmp

def sample_output_files(name):
    tmp=glob('%s/documents/sample-*.sol' % name)
    tmp.sort()
    return tmp

def box_new(name):
    u"Create a blank tree for a new problem."
    shutil.copytree('.box/skel/', '%s/' % name, symlinks = True)

def box_build_pdf(name):
    u"Compile PDFs for all documents of problem `name`."
    ui.task_header(name, 'Compiling documents...')
    if not program_available('pdflatex'):
        ui.show_message('Error', 'Could not find `pdflatex`.', ERROR)
        hints.give_hint('pdflatex')
        return
    for tex_fn in glob('%s/documents/*.tex' % name):
        pdf_fn = change_extension(tex_fn, 'pdf')
        build_pdf(pdf_fn, tex_fn)

def box_build_input(name):
    u"Build the input file for problem `name`."
    ui.task_header(name, 'Building generators...')
    build_binaries('%s/attic/' % name)
    ui.task_header(name, 'Generating input files...')

    clean.clean_build(name)
    if not os.path.exists('%s/build/' % name):
        os.mkdir('%s/build/' % name)
    if not os.path.exists('%s/build/tests/' % name):
        os.mkdir('%s/build/tests/' % name)
    with open('/dev/null', 'w') as dev_null:
        subprocess.check_call(['%s/attic/build-tests' % name],
                              env = { 'ROOT': '%s' % name })
                              #stderr = dev_null)

def box_check_input(name):
    u"Check the input file for problem `name`."
    global short_mode

    def run_checker(checker_fn):
        u"Run a single checker program against the input file."
        ui.task_header(name, 'Validating input with %s...' % checker_fn)
        if not os.access(checker_fn, os.X_OK):
            ui.show_message('Warning', '%s is not an executable.' % checker_fn, WARNING)
            return
        for input_fn in sample_input_files(name)+input_files(name):
            if short_mode:
                sys.stdout.write(" ") # start task for short mode
            else:
                ui.start_task(input_fn)
            with open(input_fn, 'r') as input_file:
                with open('/dev/null', 'w') as dev_null:
                    if subprocess.call(checker_fn,
                                       stdin = input_file,
                                       stdout = dev_null,
                                       stderr = dev_null) != 0:
                        ui.end_task(failed_msg,ERROR,short)
                    else:
                        ui.end_task(OK_msg,OK,short)

    if short_mode:
        OK_msg,failed_msg,short='.','X','short'
    else:
        OK_msg,failed_msg,short='OK','failed',None
    ui.task_header(name, 'Building checkers...')
    build_binaries('%s/checkers/' % name)
    input_checkers = glob('%s/checkers/*.*exe' % name)
    if not input_checkers:
        ui.show_message('Warning', 'No input checkers available.', WARNING)
        return
    for checker_fn in input_checkers:
        if checker_fn.find("output")>0:
            continue
        run_checker(checker_fn)
    if short: print


def box_check_output(name):
    u"Check the output file for problem `name`."
    global short_mode

    def run_checker(checker_fn):
        u"Run a single checker program against the input file."
        ui.task_header(name, 'Validating output with %s...' % checker_fn)
        if not os.access(checker_fn, os.X_OK):
            ui.show_message('Warning', '%s is not an executable.' % checker_fn, WARNING)
            return
        for output_fn in sample_output_files(name)+output_files(name):
            if short_mode:
                sys.stdout.write(" ") # start task for short mode
            else:
                ui.start_task(output_fn)
            with open(output_fn, 'r') as output_file:
                with open('/dev/null', 'w') as dev_null:
                    if subprocess.call(checker_fn,
                                       stdin = output_file,
                                       stdout = dev_null,
                                       stderr = dev_null) != 0:
                        ui.end_task(failed_msg,ERROR,short)
                    else:
                        ui.end_task(OK_msg,OK,short)

    if short_mode:
        OK_msg,failed_msg,short='.','X','short'
    else:
        OK_msg,failed_msg,short='OK','failed',None
    ui.task_header(name, 'Building checkers...')
    build_binaries('%s/checkers/' % name)
    output_checkers = glob('%s/checkers/*output*.*exe' % name)
    if not output_checkers:
        ui.show_message('Warning', 'No output checkers available.', WARNING)
        return
    for checker_fn in output_checkers:
        
        run_checker(checker_fn)
    if short: print


def box_build_good_solutions(name):
    u"Compile all good solutions for problem `name`."
    ui.task_header(name, 'Building solutions...')
    build_binaries('%s/solutions/reference/' % name)
    build_binaries('%s/solutions/good/' % name)

def box_build_solutions(name):
    u"Compile all solutions for problem `name`."
    ui.task_header(name, 'Building solutions...')
    build_binaries('%s/solutions/reference/' % name)
    build_binaries('%s/solutions/good/' % name)
    build_binaries('%s/solutions/slow/' % name)
    build_binaries('%s/solutions/wrong/' % name)

def reference_solution(name):
    good_solutions = [solution for solution, status in solutions(name)
                      if status == 'AC']
    if not good_solutions:
        return None
    return good_solutions[0]

def build_one_output(name, input_fn):
    output_fn = change_extension(input_fn, 'sol')
    delete_file(output_fn)
    result = run_solution(reference_solution(name), input_fn, output_fn)
    return result.status == 'OK'

def box_build_output(name):
    u"Build output file for problem `name`."
    global short_mode
    if short_mode:
        OK_msg,failed_msg,short='.','X','short'
    else:
        OK_msg,failed_msg,short='OK','failed',None
    ui.task_header(name, 'Generating problem output...')

    if reference_solution(name) is None:
        ui.show_message('Error', 'No good solutions available.', ERROR)
        hints.give_hint('output-good-solutions')
        return False

    for input_fn in input_files(name):
        output_fn = change_extension(input_fn, 'sol')
        if short_mode:
            sys.stdout.write(" ") # start task for short mode
        else:
            ui.start_task(output_fn)
        if build_one_output(name, input_fn):
            ui.end_task(OK_msg,OK,short)
        else:
            ui.end_task(failed_msg,ERROR,short)
    if short: print
    return True

def solutions(name,type='all'):
    if type=='all' or type=='reference':
        for sol_fn in glob('%s/solutions/reference/*.*exe' % name):
            yield sol_fn, 'AC'
    if type=='all' or type=='good':
        for sol_fn in glob('%s/solutions/good/*.*exe' % name):
            yield sol_fn, 'AC'
    if type=='all' or type=='wrong':
        for sol_fn in glob('%s/solutions/wrong/*.*exe' % name):
            yield sol_fn, 'WA'
    if type=='all' or type=='slow':
        for sol_fn in glob('%s/solutions/slow/*.*exe' % name):
            yield sol_fn, 'TLE'

def box_check_solutions(name, do_slow=True):
    u"Check all solutions for problem `name`."
    global short_mode
    if short_mode:
        OK_msg,timeout_msg,wrong_msg,exec_msg,short='.','T','W','X','short'
    else:
        OK_msg,timeout_msg,wrong_msg,exec_msg,short='OK','timeout','wrong','failed',None
    # problem specs, to be read
    jtime_limit = 60
    ctime_limit = 60
    mem_limit = 1024
    file_limit = 1024
    #try:
    #    exec open('%s/attic/problem.desc' % name).read()  in globals(), locals()
    #except:
    #    pass
    for sol_fn, expected in solutions(name):
        ui.task_header(name, 'Testing %s on sample input...' % sol_fn)
        for input_fn in sample_input_files(name):
            if short_mode:
                sys.stdout.write(" ") # start task for short mode
            else:
                ui.start_task(input_fn)
            basename, extension = os.path.splitext(sol_fn)
            if extension=='.jexe':
                the_time_limit=jtime_limit
            else:
                the_time_limit=ctime_limit
            result = run_solution(sol_fn, input_fn,
                                  reference_fn = change_extension(input_fn, 'sol'), 
                                  time_limit=the_time_limit,mem_limit=mem_limit,file_limit=file_limit)
            if result.status == 'RE':
                ui.end_task(exec_msg, ERROR, short)
                #ui.end_task(result.detail, ERROR, short)
            elif result.status == 'TLE':
                color = [WARNING, OK][expected == result.status]
                ui.end_task(timeout_msg, color, short)
            elif result.status == 'WA':
                color = [ERROR, OK][expected == 'WA']
                ui.end_task(wrong_msg, color, short)
                hints.give_hint('solution-wrong-sample')
            elif result.status == 'AC':
                color = [ERROR, OK][expected == 'AC']
                ui.end_task(OK_msg, color, short)
            else:
                assert False

    cgood_solutions_time,cslow_solutions_time=[],[]
    jgood_solutions_time,jslow_solutions_time=[],[]
    def run_check(name,type,ctime_limit=100,jtime_limit=100):
      for sol_fn, expected in solutions(name,type):
        cur_cgood_solutions_time,cur_jgood_solutions_time=[],[]  
        cur_cslow_solutions_time,cur_jslow_solutions_time=[],[]  
        ui.task_header(name, 'Testing %s on REAL input...' % sol_fn)
        basename, extension = os.path.splitext(sol_fn)
        if extension=='.jexe':
            time_limit=jtime_limit
        else:
            time_limit=ctime_limit
        for input_fn in input_files(name):
            if short_mode:
                sys.stdout.write(" ") # start task for short mode
            else:
                ui.start_task(input_fn)

            result = run_solution(sol_fn, input_fn,
                                  reference_fn = change_extension(input_fn, 'sol'),
                                  time_limit=time_limit,mem_limit=mem_limit,file_limit=file_limit)
            if result.status == 'RE':
                ui.end_task(exec_msg, ERROR, short)
                #ui.end_task(result.detail, ERROR, short)
            elif result.status == 'TLE':
                color = [WARNING, OK][expected == result.status]
                ui.end_task(timeout_msg, color, short)
            elif result.status == 'WA':
                color = [ERROR, OK][expected == 'WA']
                ui.end_task(wrong_msg, color, short)
            elif result.status == 'AC':
                color = [ERROR, OK][expected == 'AC']
                ui.end_task(OK_msg, color, short)
            else:
                assert False
            if extension=='.jexe':
                if expected == 'AC':
                    cur_jgood_solutions_time.append(result.running_time)
                elif expected == 'TLE':
                    cur_jslow_solutions_time.append(result.running_time)
            else:
                if expected == 'AC':
                    cur_cgood_solutions_time.append(result.running_time)
                elif expected == 'TLE':
                    cur_cslow_solutions_time.append(result.running_time)

        if len(cur_cgood_solutions_time)!=0:
          cgood_solutions_time.append(max(cur_cgood_solutions_time))
          print ' Total time: %.2f s  (worst case: %.2f)' %  (sum(cur_cgood_solutions_time),max(cur_cgood_solutions_time))
        if len(cur_cslow_solutions_time)!=0:
          cslow_solutions_time.append(max(cur_cslow_solutions_time))
          print ' Total time: %.2f s  (worst case: %.2f)' %  (sum(cur_cslow_solutions_time),max(cur_cslow_solutions_time))
        if len(cur_jgood_solutions_time)!=0:
          jgood_solutions_time.append(max(cur_jgood_solutions_time))
          print ' Total time: %.2f s  (worst case: %.2f)' %  (sum(cur_jgood_solutions_time),max(cur_jgood_solutions_time))
        if len(cur_jslow_solutions_time)!=0:
          jslow_solutions_time.append(max(cur_jslow_solutions_time))
          print ' Total time: %.2f s  (worst case: %.2f)' %  (sum(cur_jslow_solutions_time),max(cur_jslow_solutions_time))

    run_check(name,'reference')
    ctime_limit=newtimelimit(cgood_solutions_time)
    jtime_limit=newtimelimit(jgood_solutions_time)
    print
    print '**** C/C++ Reference Time Limit: %5.2f       Java Reference Time Limit: %5.2f' % (ctime_limit,jtime_limit)
    run_check(name,'good')
    ctime_limit=timelimit(cgood_solutions_time)
    jtime_limit=timelimit(jgood_solutions_time)
    print
    print '**** C/C++ Time Limit: %5.2f       Java New Time Limit: %5.2f' % (ctime_limit,jtime_limit)
    # write time limits in file attic/problem.desc
    lines=[]
    try:
        with open('%s/attic/problem.desc' % name, 'r') as desc_fn:
            lines=desc_fn.readlines()
    except:
        pass
    with open('%s/attic/problem.desc' % name, 'w') as desc_fn:
        for l in lines:
            if l.find('time_limit')==-1:
                desc_fn.write(l)
        desc_fn.write('time_limit_c=%d\ntime_limit_java=%d\n' % (ctime_limit, jtime_limit))

    if do_slow:
        run_check(name,'slow',ctime_limit,jtime_limit)
        if short_mode: print
        run_check(name,'wrong')
        if short_mode: print

def timelimit(times):
    # times.sort()
    # print '*****\ntimes'
    # for f in times:
    #     print "%3.2f" % (f), 
    # print '\n***'
    try:
        maxtime=max(times)
        mintime=min(times)
    except:
        maxtime=0
        mintime=0
    return math.ceil(max(1, 4*mintime, 1.5*maxtime))

def newtimelimit(times):
    try:
        maxtime=max(times)
    except:
        maxtime=0
    return max(math.floor(3*maxtime+0.8),1)

def box_time_solutions(name,do_slow=True):
    u"Run and time all solutions for problem `name`."
    global short_mode
    if short_mode:
        OK_msg,timeout_msg,wrong_msg,exec_msg,short='.','T','W','X','short'
    else:
        OK_msg,timeout_msg,wrong_msg,exec_msg,short='OK','timeout','wrong','failed',None
    # problem specs, to be read
    jtime_limit = 60
    ctime_limit = 60
    mem_limit = 1024
    file_limit = 1024
    #try:
    #    exec open('%s/attic/problem.desc' % name).read()  in globals(), locals()
    #except:
    #    pass

    cgood_solutions_time,cslow_solutions_time=[],[]
    jgood_solutions_time,jslow_solutions_time=[],[]
    def time_run(type,ctime_limit=20,jtime_limit=20):
      for sol_fn, expected in solutions(name,type):
        cur_cgood_solutions_time,cur_jgood_solutions_time=[],[]  
        cur_cslow_solutions_time,cur_jslow_solutions_time=[],[]  
        ui.task_header(name, 'Timing %s on REAL input...' % sol_fn)
        basename, extension = os.path.splitext(sol_fn)
        if extension=='.jexe':
            time_limit=jtime_limit
        else:
            time_limit=ctime_limit
        for input_fn in input_files(name):
            if short_mode:
                sys.stdout.write(" ") # start task for short mode
            else:
                ui.start_task(input_fn)

            result = run_solution(sol_fn, input_fn,
                                  time_limit=time_limit,mem_limit=mem_limit,file_limit=file_limit)

            if result.status == 'RE':
                ui.end_task(result.detail, ERROR, short)
            elif result.status == 'TLE':
                color = [WARNING, OK][expected == result.status]
                ui.end_task(timeout_msg, color, short)
            elif result.status == 'OK':
                color = [WARNING, OK][expected != result.status]
                if short_mode:
                    ui.end_task(OK_msg, color, short)
                else:
                    ui.end_task('%5.2f s' % result.running_time, color)
            else:
                assert False
            if extension=='.jexe':
                if expected == 'AC':
                    cur_jgood_solutions_time.append(result.running_time)
                elif expected == 'TLE':
                    cur_jslow_solutions_time.append(result.running_time)
            else:
                if expected == 'AC':
                    cur_cgood_solutions_time.append(result.running_time)
                elif expected == 'TLE':
                    cur_cslow_solutions_time.append(result.running_time)

        if len(cur_cgood_solutions_time)!=0:
          cgood_solutions_time.append(max(cur_cgood_solutions_time))
          print ' Total time: %.2f s  (worst case: %.2f)' %  (sum(cur_cgood_solutions_time),max(cur_cgood_solutions_time))
        if len(cur_cslow_solutions_time)!=0:
          cslow_solutions_time.append(max(cur_cslow_solutions_time))
          print ' Total time: %.2f s  (worst case: %.2f)' %  (sum(cur_cslow_solutions_time),max(cur_cslow_solutions_time))
        if len(cur_jgood_solutions_time)!=0:
          jgood_solutions_time.append(max(cur_jgood_solutions_time))
          print ' Total time: %.2f s  (worst case: %.2f)' %  (sum(cur_jgood_solutions_time),max(cur_jgood_solutions_time))
        if len(cur_jslow_solutions_time)!=0:
          jslow_solutions_time.append(max(cur_jslow_solutions_time))
          print ' Total time: %.2f s  (worst case: %.2f)' %  (sum(cur_jslow_solutions_time),max(cur_jslow_solutions_time))
      
    time_run('reference')
    ctime_limit=newtimelimit(cgood_solutions_time)
    jtime_limit=newtimelimit(jgood_solutions_time)
    print
    print '**** C/C++ Reference Time Limit: %5.2f       Java Reference Time Limit: %5.2f' % (ctime_limit,jtime_limit)

    time_run('good')
    ctime_limit=timelimit(cgood_solutions_time)
    jtime_limit=timelimit(jgood_solutions_time)
    print
    print '**** C/C++ Time Limit: %5.2f       Java Time Limit: %5.2f' % (ctime_limit,jtime_limit)
    # write time limits in file attic/problem.desc
    lines=''
    try:
        with open('%s/attic/problem.desc' % name, 'r') as desc_fn:
            lines=desc_fn.readlines()
    except:
        pass
    with open('%s/attic/problem.desc' % name, 'w') as desc_fn:
        for l in lines:
            if l.find('time_limit')==-1:
                desc_fn.write(l)
        desc_fn.write('time_limit_c=%d\ntime_limit_java=%d\n' % (ctime_limit, jtime_limit))

    if do_slow:
        time_run('slow',ctime_limit,jtime_limit)

    print '\nSummary:'
    print '  C/C++ Time Limit: %5.2f       Java Time Limit: %5.2f' % (ctime_limit,jtime_limit)
    if len(cgood_solutions_time)>0:
        print '  C/C++ good: [%5.2f %5.2f]' % (min(cgood_solutions_time), max(cgood_solutions_time)),
    if len(cslow_solutions_time)>0:
        print '    slow: [%5.2f %5.2f]' % (min(cslow_solutions_time), max(cslow_solutions_time)),
    print
    if len(jgood_solutions_time)>0:
        print '  Java  good: [%5.2f %5.2f]' % (min(jgood_solutions_time), max(jgood_solutions_time)),
    if len(jslow_solutions_time)>0:
        print '    slow: [%5.2f %5.2f]' % (min(jslow_solutions_time), max(jslow_solutions_time)),
    print

def parse_arguments():
    global short_mode
    short_mode=False
    optlist, args = getopt.gnu_getopt(sys.argv[1:], 'l:s')
    for flag, arg in optlist:
        if flag == '-l':
            sys.stdout.dumb_taps.append(open(arg, 'w'))
        if flag == '-s':
            short_mode=True
    last_dir = None
    while not os.path.exists('.box'):
        head, tail = os.getcwd(), None
        while not tail:
            head, tail = os.path.split(head)
        last_dir = tail
        os.chdir('..')
        if os.getcwd() == '/':
            ui.show_message('Error', 'Could not find .box/ directory.', ERROR)
            ui.usage()
    if last_dir:
        args.append(last_dir)
    try:
        cmd = args.pop(0)
    except IndexError:
        ui.usage()
    if args:
        targets = [arg.rstrip('/') for arg in args]
    else:
        targets = [directory.rstrip('/') for directory in glob('*/')]
    return cmd, targets

def _main():
    cmd, targets = parse_arguments()

    if cmd == 'version':
        with open('.box/VERSION') as version_file:
            print 'box %s' % (version_file.read(), )
            sys.exit(0)

    if cmd == 'build-packages':
        box_build_package('warmup')
        box_build_package('contest')
        sys.exit(0)
    elif cmd == 'build-package-warmup':
        box_build_package('warmup')
        sys.exit(0)
    elif cmd == 'build-package-contest':
        box_build_package('contest')
        sys.exit(0)
    else:        
      for target in targets:
        clean.clean_backups(target)
        if cmd == 'new':
            box_new(target)
        elif cmd == 'build-pdf':
            box_build_pdf(target)
        elif cmd == 'build-input':
            box_build_input(target)
        elif cmd == 'build-output':
            box_build_output(target)
        elif cmd == 'build-solutions':
            box_build_solutions(target)
        elif cmd == 'check-solutions':
            box_check_solutions(target)
        elif cmd == 'check-input':
            box_check_input(target)
        elif cmd == 'check-output':
            box_check_output(target)
        elif cmd == 'build':
            box_build_pdf(target)
            box_build_input(target)
            box_build_solutions(target)
            box_build_output(target)
        elif cmd == 'check':
            box_build_pdf(target)
            box_build_input(target)
            box_check_input(target)
            box_build_solutions(target)
            box_build_output(target)
            box_check_output(target)
            box_check_solutions(target,do_slow=True)
        elif cmd == 'check-good':
            box_build_pdf(target)
            box_build_input(target)
            box_check_input(target)
            box_build_solutions(target)
            box_build_output(target)
            box_check_output(target)
            box_check_solutions(target,do_slow=False)
        elif cmd == 'clean':
            clean.box_clean(target)
        elif cmd == 'time':
            box_time_solutions(target)
        elif cmd == 'time-good':
            box_time_solutions(target,do_slow=False)
        elif cmd in ('commit', 'push', 'pull') and program_available('sl'):
            os.system('sl')
            sys.exit(1)
        else:
            ui.usage()
        hints.show_hints()

if __name__ == '__main__':
    _main()
