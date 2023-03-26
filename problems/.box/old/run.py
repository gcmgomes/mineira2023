#!/usr/bin/env python2.7
# encoding: utf-8

import os
import subprocess
import resource
import signal
from tempfile import NamedTemporaryFile

import hints
from util import delete_file

class Result (object):
    def __init__(self, running_time = None):
        if running_time is not None:
            self.running_time = running_time

def run_solution(sol_fn, input_fn,
                 output_fn = None,
                 reference_fn = None, 
                 time_limit = 20,
                 mem_limit = 1024,
                 file_limit = 1024):
    if reference_fn is None:
        if output_fn is None:
            output_fn = '/dev/null'
    else:
        assert output_fn is None
        output_file = NamedTemporaryFile(delete = False)
        output_fn = output_file.name

    mem_limit=mem_limit*1024*1024
    file_limit=file_limit*1024
    pid = os.fork()
    if pid == 0:
        resource.setrlimit(resource.RLIMIT_CPU, (time_limit+1, time_limit + 1))
        resource.setrlimit(resource.RLIMIT_DATA, (mem_limit, mem_limit))
        resource.setrlimit(resource.RLIMIT_FSIZE, (file_limit, file_limit))
        with open(input_fn, 'r') as in_file:
            os.dup2(in_file.fileno(), 0)
        with open(output_fn, 'w') as out_file:
            os.dup2(out_file.fileno(), 1)
        with open('/dev/null', 'w') as err_file:
            os.dup2(err_file.fileno(), 2)
        os.execl(sol_fn, sol_fn)
    (pid, status, rusage) = os.wait4(pid, 0)
    result = Result(running_time = rusage.ru_utime)

    # ranido changed
    #if os.WIFSIGNALED(status) and os.WTERMSIG(status) == signal.SIGXCPU:
    if result.running_time > time_limit:
        result.status = 'TLE'
    elif os.WEXITSTATUS(status) != 0:
        result.status = 'RE'
        result.detail = 'failed'
        hints.give_hint('solution-failed')
        if os.WIFSIGNALED(status) and os.WTERMSIG(status) == signal.SIGSEGV:
            hints.give_hint('solution-SIGSEGV')
            result.detail = 'SIGSEGV'
        elif os.WIFSIGNALED(status) and os.WTERMSIG(status) == signal.SIGABRT:
            hints.give_hint('solution-SIGABRT')
            result.detail = 'SIGABRT'
    elif reference_fn:
        if subprocess.call(['diff', reference_fn, output_fn],
                           stdout = NamedTemporaryFile(),
                           stderr = NamedTemporaryFile()) == 0:
            result.status = 'AC'
        else:
            result.status = 'WA'
    else:
        result.status = 'OK'

    if reference_fn:
        delete_file(output_fn)
    return result
