from sys import stdin
from sys import stdout

in_doc = 0;

for line in stdin:
    if in_doc == 1:
         stdout.write(line)
    if line.find("<end_doc>") >= 0:
        in_doc = 0
    elif line.find("<begin_doc>") >= 0:
        in_doc = 1

