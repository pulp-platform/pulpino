#!/usr/bin/env python
# Francesco Conti <f.conti@unibo.it>
#
# Copyright (C) 2016 ETH Zurich, University of Bologna.
# All rights reserved.

import sys,os,subprocess,re

devnull = open(os.devnull, 'wb')

def execute(cmd, silent=False):
    if silent:
        stdout = devnull
    else:
        stdout = None
    return subprocess.call(cmd.split(), stdout=stdout)

def execute_out(cmd, silent=False):
    p = subprocess.Popen(cmd.split(), stdout=subprocess.PIPE)
    out, err = p.communicate()
    return out

def find_server():
    stdout = execute_out("git remote -v")

    stdout = stdout.split('\n')
    for line in stdout:
        if "origin" in line:
            splits = line.split(' ')
            splits = splits[0].split('\t')

            remote = splits[1]

            # now we have to remove the pulpino.git suffix and figure out the group
            remote =  remote.rsplit('/', 1)[0]
            tmp = remote[::-1]
            tmp = re.split(r'[:/]', tmp, 1)
            server = tmp[1][::-1]
            group = tmp[0][::-1]

            return [server, group, remote]

    print tcolors.ERROR + "ERROR: could not find remote server." + tcolors.ENDC
    sys.exit(1)

if not vars().has_key('server'):
    [server, group, remote] = find_server()

print "Using remote git server %s" % (server)


# download IPApproX tools in ./ipstools and import them
if os.path.exists("ipstools") and os.path.isdir("ipstools"):
    cwd = os.getcwd()
    os.chdir("ipstools")
    execute("git pull", silent=True)
    os.chdir(cwd)
    import ipstools
else:
    # try to find the ipstools repository
    if execute("git clone %s/IPApproX ipstools" % (remote)) != 0:
        if execute("git clone %s:pulp-tools/IPApproX ipstools" % (server)) != 0:
            execute("git clone %s/pulp-tools/IPApproX ipstools" % (server))

    import ipstools

try:
    os.mkdir("fe/ips")
except OSError:
    pass
# creates an IPApproX database
ipdb = ipstools.IPDatabase(ips_dir="./ips", skip_scripts=True)
# updates the IPs from the git repo
ipdb.update_ips()

# launch generate-ips.py
execute("./generate-scripts.py")

