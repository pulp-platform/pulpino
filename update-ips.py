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

with open("ips_list.txt", "rb") as f:
    ips_list = f.readlines()

def execute(cmd):
    return subprocess.call(cmd.split())

# get a list of all IPs that we are interested in from ips_list.txt
ips = []
for ips_el in ips_list:
    if ips_el[0] == "#":
        continue
    try:
        splits = ips_el.split()

        path   = splits[0]
        commit = splits[1]

        path_split = splits[0].split('/')

        name = path_split[-1]

        ips.append({'name': name, 'commit': commit, 'path': path })
    except IndexError:
        continue

# update or clone the IPs
for ip in ips:
    # check if directory already exists, this hints to the fact that we probably already cloned it
    if os.path.isdir("./fe/ips/%s" % ip['path']):
        os.chdir("./fe/ips/%s" % ip['path'])

        # now check if the directory is a git directory
        if not os.path.isdir(".git"):
            print tcolors.ERROR + "ERROR: Found a normal directory instead of a git directory at %s. You may have to delete this folder to make this script work again" % os.getcwd() + tcolors.ENDC
            sys.exit(1)

        print tcolors.OK + "Updating %s..." % ip['name'] + tcolors.ENDC

        # fetch everything first so that all commits are available later
        ret = execute("%s fetch" % (git))
        if ret != 0:
            print tcolors.ERROR + "ERROR: could not fetch IP %s." % (ip['name']) + tcolors.ENDC
            sys.exit(1)

        # make sure we have the correct branch/tag for the pull
        ret = execute("%s checkout %s" % (git, ip['commit']))
        if ret != 0:
            print tcolors.ERROR + "ERROR: could not checkout IP %s at %s." % (ip['name'], ip['commit']) + tcolors.ENDC
            sys.exit(1)

        ret = execute("%s pull --ff-only" % git)
        if ret != 0:
            print tcolors.ERROR + "ERROR: could not update %s" % ip['name'] + tcolors.ENDC
        os.chdir(cwd)
        print

    # Not yet cloned, so we have to do that first
    else:
        os.chdir("./fe")
        ret = execute("%s clone git@kesch.ee.ethz.ch:pulp-project/%s.git ips/%s" % (git, ip['name'], ip['path']))
        if ret != 0:
            print tcolors.ERROR + "ERROR: could not clone, you probably have to remove the fe/ips/%s directory. Try with remove-ips.py" % ip['name'] + tcolors.ENDC
            sys.exit(1)
        os.chdir("./ips/%s" % ip['path'])
        ret = execute("%s checkout %s" % (git, ip['commit']))
        if ret != 0:
            print tcolors.ERROR + "ERROR: could not checkout IP %s at %s." % (ip['name'], ip['commit']) + tcolors.ENDC
            sys.exit(1)
        os.chdir(cwd)

# Create symlink for scripts
if not os.path.islink('fe/ips/scripts'):
    execute("ln -s %s/fe/sim/ips_scripts fe/ips/scripts" % cwd)
sys.exit(0)
