#!/usr/bin/python

###############################################################################
#
# builds and runs tests, resolves include dependencies
#
# see help for usage: ./run_tests.py --help
#
# (c) 2013-2017 Andre Mueller
#
###############################################################################

import re
import os
import glob
import shutil
import ntpath
from os import path
from os import system
from sys import stdout
from sys import argv
from sys import exit
from sets import Set

#default settings
builddir = "../build_test"
incpaths = ["", "../include/"]
macros   = [] # ["NO_DEBUG", "NDEBUG"]
compiler = "gcc"
valgrind = "valgrind --error-exitcode=1"
gcov     = "gcov -l "

gccflags = ("-std=c++0x -O0 -g "
                    " -Wall -Wextra -Wpedantic "
                    " -Wno-unknown-pragmas"
                    " -Wno-unknown-warning"
                    " -Wno-unknown-warning-option"
                    " -Wformat=2 "
                    " -Wall -Wextra -Wpedantic "
                    " -Wcast-align -Wcast-qual "
                    " -Wconversion "
                    " -Wctor-dtor-privacy "
                    " -Wdisabled-optimization "
                    " -Wdouble-promotion "
                    " -Winit-self "
                    " -Wlogical-op "
                    " -Wmissing-include-dirs "
                    " -Wno-sign-conversion "
                    " -Wnoexcept "
                    " -Wold-style-cast "
                    " -Woverloaded-virtual "
                    " -Wredundant-decls "
                    " -Wshadow "
                    " -Wstrict-aliasing=1 "
                    " -Wstrict-null-sentinel "
                    " -Wstrict-overflow=5 "
                    " -Wswitch-default "
                    " -Wundef "
                    " -Wno-unknown-pragmas "
                    " -Wuseless-cast ")

#available compilers
compilers = {
    "gcc"   : {"exe": "g++",
               "flags" : gccflags,
               "macro" : "-D", "incpath" : "-I",
               "obj" : "",
               "cov" : "-fprofile-arcs -ftest-coverage",
               "link" : "-o "
    },
    "clang" : {"exe": "clang++",
               "flags" : gccflags,
               "macro" : "-D", "incpath" : "-I",
               "obj" : "",
               "cov" : "-fprofile-arcs -ftest-coverage",
               "link" : "-o "
    },
    "msvc" : {"exe": "cl",
              "flags" : " /W4 /EHsc ",
              "macro" : "/D:", "incpath" : "/I:",
              "obj" : "/Foc:",
              "cov" : "",
              "link" : "/link /out:"
    }
}

tuext      = "cpp"
separator  = "-----------------------------------------------------------------"

# [ (needs compilation, dependency regex) ]
deprxp = [re.compile('^\s*#pragma\s+test\s+needs\(\s*"(.+\..+)"\s*\)\s*$'),
          re.compile('^\s*#include\s+\<(.+\..+)\>\s*$'),
          re.compile('^\s*#include\s+"(.+\..+)"\s*$')]

testrxp = re.compile('(.+)\.' + tuext)



# support functions
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
pathsep = "/"
if onwindows:
    artifactext = ".exe"
    pathsep = "\\"

paths = []
sources = []
includes = []
showDependencies = False
haltOnFail = True
recompile = False
allpass = True
useValgrind = False
useGcov = False
doClean = False

# process input args
if len(argv) > 1:
    next = 0
    for i in range(1,len(argv)):
        if i >= next:
            arg = argv[i]
            if arg == "-h" or arg == "--help":
                print "Usage:"
                print "  " + argv[0] + \
                    " [--help]" \
                    " [--clean]" \
                    " [-c (gcc|clang|msvc)]" \
                    " [-r]" \
                    " [-d]" \
                    " [--continue-on-fail]" \
                    " [--valgrind]" \
                    " [--gcov]" \
                    " [<directory|file>...]"
                print ""
                print "Options:"
                print "  -h, --help                        print this screen"
                print "  --clean                           do a clean re-build; removes entire build directory"
                print "  -r, --recompile                   recompile all source files before running"
                print "  -d, --show-dependecies            show all resolved includes during compilation"
                print "  -c, --compiler (gcc|clang|msvc)   select compiler"
                print "  --valgrind                        run test through valgrind"
                print "  --gcov                            run test through gcov"
                print "  --continue-on-fail                continue running regardless of failed builds or tests";
                exit(0)
            elif arg == "--clean":
                doClean = True
            elif arg == "-r" or arg == "--recompile":
                recompile = True
            elif arg == "-d" or arg == "--show-dependencies":
                showDependencies = True
            elif arg == "--continue-on-fail":
                haltOnFail = False
            elif arg == "--valgrind":
                useValgrind = True
            elif arg == "--gcov":
                useGcov = True
            elif arg == "-c" or arg == "--compiler":
                if i+1 < len(argv):
                    compiler = argv[i+1]
                    next = i + 2
            else:
                paths.append(arg)



