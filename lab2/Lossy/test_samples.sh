#!/bin/bash

time ./Lossy -i ../samples/sample01.wav -e -b 4096 -k 1 -o e_s1_b_o1 > /dev/null
./Lossy -i e_s1_b_o1.cod -d -b 4096 -o d_s1_b_o1 > /dev/null
ls -l e_s1_b_o1.cod ../samples/sample01.wav

time ./Lossy -i ../samples/sample02.wav -e -b 4096 -k 1 -o e_s2_b_o1 > /dev/null
./Lossy -i e_s2_b_o1.cod -d -b 4096 -o d_s2_b_o1 > /dev/null
ls -l e_s2_b_o1.cod ../samples/sample02.wav

time ./Lossy -i ../samples/sample03.wav -e -b 4096 -k 1 -o e_s3_b_o1 > /dev/null
./Lossy -i e_s3_b_o1.cod -d -b 4096 -o d_s3_b_o1 > /dev/null
ls -l e_s3_b_o1.cod ../samples/sample03.wav

time ./Lossy -i ../samples/sample04.wav -e -b 4096 -k 1 -o e_s4_b_o1 > /dev/null
./Lossy -i e_s4_b_o1.cod -d -b 4096 -o d_s4_b_o1 > /dev/null
ls -l e_s4_b_o1.cod ../samples/sample04.wav

time ./Lossy -i ../samples/sample05.wav -e -b 4096 -k 1 -o e_s5_b_o1 > /dev/null
./Lossy -i e_s5_b_o1.cod -d -b 4096 -o d_s5_b_o1 > /dev/null
ls -l e_s5_b_o1.cod ../samples/sample05.wav

time ./Lossy -i ../samples/sample06.wav -e -b 4096 -k 1 -o e_s6_b_o1 > /dev/null
./Lossy -i e_s6_b_o1.cod -d -b 4096 -o d_s6_b_o1 > /dev/null
ls -l e_s6_b_o1.cod ../samples/sample06.wav

time ./Lossy -i ../samples/sample07.wav -e -b 4096 -k 1 -o e_s7_b_o1 > /dev/null
./Lossy -i e_s7_b_o1.cod -d -b 4096 -o d_s7_b_o1 > /dev/null
ls -l e_s7_b_o1.cod ../samples/sample07.wav

ls -l e_s*.cod d_s*.wav