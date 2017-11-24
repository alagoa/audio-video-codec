#!/bin/bash

./TheRealAudioCodec -i ../samples/sample01.wav -e -b 4096 -k 1 -o e_s1_b_o1 > /dev/null
./TheRealAudioCodec -i e_s1_b_o1.cod -d -b 4096 -o d_s1_b_o1 > /dev/null
md5sum d_s1_b_o1.wav ../samples/sample01.wav

./TheRealAudioCodec -i ../samples/sample02.wav -e -b 4096 -k 1 -o e_s2_b_o1 > /dev/null
./TheRealAudioCodec -i e_s2_b_o1.cod -d -b 4096 -o d_s2_b_o1 > /dev/null
md5sum d_s2_b_o1.wav ../samples/sample02.wav

./TheRealAudioCodec -i ../samples/sample03.wav -e -b 4096 -k 1 -o e_s3_b_o1 > /dev/null
./TheRealAudioCodec -i e_s3_b_o1.cod -d -b 4096 -o d_s3_b_o1 > /dev/null
md5sum d_s3_b_o1.wav ../samples/sample03.wav

./TheRealAudioCodec -i ../samples/sample04.wav -e -b 4096 -k 1 -o e_s4_b_o1 > /dev/null
./TheRealAudioCodec -i e_s4_b_o1.cod -d -b 4096 -o d_s4_b_o1 > /dev/null
md5sum d_s4_b_o1.wav ../samples/sample04.wav

./TheRealAudioCodec -i ../samples/sample05.wav -e -b 4096 -k 1 -o e_s5_b_o1 > /dev/null
./TheRealAudioCodec -i e_s5_b_o1.cod -d -b 4096 -o d_s5_b_o1 > /dev/null
md5sum d_s5_b_o1.wav ../samples/sample05.wav

./TheRealAudioCodec -i ../samples/sample06.wav -e -b 4096 -k 1 -o e_s6_b_o1 > /dev/null
./TheRealAudioCodec -i e_s6_b_o1.cod -d -b 4096 -o d_s6_b_o1 > /dev/null
md5sum d_s6_b_o1.wav ../samples/sample06.wav

./TheRealAudioCodec -i ../samples/sample07.wav -e -b 4096 -k 1 -o e_s7_b_o1 > /dev/null
./TheRealAudioCodec -i e_s7_b_o1.cod -d -b 4096 -o d_s7_b_o1 > /dev/null
md5sum d_s7_b_o1.wav ../samples/sample07.wav

ls -lh e_s*.cod d_s*.wav