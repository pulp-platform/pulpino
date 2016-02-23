#!/usr/bin/env python
import sys,os,subprocess

# git command to use
git = "git"

# uncomment if you want to set it manually
# server = "git@iis-git.ee.ethz.ch:pulp-project"

cwd = os.getcwd()

errors = []

class tcolors:
    OK      = '\033[92m'
    WARNING = '\033[93m'
    ERROR   = '\033[91m'
    ENDC    = '\033[0m'

devnull = open(os.devnull, 'wb')

with open("ips_list.txt", "rb") as f:
    ips_list = f.readlines()

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
    stdout = execute_out("%s remote -v" % (git))

    stdout = stdout.split('\n')
    for line in stdout:
        if "origin" in line:
            splits = line.split(' ')
            splits = splits[0].split('\t')
            return splits[1]

    print tcolors.ERROR + "ERROR: could not find remote server." + tcolors.ENDC
    sys.exit(1)

if vars().has_key('server'):
    server = find_server()

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
    # make sure we are in the correct directory to start
    os.chdir(cwd)
    print

    # check if directory already exists, this hints to the fact that we probably already cloned it
    if os.path.isdir("./%s" % ip['path']):
        os.chdir("./%s" % ip['path'])

        # now check if the directory is a git directory
        if not os.path.isdir(".git"):
            print tcolors.ERROR + "ERROR: Found a normal directory instead of a git directory at %s. You may have to delete this folder to make this script work again" % os.getcwd() + tcolors.ENDC
            errors.append("%s - %s: Not a git directory" % (ip['name'], ip['path']));
            continue

        print tcolors.OK + "Updating %s..." % ip['name'] + tcolors.ENDC

        # fetch everything first so that all commits are available later
        ret = execute("%s fetch" % (git))
        if ret != 0:
            print tcolors.ERROR + "ERROR: could not fetch IP %s." % (ip['name']) + tcolors.ENDC
            errors.append("%s - Could not fetch" % (ip['name']));
            continue

        # make sure we have the correct branch/tag for the pull
        ret = execute("%s checkout %s" % (git, ip['commit']))
        if ret != 0:
            print tcolors.ERROR + "ERROR: could not checkout IP %s at %s." % (ip['name'], ip['commit']) + tcolors.ENDC
            errors.append("%s - Could not checkout commit %s" % (ip['name'], ip['commit']));
            continue

        # check if we are in detached HEAD mode
        stdout = execute_out("%s status" % git)

        if not ("HEAD detached" in stdout):
            # only do the pull if we are not in detached head mode
            ret = execute("%s pull --ff-only" % git)
            if ret != 0:
                print tcolors.ERROR + "ERROR: could not update %s" % ip['name'] + tcolors.ENDC
                errors.append("%s - Could not update" % (ip['name']));
                continue

    # Not yet cloned, so we have to do that first
    else:
        os.chdir("./")
        ret = execute("%s clone %s/%s.git %s" % (git, server, ip['name'], ip['path']))
        if ret != 0:
            print tcolors.ERROR + "ERROR: could not clone, you probably have to remove the %s directory." % ip['name'] + tcolors.ENDC
            errors.append("%s - Could not clone" % (ip['name']));
            continue

        os.chdir("./%s" % ip['path'])
        ret = execute("%s checkout %s" % (git, ip['commit']))
        if ret != 0:
            print tcolors.ERROR + "ERROR: could not checkout IP %s at %s." % (ip['name'], ip['commit']) + tcolors.ENDC
            errors.append("%s - Could not checkout commit %s" % (ip['name'], ip['commit']));
            continue

os.chdir(cwd)


print '\n\n'
print tcolors.WARNING + "SUMMARY" + tcolors.ENDC

if len(errors) == 0:
    print tcolors.OK + "IPs updated successfully!" + tcolors.ENDC
    sys.exit(0)
else:
    for error in errors:
        print tcolors.ERROR + '    %s' % (error) + tcolors.ENDC

    print

    print tcolors.ERROR + "ERRORS during IP update!" + tcolors.ENDC
    sys.exit(1)
