#!/bin/sh
A=$(find -name XiaofaoHPC3-1.o*)
B=$(find -name XiaofanHPC3-2.o*)
C=$(find -name XiaofanHPC3-4.o*)
D=$(find -name XiaofanHPC3-8.o*)
python combine.py $A $B $C $D > result