#!/usr/bin/env python3
# encoding: utf-8

import functools
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
from ui import OK, WARNING, ERROR, INFO
import hints
import clean
from util import delete_file, change_extension
from run import run_solution
from tex import build_pdf

C_LANG = "C/C++"
JAVA_LANG = "Java/Kotlin"
PYTHON_LANG = "Python"


def program_available(program):
    "Check if `program` is available in the system PATH."
    with open('/dev/null', 'w') as dev_null:
        return subprocess.call(['which', program], stdout=dev_null) == 0


def box_build_package(pkg):
    "Build Boca package."

    curdir = os.getcwd()
    if pkg == 'contest':
        print('\nBuilding packages for contest session...')
        os.chdir('../boca/contest')
        try:
            shutil.rmtree('../../packages/contest')
        except:
            pass
    else:
        print('\nBuilding packages for warmup session...')
        os.chdir('../boca/warmup')
        try:
            shutil.rmtree('../../packages/warmup')
        except:
            pass
    if subprocess.call('../bin/build_packages.py', shell=True) == 0:
        ui.end_task('OK')
    else:
        ui.end_task('failed!')
    print('\nChecking...')
    if subprocess.call('./diff.sh', shell=True) == 0:
        try:
            os.mkdir('../../packages')
        except:
            pass
        if pkg == 'contest':
            shutil.copytree('packages', '../../packages/contest')
        else:
            shutil.copytree('packages', '../../packages/warmup')
        ui.end_task('OK')
    else:
        ui.end_task('failed!')
    os.chdir(curdir)


def build_binaries(directory):
    "Build all source code in `directory`."
    def build_binary(src_fn):
        "Build the binary for `src_fn`."
        ui.start_task(src_fn)
        exe_fn = change_extension(src_fn, 'exe')
        jar_fn = change_extension(src_fn, 'jar')
        src_dir = os.path.dirname(src_fn)
        # get compilers from environment
        gcc = os.getenv('CC')
        if not gcc:
            gcc = 'gcc'
        gpp = os.getenv('CXX')
        if not gpp:
            gpp = 'g++'

        if src_fn.endswith('.c'):
            cmd_line = '%s -g -O2 -std=gnu11 -static -o %s %s -lm' % (
                gcc, exe_fn, src_fn)
        elif src_fn.endswith('.cpp'):
            extra_cpp_includes = (' -I`pwd`/.box/resources') if 'solutions' not in src_dir else ''
            cmd_line = '%s%s -g -O2 -std=gnu++17 -static -o %s %s' % (
                gpp, extra_cpp_includes, exe_fn, src_fn)
        elif src_fn.endswith('.java'):
            cmd_line = ('javac -sourcepath %s -d %s %s'
                        ' && ln -sf `pwd`/.box/run_java.sh %s'
                        ' && chmod a+x %s' % (src_dir, src_dir, src_fn, exe_fn, exe_fn))
        elif src_fn.endswith('.kt'):
            cmd_line = ('kotlinc -J-Xms1024m -J-Xmx1024m -J-Xss100m -include-runtime -d %s %s'
                        ' && ln -sf `pwd`/.box/run_kotlin.sh %s'
                        ' && chmod a+x %s' % (jar_fn, src_fn, exe_fn, exe_fn))
        elif src_fn.endswith('.py'):
            cmd_line = (f'python3 -m py_compile {src_fn}'
                        f' && ln -sf `pwd`/{src_fn} {exe_fn}'
                        f' && chmod a+x {exe_fn}')
        else:
            ui.show_message(
                'Error', 'Unknown extension for source file.', ERROR)
            ui.end_task('failed')

        if sys.platform == 'darwin':
            cmd_line = cmd_line.replace(' -static ', ' ').replace(' -g ', ' ')

        if subprocess.call(cmd_line, shell=True) == 0:
            ui.end_task('OK')
        else:
            ui.end_task('failed')

    for src_fn in glob(os.path.join(directory, '*.c')):
        build_binary(src_fn)
    for src_fn in glob(os.path.join(directory, '*.cpp')):
        build_binary(src_fn)
    for src_fn in glob(os.path.join(directory, '*.java')):
        build_binary(src_fn)
    for src_fn in glob(os.path.join(directory, '*.kt')):
        build_binary(src_fn)
    for src_fn in glob(os.path.join(directory, '*.py')):
        build_binary(src_fn)


