#!/usr/bin/env python2.7
# encoding: utf-8

import sys
import textwrap

RESET = '\x1b[0m'
BOLD = '\x1b[1m'
UNDERLINE = '\x1b[4m'
RED = '\x1b[31m'
GREEN = '\x1b[32m'
YELLOW = '\x1b[33m'
BLUE = '\x1b[34m'

def colorize(text, color):
    u"Add ANSI coloring to `text`."
    return color + text + RESET

def decolorize(text):
    u"Strip ANSI coloring from `text`."
    return re.sub(r'\033\[[0-9]+m', '', text)

INFO = BOLD
OK = BOLD + GREEN
WARNING = BOLD + YELLOW
ERROR = BOLD + RED

def show_message(label, message, color = INFO):
    indent_size = len(label) + 3
    lines = textwrap.wrap(message,
                          width = 80,
                          initial_indent = ' %s: ' % label,
                          subsequent_indent = ' ' * indent_size)
    lines[0] = lines[0][indent_size:]
    sys.stdout.write('\n %s ' % colorize(label + ':', color + UNDERLINE))
    sys.stdout.write('\n'.join(lines))
    print

def task_header(name, message):
    print
    sys.stdout.write(colorize('[%s] %s' % (name, message), INFO))
    print

def start_task(message):
    # Remove the leading directory (it's already mentioned in the header)
    message = message.split('/', 1)[1]
    # Ensure the message has an even length.
    if len(message) % 2 == 0:
        message = ' * %s   ' % message
    else:
        message = ' * %s  ' % message

    if len(message) < 64:
        message += '. ' * ((64 - len(message)) // 2)
    elif len(message) > 64:
        # clip message
        message = message[:60] + '... '

    sys.stdout.write(message)

def end_task(status, color = None, short=None):
    if color is None:
        if status == 'OK':
            color = OK
        elif status == 'failed':
            color = ERROR
        else:
            color = INFO
    
    sys.stdout.write(colorize(status, color))
    if short==None: print


def usage():
    u"Usage instructions for this script."
    basename = 'box'

    print
    sys.stdout.write('Usage:')
    print '\t%s new <name>' % basename
    print '\t%s build <name>' % basename
    print '\t%s [-v] check <name>' % basename
    print '\t%s clean <name>' % basename
    print '\t%s time <name>' % basename
    print
    print '\t%s build-pdf <name>' % basename
    print '\t%s build-input <name>' % basename
    print '\t%s build-output <name>' % basename
    print '\t%s build-solutions <name>' % basename
    print '\t%s [-v] check-input <name>' % basename
    print '\t%s [-v] check-solutions <name>' % basename
    print
    print '\t%s build-packages' % basename
    print '\t%s build-package-warmup' % basename
    print '\t%s build-package-contest' % basename
    print
    print '(the flag -v forces verbose output)' 
    print

    sys.exit(1)

class Log (object):
    u"Logging class; allows writing to more than one file."
    def __init__(self):
        self.smart_taps = [sys.stderr]
        self.dumb_taps = []

    def write(self, data):
        u"Write data to all taps, removing ANSI codes as needed."
        for tap in self.smart_taps:
            tap.write(data)
        for tap in self.dumb_taps:
            tap.write(decolorize(data))
sys.stdout = Log()

