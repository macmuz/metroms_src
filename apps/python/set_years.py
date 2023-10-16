#!/usr/bin/env python3
import re
import sys

start = int(sys.argv[1])
stop = int(sys.argv[2])

status = 0
with open("Params.py", "r") as f:
    lines = f.readlines()
with open("Params_copy.py", "w") as f:
    for line in lines:
        f.write(line)
with open("Params.py", "w") as f:
    for line in lines:
        if line.find("RIVERFILE") != -1 or \
           line.find("_BRYNAME_") != -1:
            status = 1
            f.write(line)
            continue
        if status == 1:
            if line.find("]") != -1:
                newline = line.split("\"],")[0]
            else:
                newline = line.split(" | ")[0]
            for i in range(start,stop):
                myline = re.sub("_[0-9]{4}", "_"+str(i), newline)
                f.write(myline+" | \\n\"+\\\n")
            lastline = re.sub("_[0-9]{4}", "_"+str(stop), newline)
            f.write(lastline+"\"],\n")
            status = 2 
        if status == 2:
            if line.find("]") != -1:
                status = 0
            continue

        if line.find("ATMDIR") != -1:
            status = 3
            f.write(line)
            continue
        if status == 3:
            if line.find("]") != -1:
                newline = line.split("\"],")[0]
            elif line.find(" \\ ") != -1:
                newline = line.split(" \\ ")[0]
            else:
                newline = line.split(" | ")[0]
            for i in range(start,stop):
                myline = re.sub("_[0-9]{4}", "_"+str(i), newline)
                f.write(myline+" | \\n\"+\\\n")
            oldline = newline
            status = 4
        if status == 4:
            if line.find(" \\ ") != -1:
                myline = re.sub("_[0-9]{4}", "_"+str(stop), oldline)
                f.write(myline+" \\ \\n\"+\\\n")
                status = 3
            if line.find("]") != -1:
                myline = re.sub("_[0-9]{4}", "_"+str(stop), oldline)
                f.write(myline+"\"],\n")
                status = 0
            continue
        f.write(line)
