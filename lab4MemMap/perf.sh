#!/bin/bash
set -v
dd if=/dev/urandom of=test.dat  bs=50M count=1

time ./mmap 20
diff -q -s test.dat output.dat

time ./mmap 40
diff -q -s test.dat output.dat

time ./mmap 60
diff -q -s test.dat output.dat



time ./realloc 20 
diff -q -s test.dat output.dat

time ./realloc 40 
diff -q -s test.dat output.dat

time ./realloc 60 
diff -q -s test.dat output.dat
