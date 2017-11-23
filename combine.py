#!/usr/bin/python
# -*- coding: UTF-8 -*-

import sys

def readAll(file):
    with open(file) as f:
        return f.readlines()

def readFrom(file, line):
    with open(file) as f:
        return f.readlines()[line:]

def printPart(title, pos, f):
    print(title)
    for i in xrange(0, 4):
        sys.stdout.write(f[i][pos])

def main():
    f = []
    print(sys.argv)
    f.append(readFrom(sys.argv[1], 4))
    for i in xrange(2, 4):
        f.append(readAll(sys.argv[i]))
    printPart('Original:', 0, f)
    printPart('Part1:', 1, f)
    printPart('Part2:', 2, f)
    printPart('Part3:', 3, f)

if __name__ == '__main__':
    main()