# get compiler-specific strings
if compiler not in compilers.keys():
    print "ERROR: compiler " + compiler + " not supported"
    print "choose one of:"
    for key in compilers.keys():
        print "    " + key
    exit(1)

compileexec = compilers[compiler]["exe"]
compileopts = compilers[compiler]["flags"]
macroOpt    = compilers[compiler]["macro"]
includeOpt  = compilers[compiler]["incpath"]
objOutOpt   = compilers[compiler]["obj"]
linkOpt     = compilers[compiler]["link"]
coverageOpt = compilers[compiler]["cov"]

if useGcov:
    compileopts = compileopts + " " + coverageOpt

if onwindows:
    builddir = builddir.replace("/", "\\")

builddir = builddir + "_" + compiler



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
    exit(1)



# make build directory
if doClean:
    if os.path.exists(builddir): shutil.rmtree(builddir)
    for f in glob.glob("*.gcov"): os.remove(f)
    for f in glob.glob("*.gcda"): os.remove(f)
    for f in glob.glob("*.gcno"): os.remove(f)

if not os.path.exists(builddir):
    os.makedirs(builddir)
    print separator
    print "C L E A N  B U I L D"

print separator



# compile and run tests
compilecmd = compileexec + " " + compileopts
print "compiler call: "
print compilecmd
print separator

for m in macros:
    if onwindows: m = m.replace("/", "\\")
    if m != "": compilecmd = compilecmd + " " + macroOpt + m

for ip in incpaths:
    if onwindows: ip = ip.replace("/", "\\")
    if ip != "": compilecmd = compilecmd + " " + includeOpt + ip


for source in sources:
    if onwindows: source = source.replace("/", "\\")

    res1 = testrxp.match(source)
    res2 = testrxp.match(path.basename(source))
    if res1 is not None and res2 is not None:
        tname = res1.group(1)
        sname = res2.group(1)
        stdout.write("testing " + tname + " > checking depdendencies > ")
        stdout.flush()
        artifact = builddir + pathsep + sname + artifactext
        if onwindows: artifact = artifact.replace("/", "\\")

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
                    exit(1)

        if doCompile:
            stdout.write("compiling > ")
            stdout.flush()

            if path.exists(artifact):
                os.remove(artifact)

            tus = ""
            for dep in srcdeps:
                 if dep.endswith("." + tuext):
                     tus = tus + " " + dep

            if onwindows: tus = tus.replace("/", "\\")

            compilecall = compilecmd

            # object file output
            if objOutOpt != "":
                objfile = builddir + pathsep + sname + ".o"
                compilecall = compilecall + " " + objOutOpt + objfile

            compilecall = compilecall + " " + tus + " " + linkOpt + artifact

            system(compilecall)
            if not path.exists(artifact):
                print "FAILED!"
                allpass = False
                if haltOnFail: exit(1);

        stdout.write("running > ")
        stdout.flush()
        runres = system(artifact)

        if runres == 0 and useValgrind:
            print "valgrind > "
            runres = system(valgrind + " " + artifact)

        if runres == 0 and useGcov:
            stdout.write("gcov > ")
            system(gcov + " " + source)

        if runres == 0:
            print "passed"
        else:
            print "FAILED!"
            allpass = False
            if haltOnFail : exit(1)


print separator

if allpass:
    print "All tests passed."
    exit(0)
else:
    print "Some tests failed."
    exit(1)