def mycomp(a, b):
    a, tmp = os.path.split(a)
    atestnum, tmp = os.path.splitext(tmp)
    tmp, acasenum = os.path.split(a)
    b, tmp = os.path.split(b)
    btestnum, tmp = os.path.splitext(tmp)
    tmp, bcasenum = os.path.split(b)
    if int(acasenum) > int(bcasenum):
        return 1
    elif int(acasenum) < int(bcasenum):
        return -1
    else:
        if int(atestnum) > int(btestnum):
            return 1
        elif int(atestnum) < int(btestnum):
            return -1
        else:
            return 0


def input_files(name):
    tmp = sorted(glob('%s/build/tests/*/*.in' % name), key=functools.cmp_to_key(mycomp))
    return tmp


def sample_input_files(name):
    tmp = sorted(glob('%s/documents/sample-*.in' % name))
    return tmp


def output_files(name):
    tmp = sorted(glob('%s/build/tests/*/*.sol' % name), key=functools.cmp_to_key(mycomp))
    return tmp


def sample_output_files(name):
    tmp = sorted(glob('%s/documents/sample-*.sol' % name))
    return tmp


def special_judge(name):
    judges = [judge for judge in glob('%s/judge/*' % name)]
    if len(judges) > 0:
        judge_cmd = change_extension(judges[0], 'exe')
        return judge_cmd
    return None


def output_limit(name):
    output_limit_file_path = '%s/attic/output_limit' % name
    if os.path.exists(output_limit_file_path):
        return int(open(output_limit_file_path, 'r').read().strip())
    return None


def custom_cpp_timelimit_formula(name):
    tl_formula_file_path = '%s/attic/cpp_timelimit_formula' % name
    if os.path.exists(tl_formula_file_path):
        return open(tl_formula_file_path, 'r').read().strip()
    return None


def box_new(name):
    "Create a blank tree for a new problem."
    shutil.copytree('.box/skel/', '%s/' % name, symlinks=True)


def box_build_pdf(name):
    "Compile PDFs for all documents of problem `name`."
    ui.task_header(name, 'Compiling documents...')
    if not program_available('pdflatex'):
        ui.show_message('Error', 'Could not find `pdflatex`.', ERROR)
        hints.give_hint('pdflatex')
        return
    for tex_fn in glob('%s/documents/*.tex' % name):
        pdf_fn = change_extension(tex_fn, 'pdf')
        build_pdf(pdf_fn, tex_fn)


def box_build_input(name):
    "Build the input file for problem `name`."
    ui.task_header(name, 'Building generators...')
    build_binaries('%s/attic/' % name)
    ui.task_header(name, 'Generating input files...')

    clean.clean_build(name)
    if not os.path.exists('%s/build/' % name):
        os.mkdir('%s/build/' % name)
    if not os.path.exists('%s/build/tests/' % name):
        os.mkdir('%s/build/tests/' % name)
    ui.start_task('%s/attic/build-tests' % name)
    OK_msg, failed_msg, short = 'OK', 'failed', None
    my_env = os.environ.copy()
    my_env['ROOT'] = '%s' % name
    with NamedTemporaryFile('w') as error_file:
        error_file.write('test')
        try:
            res_proc = subprocess.check_call(['%s/attic/build-tests' % name],
                                             env=my_env, stderr=error_file)
            if res_proc == 0:
                ui.end_task(OK_msg, OK, short)
            else:
                ui.end_task(failed_msg, ERROR, short)
                print('\nError messages:\n---------------', file=sys.stderr)
                with open(error_file.name, 'r') as efile:
                    print(efile.read(), file=sys.stderr)
                print('---------------', file=sys.stderr)
        except:
            ui.end_task(failed_msg, ERROR, short)
            print('\nError messages:\n---------------', file=sys.stderr)
            with open(error_file.name, 'r') as efile:
                print(efile.read(), file=sys.stderr)
            print('---------------', file=sys.stderr)

    size = 0
    for i in input_files(name):
        size += os.path.getsize(i)
    print(' %d tests built (%s)' % (len(input_files(name)), sizeof_fmt(size)))


