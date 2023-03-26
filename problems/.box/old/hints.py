#!/usr/bin/env python2.7
# encoding: utf-8

import ui

HINTS = {
    'testdata-not-executable':
    'Files in the testdata/ directory that do not end in `.in` are '
    'assumed to be generator scripts; either set their executable bit or '
    'rename them, if they are plain-text inputs.',

    'output-good-solutions':
    'Create at least one solution in the solutions/good/ directory to '
    'generate the solution file for this problem.',

    'solution-SIGABRT':
    'Solutions failing with SIGABRT are usually asserting input '
    'restrictions; please remove these and move these checks to a script in '
    'testdata/checkers/ instead.',

    'solution-SIGSEGV':
    'Solutions failing with SIGSEGV are doing invalid memory accesses; '
    'check if they are assuming input is smaller than it actually is.',

    'solution-failed':
    'Run failing solutions manually to discover the reason for the crash. '
    'Note that the solutions/wrong/ folder is intended for solutions that '
    'run correctly but give wrong answers; either debug failing solutions '
    'or delete them from the problem package.',

    'solution-wrong-sample':
    'All solutions should correctly solve the sample input; either debug '
    'failing solutions or delete them from the problem package.',

    'pdflatex':
    'The pdflatex application is needed to generate the problem statements. '
    'Make sure it can be found in your $PATH (e.g. `which pdflatex`).',
}
useful_hints = []

def give_hint(hint):
    u"Give `hint` to the user after execution is complete."
    assert hint in HINTS
    if hint not in useful_hints:
        useful_hints.append(hint)

def show_hints():
    for hint in useful_hints:
        ui.show_message('Hint', HINTS[hint])
