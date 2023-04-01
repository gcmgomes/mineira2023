**********
# Box Manual
**********

Box is a set of python scripts to help the process of problem
setting. It was developed by Fabio Dias Moreira (fabio@dias.moreira.nom.br).

## Instructions
------------

The script (box) is located in the `bin` folder at the root of the repository. You can copy it to your local `bin` folder, or include the path to the repository `bin` in your execution `PATH`.

In the `problems` folder, type `box` to see the options available. The options available are:

    box new <name>
       creates the directory structure for a new problem
    box build <name>
       builds all files
    box check <name>
       builds and checks all files, executes all solutions
    box check-good <name>
       builds and checks all files, executes good solutions
    box clean <name>
       removes all built files
    box time <name>
	    times the execution for all solutions
    box time-good <name>
        times the execution for good solutions
    box report <name>
        prints a report of execution times
    box build-pdf <name>
        builds statement and solution documents
    box build-input <name>
	    builds input files
    box build-output <name>
        builds output files
    box build-solutions <name>
	    compile all solutions
    box check-input <name>
        checks input files
    box check-input <name>
        checks output files
    box check-solutions <name>
        checks all solutions
    box build-packages
        builds contest and warmup packages
    box build-package-contest
    	builds contest package
    box build-package-warmup
        builds warmup package

The `<name>` argument is optional. If you are inside a problem folder, the command will act only on that problem. if you are in the top folder (that is, the folder named 'problems'), the command will act on every problem in the folder 'problems', sequentially.

You can also use the optional argument "-v" (like in "box -v check") to produce a more verbose output from the commands.

## Directory Structure
-------------------
A problem is composed of five folders:

```
documents/
solutions/
build/
checkers/
attic/
````

Optionally a `judge/` folder might also be added if the problem requires a special judge.

The `documents ` folder contains:
- `sample-<n>.in` - sample input, for n=1,2...     
- `sample-<n>.sol` - sample output, for n=1,2...
- `solution.tex` - tex for comments on the solution, to be distributed after the contest
- `statement.tex`  - tex for the problem statement

The `solutions` folder contains three folders: `good ` (for solutions
considered valid),  `slow ` (for solutions considered too slow) and
 `wrong ` (for wrong solutions). Use the name of the author as the name
of the solution (for example ranido.c, or ranido-queue.c). Box uses
the result returned by  `main ` to check the status of the execution,
so remember to  `return 0 ` from your `main`.

The  `checkers ` folder stores the input/output checkers.  Several
checkers are available by default, for checking extra spaces, extra
lines, etc, but a checker specific for the problem must be provided
(for example if the statement specifies that the input graph does not
contain a cycle that must be checked).  All checkers are run for all
input files and for the `sample-<n>.in` files in the documents folder.

The `build ` folder is used by box to store generated files. In
particular, it is where the generated input files are stored.
Input files are generated in the folder `build/tests`. It is removed
by a `check clean` command.

The  `attic` folder contains the executables for generating the input
files, and input files (pre-built). You can use this folder as you
wish, but by default a standard test generator is included that
parses a textfile `testplan.txt` to produce the files. It is a
good way to document the tests included. More instructions inside
the `attic` folder.

### More details on the available options
-------

* box new <name>
  Creates a new problem, preparing a template for the problem.

* box build <name>
  Builds the statement, compiles the solutions, generates the test
  cases (input and output).  Does not check the input/output, does not
  run all solutions.

* box check <name>
  Builds the statement, compiles the solutions, generates the test cases
  (input and output), check the sample input, check the real input, run
  all solutions and verifies they run as expected.

* box check-good <name>
  Same as box check, but runs only good solutions

* box clean <name>
  Removes everything that should be removed (test cases, executables,
  pdfs).

* box time <name>
  Runs all solutions, reporting the execution time.

* box time-good <name>
  Runs all 'good' solutions, reporting the execution time.

* box report <name>
  Prints a report of execution times (needs to execute 'box check' before).

* box build-pdf <name>
  Builds the statement and solution pdfs.

* box build-input <name>
  Builds the input test cases.

* box build-output <name>
  Builds the output test cases.

* box build-solutions <name>
  Compiles all solutions (good, slow, wrong)

* box check-input <name>
  Verifies the input (final input file and sample.in).

* box check-solutions <name>
  Checks that all solutions run as expected: good solutions must agree
  on the output, slow solutions should timeout, wrong solutions should
  give wrong answers. The timeout at the moment is fixed as 20s for
  all problems.