def sizeof_fmt(num, suffix='B'):
    for unit in ['', 'Ki', 'Mi', 'Gi', 'Ti', 'Pi', 'Ei', 'Zi']:
        if abs(num) < 1024.0:
            return "%3.1f%s%s" % (num, unit, suffix)
        num /= 1024.0
    return "%.1f%s%s" % (num, 'Yi', suffix)


def box_check_input(name):
    "Check the input file for problem `name`."
    global short_mode

    def run_checker(checker_fn):
        "Run a single checker program against the input file."
        ui.task_header(name, 'Validating input with %s...' %
                       os.path.basename(checker_fn))
        if not os.access(checker_fn, os.X_OK):
            ui.show_message('Warning', '%s is not an executable.' %
                            checker_fn, WARNING)
            return
        for input_fn in sample_input_files(name)+input_files(name):
            if not short_mode:
                ui.start_task(input_fn)
            with open(input_fn, 'r') as input_file:
                with open('/dev/null', 'w') as dev_null:
                    if subprocess.call(checker_fn,
                                       stdin=input_file,
                                       stdout=dev_null,
                                       stderr=dev_null) != 0:
                        ui.end_task(failed_msg, ERROR, short)
                    else:
                        ui.end_task(OK_msg, OK, short)

    if short_mode:
        OK_msg, failed_msg, short = '.', 'X', 'short'
    else:
        OK_msg, failed_msg, short = 'OK', 'failed', None
    ui.task_header(name, 'Building checkers...')
    build_binaries('%s/checkers/' % name)
    input_checkers = glob('%s/checkers/*.*exe' % name)
    if not input_checkers:
        ui.show_message('Warning', 'No input checkers available.', WARNING)
        return
    for checker_fn in input_checkers:
        if checker_fn.find("output") > 0:
            continue
        run_checker(checker_fn)
    if short:
        print()


def box_check_output(name):
    "Check the output file for problem `name`."
    global short_mode

    def run_checker(checker_fn):
        "Run a single checker program against the output file."
        ui.task_header(name, 'Validating output with %s...' %
                       os.path.basename(checker_fn))
        if not os.access(checker_fn, os.X_OK):
            ui.show_message('Warning', '%s is not an executable.' %
                            checker_fn, WARNING)
            return
        for output_fn in sample_output_files(name)+output_files(name):
            if not short_mode:
                ui.start_task(output_fn)
            with open(output_fn, 'r') as output_file:
                with open('/dev/null', 'w') as dev_null:
                    if subprocess.call(checker_fn,
                                       stdin=output_file,
                                       stdout=dev_null,
                                       stderr=dev_null) != 0:
                        ui.end_task(failed_msg, ERROR, short)
                    else:
                        ui.end_task(OK_msg, OK, short)

    if short_mode:
        OK_msg, failed_msg, short = '.', 'X', 'short'
    else:
        OK_msg, failed_msg, short = 'OK', 'failed', None
    ui.task_header(name, 'Building checkers...')
    build_binaries('%s/checkers/' % name)
    output_checkers = glob('%s/checkers/*.*exe' % name)
    if not output_checkers:
        ui.show_message('Warning', 'No output checkers available.', WARNING)
        return
    for checker_fn in output_checkers:
        if checker_fn.find("input") > 0:
            continue
        run_checker(checker_fn)
    if short:
        print()


def box_build_good_solutions(name):
    "Compile all good solutions for problem `name`."
    ui.task_header(name, 'Building solutions...')
    build_binaries('%s/solutions/good/' % name)
    build_binaries('%s/solutions/pass/' % name)
    if os.path.exists('%s/judge' % name):
        ui.show_message('Info', 'Needs special judge, building...', INFO)
        build_binaries('%s/judge/' % name)


def box_build_solutions(name):
    "Compile all solutions for problem `name`."
    ui.task_header(name, 'Building solutions...')
    build_binaries('%s/solutions/good/' % name)
    build_binaries('%s/solutions/pass/' % name)
    build_binaries('%s/solutions/slow/' % name)
    build_binaries('%s/solutions/wrong/' % name)
    if os.path.exists('%s/judge' % name):
        ui.show_message('Info', 'Needs special judge, building it...', INFO)
        build_binaries('%s/judge/' % name)


