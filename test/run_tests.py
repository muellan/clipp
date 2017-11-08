#!/usr/bin/python

###############################################################################
#
# builds and runs tests, resolves include dependencies
#
# see help for usage:
#    ./run_tests.py --help
#
# (c) 2013-2017 Andre Mueller
#
###############################################################################

import re
import os
import shutil
import ntpath
from os import path
from os import system
from sys import stdout
from sys import argv
from sets import Set

#default settings
builddir   = "../build_test"
incpaths   = ["", "../include/", "../src/"]
macros     = ["NO_DEBUG", "NDEBUG"]
compiler   = "g++"
compileopt = "-std=c++14 -O3 -Wall -Wextra -Wpedantic -Wno-unknown-pragmas"
tuext      = "cpp"
separator  = "-----------------------------------------------------------------"

# [ (needs compilation, dependency regex) ]
deprxp = [re.compile('^\s*#pragma\s+test\s+needs\(\s*"(.+\..+)"\s*\)\s*$'),
          re.compile('^\s*#include\s+\<(.+\..+)\>\s*$'),
          re.compile('^\s*#include\s+"(.+\..+)"\s*$')]

testrxp = re.compile('(.+)\.' + tuext)

def dependencies(source, searchpaths = [], sofar = Set()):
    """ return set of dependencies for a C++ source file
        the following dependency definitions are recocnized:
          - #include "path/to/file.ext"
          - #include <path/to/file.ext>
          - #pragma test needs("path/to/file.ext")
        note: uses DFS
    """
    active = Set()
    files = Set()

    if not path.exists(source):
        return active

    curpath = path.dirname(source) + "/"

    with open(source, 'r') as file:
        for line in file:
            dep = ""
            res = None
            for rxp in deprxp:
                # if line encodes dependency
                res = rxp.match(line)
                if res is not None:
                    dep = res.group(1)
                    if dep != "":
                        if not path.exists(dep):
                            # try same path as current dependency
                            if path.exists(curpath + dep):
                                dep = curpath + dep
                            else: # try include paths
                                for sp in searchpaths:
                                    if path.exists(sp + dep):
                                        dep = sp + dep
                                        break

                        active.add(dep)
                        file = path.basename(dep)
                        if dep not in sofar and file not in files:
                            files.add(file)
                            active.update(dependencies(dep, searchpaths, active.union(sofar)))
                        break

    active.add(source)
    return active


# initialize
onwindows = os.name == "nt"

artifactext = ""
if onwindows: artifactext = ".exe"

paths = []
sources = []
includes = []
showDependencies = False
haltOnFail = True
recompile = False
allpass = True

# process input args
if len(argv) > 1:
    for i in range(1,len(argv)):
        arg = argv[i]
        if arg == "-h" or arg == "--help":
            print "Usage:"
            print "  " + argv[0] + \
                " [--help]" \
                " [--clean]" \
                " [-r]" \
                " [-d]" \
                " [-c <compiler>]" \
                " [-o <compiler-options>]" \
                " [-m <macro>]..." \
                " [-i <include-path>]..." \
                " [--continue-on-fail]" \
                " [<directory|file>...]"
            print ""
            print "Options:"
            print "  -h, --help                         print this screen"
            print "  --clean                            do a clean re-build; removes entire build directory"
            print "  -r, --recompile                    recompile all source files before running"
            print "  -d, --show-dependecies             show all resolved includes during compilation"
            print "  -c, --compiler <executable>        specify compiler executable"
            print "  -o, --compiler-options <options>   specify compiler options"
            print "  -m, --macro <macro>                add macro definition"
            print "  -i, --include <path>               add include path"
            print "  --continue-on-fail                 continue running regardless of failed builds or tests";
            exit()
        elif arg == "--clean":
            if os.path.exists(builddir):
                shutil.rmtree(builddir)
        elif arg == "-r" or arg == "--recompile":
            recompile = True
        elif arg == "-d" or arg == "--show-dependencies":
            showDependencies = True
        elif arg == "--continue-on-fail":
            haltOnFail = False
        elif arg == "-c" or arg == "--compiler":
            if i+1 < len(argv): compiler = argv[i+1]
        elif arg == "-o" or arg == "--compiler-options":
            if i+1 < len(argv): compileopt = argv[i+1]
        elif arg == "-i" or arg == "--include":
            if i+1 < len(argv): incpats.add(argv[i+1])
        elif arg == "-m" or arg == "--macro":
            if i+1 < len(argv): macros.add(argv[i+1])
        else:
            paths.append(arg)

# gather source file names
if len(paths) < 1:
    paths = [os.getcwd()]

for p in paths:
    if p.endswith("." + tuext):
        sources.append(p)
    else:
        sources.extend([path.join(root, name)
             for root, dirs, files in os.walk(p)
             for name in files
             if name.endswith("." + tuext)])

if len(sources) < 1:
    print "ERROR: no source files found"
    exit()

# make build directory
if not os.path.exists(builddir):
    os.makedirs(builddir)
    print separator
    print "C L E A N  B U I L D"

print separator

# compile and run tests
compilecmd = compiler + " " + compileopt
for m in macros:
    if m != "": compilecmd = compilecmd + " -D" + m

for ip in incpaths:
    if ip != "": compilecmd = compilecmd + " -I " + ip

#print compilecmd

for source in sources:
    res1 = testrxp.match(source)
    res2 = testrxp.match(path.basename(source))
    if res1 is not None and res2 is not None:
        tname = res1.group(1)
        sname = res2.group(1)
        stdout.write("testing " + tname + " > checking depdendencies > ")
        stdout.flush()
        artifact = builddir + "/" + sname + artifactext

        srcdeps = dependencies(source, incpaths)

        if showDependencies:
            print ""
            for dep in srcdeps: print "    needs " + dep
            stdout.write("    ")
            stdout.flush()

        doCompile = recompile or not path.exists(artifact)
        if not doCompile:
             for dep in srcdeps:
                if path.exists(dep):
                    if str(path.getmtime(artifact)) < str(path.getmtime(dep)):
                        doCompile = True
                        break
                else:
                    print "ERROR: dependency " + dep + " could not be found!"
                    exit()

        if doCompile:
            stdout.write("compiling > ")
            stdout.flush()

            if path.exists(artifact):
                os.remove(artifact)

            tus = ""
            for dep in srcdeps:
                 if dep.endswith("." + tuext):
                     tus = tus + " " + dep

            system(compilecmd + " " + tus + " -o " + artifact)
            if not path.exists(artifact):
                print "FAILED!"
                allpass = False
                if haltOnFail: exit();

        #execute test; backslashes make sure that it works with cmd.exe
        if onwindows:
            artifact = artifact.replace("/", "\\")

        stdout.write("running > ")
        stdout.flush()
        runres = system(artifact)
        if runres == 0:
            print "passed."
        else:
            print "FAILED!"
            allpass = False
            if haltOnFail : exit()

print separator

if allpass:
    print "All tests passed."
else:
    print "Some tests failed."

