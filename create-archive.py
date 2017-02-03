#!/usr/bin/env python

import sys,os,subprocess,re
from time import strftime

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

def find_url():
    stdout = execute_out("git remote -v")

    stdout = stdout.split('\n')
    for line in stdout:
        if "origin" in line:
            tmp = line.split(' ')
            tmp = tmp[0].split('\t')

            remote = tmp[1]

            return remote

    print "ERROR: could not find remote server."
    sys.exit(1)

pulpino_url = find_url()

print "Using remote PULPino url %s" % (pulpino_url)


# make sure we use a clean directory
execute("rm -rf ./tmp")

os.mkdir("./tmp");
os.chdir("./tmp");

if execute("git clone %s" % (pulpino_url)) != 0:
    print "ERROR: could not clone fresh PULPino"
    sys.exit(1)

os.chdir("./pulpino");

if execute("./update-ips.py") != 0:
    print "ERROR: could not update IPs"
    sys.exit(1)

# now remove the git folders, no point in keeping them for a zip file
matches = []
for root, dirnames, filenames in os.walk('.'):
    for dirname in dirnames:
        if dirname == ".git":
            print "delete folders %s/%s" % (root, dirname)
            execute("rm -rf %s/%s" % (root, dirname))

datestr = strftime("%Y-%m-%d_%H:%M:%S")
execute("tar -czvf ./../../pulpino-%s.tar.gz ./" % (datestr))