def reference_solution(name):
    good_solutions = [solution for solution, status in solutions(name)
                      if status == 'AC']
    if not good_solutions:
        return None
    for s in good_solutions:
        if s.find('reference') >= 0:
            return s
    return good_solutions[0]


def build_one_output(name, input_fn):
    output_fn = change_extension(input_fn, 'sol')
    delete_file(output_fn)
    result = run_solution(reference_solution(name), input_fn, output_fn)
    return result.status == 'OK'


def box_build_output(name):
    "Build output file for problem `name`."
    global short_mode
    if short_mode:
        OK_msg, failed_msg, short = '.', 'X', 'short'
    else:
        OK_msg, failed_msg, short = 'OK', 'failed', None

    if reference_solution(name) is None:
        ui.task_header(name, 'Generating problem output...')
        ui.show_message('Error', 'No good solutions available.', ERROR)
        hints.give_hint('output-good-solutions')
        return False
    else:
        ui.task_header(name, 'Generating problem output with %s...' %
                       os.path.basename(reference_solution(name)))
    for input_fn in input_files(name):
        output_fn = change_extension(input_fn, 'sol')
        if not short_mode:
            ui.start_task(output_fn)
        if build_one_output(name, input_fn):
            ui.end_task(OK_msg, OK, short)
        else:
            ui.end_task(failed_msg, ERROR, short)
    if short:
        print()
    return True


def box_report(name):
    def print_time_summary(lang, rtype, min_time_array, max_time_array, total_time_array):
        if rtype == 'primary':
            print('**** %s ****' % lang)
            if len(max_time_array) == 0:
                print('No solutions')
                return 0
            time_limit = timelimit(max_time_array, lang, name)
            print('Number of solutions: %d' % len(max_time_array))
            if time_limit > 4*min(max_time_array):
                print('Time limit calculated: %5.2fs (ATTENTION: significant differences in execution times!)' % time_limit)
            else:
                print('Time limit calculated: %5.2fs' % time_limit)
            print('Max. min. time: %5.2fs' % (max(min_time_array)))
            print('Min. max. time: %5.2fs' % (min(max_time_array)))
            print('Max. max. time: %5.2fs' % (max(max_time_array)))
            print('Min. tot. time: %5.2fs' % (min(total_time_array)))
            print('Max. tot. time: %5.2fs' % (max(total_time_array)))
            return len(max_time_array)
        else:
            time_limit = timelimit(max_time_array, lang, name)
            print('Time limit calculated using C/C++: %5.2fs' % time_limit)
            return 0

    context = {}
    exec(compile(open('%s/attic/problem.desc' % name, "rb").read(), '%s/attic/problem.desc' % name, 'exec'), context)
    print("\nSUMMARY - %s" % name)
    nsol = print_time_summary(
        C_LANG, 'primary', context['C'][0], context['C'][1], context['C'][2])
    if nsol == 0:
        print('For computing time limits you must provide at least one C/C++ correct solution')
    nsol = print_time_summary(
        JAVA_LANG, 'primary', context['Java'][0], context['Java'][1], context['Java'][2])
    if nsol == 0:
        print_time_summary(JAVA_LANG, 'secondary',
                           context['C'][0], context['C'][1], context['C'][2])
    nsol = print_time_summary(
        PYTHON_LANG, 'primary', context['Python'][0], context['Python'][1], context['Python'][2])
    if nsol == 0:
        print_time_summary(PYTHON_LANG, 'secondary',
                           context['C'][0], context['C'][1], context['C'][2])


def solutions(name, sol_groups=["good", "pass", "slow", "wrong"]):
    if "good" in sol_groups:
        for sol_fn in glob('%s/solutions/good/*.*exe' % name):
            yield sol_fn, 'AC'
    if "pass" in sol_groups:
        for sol_fn in glob('%s/solutions/pass/*.*exe' % name):
            yield sol_fn, 'AC'
    if "wrong" in sol_groups:
        for sol_fn in glob('%s/solutions/wrong/*.*exe' % name):
            yield sol_fn, 'WA'
    if "slow" in sol_groups:
        for sol_fn in glob('%s/solutions/slow/*.*exe' % name):
            yield sol_fn, 'TLE'


