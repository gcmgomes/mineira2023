#!/usr/bin/python2.7
# coding=utf-8

# generate boca packages
# ICPC LA 2016,2017
# r. anido
import sys,os,shutil,subprocess,string
from glob import glob
from math import fabs

# calculate amount of runs of a solution given a desired time limit/
# to approximate non-integer time limits on BOCA
# time must be in seconds
def calculate_amount_of_runs(time):
    # max repetition time, in seconds
    max_rep_time = 7
    if time >= max_rep_time:
        return 1
    # max 20% effective
    max_error = 0.2
    def rounding_error(time):
        return fabs(round(time) - time)
    def calculate_error_percentage(time, runs):
        return rounding_error(time*runs)/(time*runs)
    amount_of_runs = 2
    error = calculate_error_percentage(time, amount_of_runs)
    total_test_time = max(1.0, round(time*amount_of_runs))
    # up to 10 times
    for i in range(3, 11):
        new_error = calculate_error_percentage(time, i)
        new_total_test_time = max(1.0, round(time*i))
        if new_error >= error:
            continue
        # if error is already below the maximum,
        # don't bother increasing total test time
        if error > max_error or new_total_test_time == total_test_time:
            amount_of_runs, error = i, new_error
            total_test_time = new_total_test_time
    return amount_of_runs

PROBLEMDIR=os.path.join('..','..','problems')
# relative to warmup/contest directories

time_limits=[]
run_amounts=[]

def mycomp(a,b):
    a,tmp=os.path.split(a)
    atestnum,tmp = os.path.splitext(tmp)
    tmp,acasenum = os.path.split(a)
    b,tmp=os.path.split(b)
    btestnum,tmp = os.path.splitext(tmp)
    tmp,bcasenum = os.path.split(b)
    if int(acasenum)>int(bcasenum):
        return 1
    elif int(acasenum)<int(bcasenum):
        return -1
    else:
        if int(atestnum)>int(btestnum):
            return 1
        elif int(atestnum)<int(btestnum):
            return -1
        else:
            return 0

def input_files(codename):
    from_dir=os.path.join(PROBLEMDIR,codename,'build','tests','*','*.in')
    tmp=glob(from_dir)
    tmp.sort(cmp=mycomp)
    return tmp

def output_files(codename):
    from_dir=os.path.join(PROBLEMDIR,codename,'build','tests','*','*.sol')
    tmp=glob(from_dir)
    tmp.sort(cmp=mycomp)
    return tmp

