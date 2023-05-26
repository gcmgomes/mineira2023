#!/usr/bin/env python3

import sys,re,os,getopt

BASE = "../../contest"
INPUT_FILE="warmup_problem_set.tex"
TEMPLATE='''basename="{}"
codename="{}"
fullname="{}"
---
'''
HEADER='''title:{}
'''

def usage():
    print('usage:\n   {} [-h] [-m modality] output-dir'.format(sys.argv[0]))

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h", ["html"])
    except getopt.GetoptError as err:
        print(err)  # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    html = False
    for o, a in opts:
        if o in ("-h", "--html"):
            html = True
        else:
            assert False, "unhandled option"

    base = os.path.dirname(BASE)
    pat_problem=re.compile("\\\\inputProblem\{(?P<codename>[^}]*)\}\{(?P<title>[^}]*)\}")
    with open(os.path.join(base,"contest",INPUT_FILE)) as f:
        tex_src=f.read()

    with open('problems.desc','w') as fout:
        fout.write('# coding=utf-8\n#\n')
    m = re.findall(pat_problem,tex_src)
    problem_letter = 'A'
    from string import ascii_letters
    for matched,problem_letter in zip(m,ascii_letters.upper()):
        codename = matched[0]
        title = matched[1]
        with open('problems.desc','a') as fout:
            fout.write(TEMPLATE.format(problem_letter,codename,title))
    #ifilename = os.path.join(base,problems,problem_code,documents,statement)
    #ofilename = os.path.join(base,outputdir,"{}f{}{}{}_{}.html".format(year,phase,modality,level,problem_code))


if __name__ == "__main__":
    main()