def box_check_solutions(name, sol_groups, do_time=False, do_sample=False):
    "Check all solutions for problem `name`."
    global short_mode

    judge_cmd = special_judge(name)
    if judge_cmd:
        ui.show_message('Info', 'using special judge: %s ...' %
                        judge_cmd, INFO)

    max_file_kbytes = output_limit(name)
    if max_file_kbytes:
        ui.show_message('Info', 'using non-standard max output size: %d kbytes ...' %
                        max_file_kbytes, INFO)
    else:
        max_file_kbytes = 1024

    def check_solution_language(fsolname):
        with open(fsolname, 'r') as fsol:
            try:
                first_line = fsol.readline()
                if first_line.find('#!') == 0:
                    if first_line.find('python') > 0:
                        t = PYTHON_LANG
                    else:
                        t = JAVA_LANG
            except:
                t = C_LANG
        return t

    def run_check(name, sol_group, ctime_limit=100, jtime_limit=100, ptime_limit=100):
        for sol_fn, expected in solutions(name, sol_group):
            cur_cref_solutions_time, cur_jref_solutions_time, cur_pref_solutions_time = [], [], []
            cur_cref_solutions_totaltime, cur_jref_solutions_totaltime, cur_pref_solutions_totaltime = [], [], []
            solution_language = check_solution_language(sol_fn)
            basename, extension = os.path.splitext(sol_fn)
            mem_limit = 1024
            if solution_language == JAVA_LANG:
                time_limit = jtime_limit
                mem_limit = 2048
            elif solution_language == PYTHON_LANG:
                time_limit = ptime_limit
            else:
                time_limit = ctime_limit

            if ctime_limit == 100 and jtime_limit == 100 and ptime_limit == 100:
                # no time limit defined yet
                ui.task_header(name, 'Testing %s [%s,%s] on REAL input...' % (
                    os.path.basename(sol_fn), sol_group, solution_language))
            else:
                ui.task_header(name, 'Testing %s [%s,%s] on REAL input (Time limit: %2.2f)...' % (
                    os.path.basename(sol_fn), sol_group, solution_language, time_limit))
            for input_fn in input_files(name):
                if not short_mode:
                    ui.start_task(input_fn)

                result = run_solution(sol_fn, input_fn, judge_cmd=judge_cmd,
                                      reference_fn=change_extension(
                                          input_fn, 'sol'),
                                      time_limit=time_limit, mem_limit=mem_limit, file_limit=max_file_kbytes)
                if result.status == 'RE':
                    ui.end_task(exec_msg or result.detail, ERROR, short)
                elif result.status == 'TLE':
                    color = [WARNING, OK][expected == result.status]
                    ui.end_task(timeout_msg, color, short)
                elif result.status == 'WA':
                    color = [ERROR, OK][expected == 'WA']
                    if do_time:
                        ui.end_task('%4.2fs ' %
                                    result.running_time, color, short)
                    else:
                        ui.end_task(wrong_msg, color, short)
                elif result.status == 'AC':
                    color = [ERROR, OK][expected == 'AC']
                    if do_time:
                        ui.end_task('%4.2fs ' %
                                    result.running_time, color, short)
                    else:
                        ui.end_task(OK_msg, color, short)
                else:
                    assert False
                if expected == 'AC':
                    if solution_language == JAVA_LANG:
                        cur_jref_solutions_time.append(result.running_time)
                    elif solution_language == PYTHON_LANG:
                        cur_pref_solutions_time.append(result.running_time)
                    else:  # lang is c/c++
                        cur_cref_solutions_time.append(result.running_time)

            if len(cur_cref_solutions_time) != 0:
                cref_solutions_maxtime.append(max(cur_cref_solutions_time))
                cref_solutions_mintime.append(min(cur_cref_solutions_time))
                cref_solutions_totaltime.append(sum(cur_cref_solutions_time))
                print(' Total time: %.2fs  (worst case: %.2fs)' % (sum(cur_cref_solutions_time), max(cur_cref_solutions_time)))
            if len(cur_jref_solutions_time) != 0:
                jref_solutions_maxtime.append(max(cur_jref_solutions_time))
                jref_solutions_mintime.append(min(cur_jref_solutions_time))
                jref_solutions_totaltime.append(sum(cur_jref_solutions_time))
                print(' Total time: %.2fs  (worst case: %.2fs)' % (sum(cur_jref_solutions_time), max(cur_jref_solutions_time)))
            if len(cur_pref_solutions_time) != 0:
                pref_solutions_maxtime.append(max(cur_pref_solutions_time))
                pref_solutions_mintime.append(min(cur_pref_solutions_time))
                pref_solutions_totaltime.append(sum(cur_pref_solutions_time))
                print(' Total time: %.2fs  (worst case: %.2fs)' % (sum(cur_pref_solutions_time), max(cur_pref_solutions_time)))

        return

    if short_mode:
        OK_msg, timeout_msg, wrong_msg, exec_msg, short = '.', 'T', 'W', 'X', 'short'
    else:
        OK_msg, timeout_msg, wrong_msg, exec_msg, short = 'OK', 'timeout', 'wrong', None, None
    # problem specs, to be read or calculated
    # using this only for the sample
    jtime_limit = 60
    ctime_limit = 60
    ptime_limit = 60

    if do_sample:
        for sol_fn, expected in solutions(name, "good"):
            solution_language = check_solution_language(sol_fn)
            ui.task_header(name, 'Testing %s [%s,%s] on sample input...' % (
                os.path.basename(sol_fn), "good", solution_language))
            for input_fn in sample_input_files(name):
                if not short_mode:
                    ui.start_task(input_fn)
                basename, extension = os.path.splitext(sol_fn)
                mem_limit = 1024
                if solution_language == JAVA_LANG:
                    time_limit = jtime_limit
                    mem_limit = 2048
                elif solution_language == PYTHON_LANG:
                    time_limit = ptime_limit
                else:
                    time_limit = ctime_limit
                result = run_solution(sol_fn, input_fn, judge_cmd=judge_cmd,
                                      reference_fn=change_extension(
                                          input_fn, 'sol'),
                                      time_limit=time_limit, mem_limit=mem_limit, file_limit=max_file_kbytes)
                if result.status == 'RE':
                    ui.end_task(exec_msg or result.detail, ERROR, short)
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

    cref_solutions_maxtime, cref_solutions_mintime, cref_solutions_totaltime = [], [], []
    jref_solutions_maxtime, jref_solutions_mintime, jref_solutions_totaltime = [], [], []
    pref_solutions_maxtime, pref_solutions_mintime, pref_solutions_totaltime = [], [], []
    if "good" in sol_groups:
        # will calculate time limits
        run_check(name, 'good')
        if short_mode:
            print()
        if len(cref_solutions_maxtime) == 0:  # no C solutions
            print("Must have at least one C solution to calculate time limits!", file=sys.stderr)
        ctime_limit = timelimit(cref_solutions_maxtime, C_LANG, name)
        if len(jref_solutions_maxtime) == 0:  # no Java solutions
            jtime_limit = timelimit(cref_solutions_maxtime, JAVA_LANG, name)
        else:
            jtime_limit = timelimit(jref_solutions_maxtime, JAVA_LANG, name)
        if len(pref_solutions_maxtime) == 0:  # no Python solutions
            ptime_limit = timelimit(cref_solutions_maxtime, PYTHON_LANG, name)
        else:
            ptime_limit = timelimit(pref_solutions_maxtime, PYTHON_LANG, name)
        # write time limits in file attic/problem.desc
        cctf = custom_cpp_timelimit_formula(name)
        if cctf is not None:
            ui.show_message('Info', 'Custom C++ time limit calculated with ' + cctf, INFO)

        try:
            os.mkdir('%s/attic' % name)
        except:
            pass
        with open('%s/attic/problem.desc' % name, 'w') as desc_fn:
            desc_fn.write('time_limit_c=%2.2f\ntime_limit_java=%2.2f\ntime_limit_python=%2.2f\n' % (
                ctime_limit, jtime_limit, ptime_limit))
            desc_fn.write("C=(%s,%s,%s)\n" % (str(cref_solutions_mintime), str(
                cref_solutions_maxtime), str(cref_solutions_totaltime)))
            desc_fn.write("Java=(%s,%s,%s)\n" % (str(jref_solutions_mintime), str(
                jref_solutions_maxtime), str(jref_solutions_totaltime)))
            desc_fn.write("Python=(%s,%s,%s)\n" % (str(pref_solutions_mintime), str(
                pref_solutions_maxtime), str(pref_solutions_totaltime)))
        box_report(name)

    # read time_limits from file attic/problem.desc if "good" not in sol_groups
    if not os.path.isfile('%s/attic/problem.desc' % name):
        ui.show_message(
            'Error', 'Please run check-solutions-good before to calculate time limits.', ERROR)
        return
    context = {}
    exec(compile(open(os.path.join(name, 'attic', 'problem.desc'), "rb").read(), os.path.join(name, 'attic', 'problem.desc'), 'exec'), context)
    ct, jt, pt = context['time_limit_c'], context['time_limit_java'], context['time_limit_python']
    if ct == 60 and jt == 60 and pt == 60:
        ui.show_message(
            'Error', 'Please run check-solutions-good before to calculate time limits.', ERROR)
        return

    if "pass" in sol_groups:
        run_check(name, 'pass', ct, jt, pt)
        if short_mode:
            print()
    if "wrong" in sol_groups:
        run_check(name, 'wrong', ct, jt, pt)
        if short_mode:
            print()
    if "slow" in sol_groups:
        run_check(name, 'slow', ct, jt, pt)
        if short_mode:
            print()