def build_problem(problem,fdesc):
    # each problem described in two lines
    # default limits
    mem_limit=1024
    p=''
    while True:
        x=fdesc.readline()
        if x=='' or x[0]=='-':
            break
        p+=x
    exec(p)

    # get maximum file size
    file_limit=get_file_limit(codename)

    # read the time limits from the problem directory,
    execfile(os.path.join(PROBLEMDIR,codename,'attic','problem.desc'))
    #with open(os.path.join('..','..',PROBLEMDIR,codename,'attic','problem.desc')) as f:
    #    for l in f.readlines():
    #        print l

    print >> sys.stderr, "%s (%s)" % (fullname,codename)

    # create directory
    try:
        shutil.rmtree(os.path.join('problems',problem))
    except:
        pass
    shutil.copytree(os.path.join('..','problem_template'),os.path.join('problems',problem))
    # git may have not copied the empty directories input, output, tests
    try:
        os.mkdir(os.path.join('problems',problem,'input'))
    except:
        pass
    try:
        os.mkdir(os.path.join('problems',problem,'output'))
    except:
        pass
    try:
        os.mkdir(os.path.join('problems',problem,'tests'))
    except:
        pass

    # description
    with open(os.path.join('problems',problem,'description','problem.info'),'w') as f:
        f.write('basename=%s\nfullname="%s"\ndescfile=%s.pdf\n' % (basename,fullname,problem))
    shutil.copy(os.path.join('pdf','%s.pdf' % problem),os.path.join('problems',problem,'description'))

    # input/output
    k=1
    for input_fn in input_files(codename):
        #print "copying input",input_fn,'as',k
        shutil.copy(input_fn,os.path.join('problems',problem,'input','%s_%04d'% (problem,k)))
        k+=1
    if k==1:
        print >> sys.stderr, 'cannot find input files'
        sys.exit(-1)
    else:
        print >> sys.stderr, '  copied %d input files' % (k-1)

    k=1
    for output_fn in output_files(codename):
        #print "copying output",output_fn,'as',k
        shutil.copy(output_fn,os.path.join('problems',problem,'output','%s_%04d'% (problem,k)))
        k+=1
    if k==1:
        print >> sys.stderr, 'cannot find output files'
        sys.exit(-1)
    else:
        print >> sys.stderr, '  copied %d output files' % (k-1)

    t_limit_c=time_limit_c
    t_limit_java=time_limit_java
    t_limit_python=time_limit_python
    # time limits
    if t_limit_java==0:
         print >> sys.stderr, '  no java solution',
         t_limit_java=2*t_limit_c
         print >> sys.stderr, ', using c time limit x 2'
    if t_limit_python==0:
        print >> sys.stderr, '  no python solution',
        t_limit_python=2*t_limit_c
        print >> sys.stderr, ', using c time limit x 2'

    # amount of executions
    runs_c = calculate_amount_of_runs(time_limit_c)
    runs_java = calculate_amount_of_runs(time_limit_java)
    runs_python = calculate_amount_of_runs(time_limit_python)
    t_limit_c = max(1.0, round(runs_c*time_limit_c))
    t_limit_java = max(1.0, round(runs_java*time_limit_java))
    t_limit_python = max(1.0, round(runs_python*time_limit_python))

    with open(os.path.join('..','templates','limit'),'r') as f:
        temp=f.readlines()
    tmp=''
    for l in temp:
        tmp+=l
    with open(os.path.join('problems',problem,'limits','c'),'w') as f:
        f.write(tmp % (t_limit_c,runs_c,mem_limit,file_limit))
    with open(os.path.join('problems',problem,'limits','cpp'),'w') as f:
        f.write(tmp % (t_limit_c,runs_c,mem_limit,file_limit))
    with open(os.path.join('problems',problem,'limits','java'),'w') as f:
        f.write(tmp % (t_limit_java,runs_java,mem_limit,file_limit))
    with open(os.path.join('problems',problem,'limits','kt'),'w') as f:
        f.write(tmp % (t_limit_java,runs_java,mem_limit,file_limit))
    with open(os.path.join('problems',problem,'limits','py3'),'w') as f:
        f.write(tmp % (t_limit_python,runs_python,mem_limit,file_limit))

    time_limits.append([problem,t_limit_c,t_limit_java,t_limit_python])
    run_amounts.append([problem,runs_c,runs_java,runs_python])

    # special judge
    judge = get_special_judge(codename)
    if judge:
        compare_path = os.path.join('problems',problem,'compare')
        languages = ['c','cpp','java','kt','py3']
        for l in languages:
            if os.path.isfile(os.path.join(compare_path, l)):
                os.unlink(os.path.join(compare_path, l))
            else:
                print >> sys.stderr,'no compare script found for',l

        if len(os.listdir(compare_path)):
            print >> sys.stderr,'Build failed, unexpected compare script found'
            sys.exit(-1)

        for l in languages:
            os.system('cp %s %s' % (os.path.join(judge), os.path.join(compare_path,l)))

def get_special_judge(name):
    judges = [judge for judge in glob(os.path.join(PROBLEMDIR,name,'judge','*'))]
    for judge in judges:
        base,ext = os.path.splitext(judge)
        if ext==".exe":
            print >> sys.stderr,'found special judge',judge
            return judge
    return None

