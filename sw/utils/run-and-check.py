#!/usr/bin/env python

import sys, os, glob
import subprocess


if len(sys.argv) != 4:
    print "ERROR: Usage is {0} <vsim> <tcl_script> <pulp_core>".format(sys.argv[0])
    sys.exit(-1)

vsim       = sys.argv[1]
tcl_script = sys.argv[2]
pulp_core  = sys.argv[3]

# remove stdout and core_state from previous runs
for f in glob.glob('./stdout/*'):
    if os.path.isfile(f):
        os.remove(f)
if os.path.isfile('./core_state'):
    os.remove('./core_state')

env_vars = os.environ
env_vars["PULP_CORE"] = pulp_core

# run simulation
cmd = [vsim, '-64', '-c', '-do', 'source tcl_files/{0}; run -a; exit'.format(tcl_script)]

print cmd
subprocess.call(cmd, env=env_vars)

with open('./stdout/uart', 'rb') as f:
     for line in f:
        if "OOOOOOK!!!!!!" in line:
            sys.exit(0)

sys.exit(1)