def timelimit(times, lang, name):
    try:
        maxtime = max(times)
        mintime = min(times)
    except:
        maxtime = 0
        mintime = 0
        return 0
    cpp_tl_formula = custom_cpp_timelimit_formula(name)
    if lang == JAVA_LANG:
        tl = max(2*mintime, 1.5*maxtime)
    elif lang == C_LANG and cpp_tl_formula is not None:
        _locals = locals()
        cpp_tl_formula = 'tl = ' + cpp_tl_formula
        exec(cpp_tl_formula, globals(), _locals)
        tl = _locals['tl']
    else:
        tl = max(3*mintime, 1.5*maxtime)
    # in 100ms
    tl = math.ceil(tl*10)/10.0
    return tl


def parse_arguments():
    global short_mode
    short_mode = True
    optlist, args = getopt.gnu_getopt(sys.argv[1:], 'l:v')
    for flag, arg in optlist:
        if flag == '-l':
            sys.stdout.dumb_taps.append(open(arg, 'w'))
        if flag == '-v':
            short_mode = False
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
    targets = sorted(targets)
    return cmd, targets


def _main():
    global short_mode
    cmd, targets = parse_arguments()

    if cmd == 'version':
        with open('.box/VERSION') as version_file:
            print('box %s' % (version_file.read(), ))
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
                box_check_solutions(
                    target, ["good", "pass", "slow", "wrong"], do_sample=True)
            elif cmd == 'check-solutions-good':
                box_check_solutions(target, ["good"], do_sample=True)
            elif cmd == 'check-solutions-pass':
                box_check_solutions(target, ["pass"], do_sample=False)
            elif cmd == 'check-solutions-wrong':
                box_check_solutions(target, ["wrong"], do_sample=False)
            elif cmd == 'check-solutions-slow':
                box_check_solutions(target, ["slow"], do_sample=False)
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
                box_check_solutions(
                    target, ["good", "pass", "slow", "wrong"], do_sample=True)
            elif cmd == 'check-good':
                box_build_pdf(target)
                box_build_input(target)
                box_check_input(target)
                box_build_solutions(target)
                box_build_output(target)
                box_check_output(target)
                box_check_solutions(target, ["good"], do_sample=True)
            elif cmd == 'clean':
                clean.box_clean(target)
            elif cmd == 'report':
                box_report(target)
            elif cmd == 'time':
                box_check_solutions(
                    target, ["good", "pass", "slow"], do_time=True)
            elif cmd == 'time-good':
                box_check_solutions(target, ["good"], do_time=True)
            elif cmd == 'time-pass':
                box_check_solutions(target, ["pass"], do_time=True)
            elif cmd == 'time-slow':
                box_check_solutions(target, ["slow"], do_time=True)
            elif cmd == 'time-wrong':
                box_check_solutions(target, ["wrong"], do_time=True)
            elif cmd in ('commit', 'push', 'pull') and program_available('sl'):
                os.system('sl')
                sys.exit(1)
            else:
                ui.usage()
            hints.show_hints()


if __name__ == '__main__':
    _main()
