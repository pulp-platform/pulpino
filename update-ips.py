#!/usr/bin/env python
import sys,os,subprocess

# git command to use
git = "git"
cwd = os.getcwd()

class tcolors:
    OK      = '\033[92m'
    WARNING = '\033[93m'
    ERROR   = '\033[91m'
    ENDC    = '\033[0m'

def execute(cmd):
    return subprocess.call(cmd.split())


ret = execute("%s submodule init" % git)

if ret != 0:
    print tcolors.ERROR + "ERROR: could not init submodules"
    sys.exit(1)

ret = execute("%s submodule update" % git)
if ret != 0:
    print tcolors.ERROR + "ERROR: could not update submodules"
    sys.exit(1)


sys.exit(0)
