#!/usr/bin/env python3
# encoding: utf-8

import os, sys
import os.path
import subprocess
import shutil
from tempfile import NamedTemporaryFile
from util import change_extension
import ui

def build_pdf(output_fn, *tex_fns):
    "Compile the `tex_fns` files to a PDF named `output_fn`."
    if os.path.exists('.build'): 
        shutil.rmtree('.build') # remove old content
    os.mkdir('.build')
        
    tex_input = NamedTemporaryFile(mode = 'w', suffix = '.tex', delete = False)
    with open('.box/defs.tex', 'r') as tex_defs:
        tex_input.write(tex_defs.read())
    print('\\newcommand{\\CWD}{.}', file=tex_input)
    print('\\begin{document}', file=tex_input)
    for tex_fn in tex_fns:
        print('\\renewcommand{\\CWD}{%s}' % \
                os.path.dirname(tex_fn), file=tex_input)
        with open(tex_fn, 'r') as tex_file:
            tex_input.write(tex_file.read())
    print('\\end{document}', file=tex_input)
    tex_input.close()

    log_fn = change_extension(output_fn, 'log')
    ui.start_task(output_fn)
    if subprocess.call(['pdflatex',
                        '-halt-on-error',
                        '-file-line-error',
                        '-output-directory',
                        '.build',
                        '-jobname=output',
                        '-interaction=nonstopmode',
                        tex_input.name],
                       stdout = NamedTemporaryFile(),
                       stderr = NamedTemporaryFile()) == 0:
        shutil.move('.build/output.pdf', output_fn)
        ui.end_task('OK')
    else:
        ui.end_task('failed')
        shutil.move('.build/output.log', log_fn)