def get_file_limit(name):
    output_limit_file_path = os.path.join(PROBLEMDIR,name,'attic','output_limit')
    file_limit=1024
    if os.path.exists(output_limit_file_path):
        file_limit = int(open(output_limit_file_path, 'r').read().strip())
        print >> sys.stderr, '  using non-standard file limit %d kbytes' % (file_limit)
    return file_limit

def build_package(problem):
    os.chdir(os.path.join('problems',problem))
    packages_dir=os.path.join('..','..','packages')
    try:
        os.remove(os.path.join(packages_dir,'%s.zip' % problem))
    except:
        pass
    if not os.path.exists(packages_dir):
        os.makedirs(packages_dir)
    try:
        if subprocess.call(['zip',
                        '-rq',
                        '%s.zip' % problem,
                        '.']) ==0:
            shutil.move('%s.zip' % problem, os.path.join('..','..','packages'))
        else:
            print >>sys.stderr,'Build failed'
            sys.exit(-1)
    except:
        print >>sys.stderr,'\nBuild package failed...\nIs zip installed?'
        sys.exit(-1)
    os.chdir(os.path.join('..','..'))

print 'cleaning everything'
os.system("./clean.sh")

print >> sys.stderr, 'building PDFs'
#print 'split pdf'
os.system("cd pdf; ./split.sh")

print >> sys.stderr, 'generating problem descriptions'
os.system("./gen_problem_descr.py")

num_problems=0
with open("problems.desc") as f:
    # find number of problems in package
    while True:
        x=f.readline()
        if x=='':
            break
        if x.find('basename')==0:
            num_problems+=1

curdir = os.getcwd()

os.chdir(PROBLEMDIR)
os.chdir(curdir)

problems=string.ascii_uppercase
fdescr = open("problems.desc")
for p in range(num_problems):
    problem=problems[p]
    print '\nbuilding problem ', problem
    build_problem(problem,fdescr)
    build_package(problem)
fdescr.close()

print "\n----------------------------"
print "      Time limits and number of executions for Boca"
print "----------------------------"
print "Problem  C/C++              Java/Kotlin              Python"
for j in range(len(time_limits)):
    t = time_limits[j]
    ra = run_amounts[j]
    print "%4s     %2d (%2s times)     %2d (%2s times)     %2d (%2s times)" % (t[0], t[1], ra[1], t[2], ra[2], t[3], ra[3])
print "----------------------------\n"

ftime = open(os.path.join('packages','times_boca.txt'),'w')
print >> ftime, "\n----------------------------"
print >> ftime, "      Time limits and number of executions for Boca"
print >> ftime, "----------------------------"
print >> ftime, "Problem  C/C++              Java/Kotlin              Python"
for j in range(len(time_limits)):
    t = time_limits[j]
    ra = run_amounts[j]
    print  >> ftime, "%4s     %2d (%2s times)     %2d (%2s times)     %2d (%2s times)" % (t[0], t[1], ra[1], t[2], ra[2], t[3], ra[3])
print  >> ftime, "----------------------------\n"
ftime.close()

for j in range(len(time_limits)):
    t = time_limits[j]
    ra = run_amounts[j]
    for i in range(1,len(t)):
        t[i] = float(t[i])/float(ra[i])

print "\n"
print "\n----------------------------"
print "      Time limits for teams"
print "----------------------------"
print "Problem  C/C++  Java/Kotlin  Python"
for t in time_limits:
    print "%4s     %2.1f     %2.1f     %2.1f" % (t[0], t[1], t[2], t[3])
print "----------------------------\n"

ftime = open(os.path.join('packages','times_teams.txt'),'w')
print >> ftime, "\n----------------------------"
print >> ftime, "      Time limits for teams"
print >> ftime, "----------------------------"
print >> ftime, "Problem  C/C++  Java/Kotlin  Python"
for t in time_limits:
    print  >> ftime, "%4s     %2.1f     %2.1f     %2.1f" % (t[0], t[1], t[2], t[3])
print  >> ftime, "----------------------------\n"

sys.exit(0)
