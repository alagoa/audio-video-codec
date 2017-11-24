#!/bin/bash

order=2

./TheRealAudioCodec -i ../samples/sample01.wav -e -b 4096 -k $order -o e_s1_b_o$order > /dev/null
./TheRealAudioCodec -i e_s1_b_o$order.cod -d -b 4096 -o d_s1_b_o$order > /dev/null
ls -lh e_s1_b_o$order.cod ../samples/sample01.wav | awk '{print $5, $9}'
md5sum d_s1_b_o$order.wav ../samples/sample01.wav

./TheRealAudioCodec -i ../samples/sample02.wav -e -b 4096 -k $order -o e_s2_b_o$order > /dev/null
./TheRealAudioCodec -i e_s2_b_o$order.cod -d -b 4096 -o d_s2_b_o$order > /dev/null
ls -lh e_s2_b_o$order.cod ../samples/sample02.wav | awk '{print $5, $9}'
md5sum d_s2_b_o$order.wav ../samples/sample02.wav

./TheRealAudioCodec -i ../samples/sample03.wav -e -b 4096 -k $order -o e_s3_b_o$order > /dev/null
./TheRealAudioCodec -i e_s3_b_o$order.cod -d -b 4096 -o d_s3_b_o$order > /dev/null
ls -lh e_s3_b_o$order.cod ../samples/sample03.wav | awk '{print $5, $9}'
md5sum d_s3_b_o$order.wav ../samples/sample03.wav

./TheRealAudioCodec -i ../samples/sample04.wav -e -b 4096 -k $order -o e_s4_b_o$order > /dev/null
./TheRealAudioCodec -i e_s4_b_o$order.cod -d -b 4096 -o d_s4_b_o$order > /dev/null
ls -lh e_s4_b_o$order.cod ../samples/sample04.wav | awk '{print $5, $9}'
md5sum d_s4_b_o$order.wav ../samples/sample04.wav

./TheRealAudioCodec -i ../samples/sample05.wav -e -b 4096 -k $order -o e_s5_b_o$order > /dev/null
./TheRealAudioCodec -i e_s5_b_o$order.cod -d -b 4096 -o d_s5_b_o$order > /dev/null
ls -lh e_s5_b_o$order.cod ../samples/sample05.wav | awk '{print $5, $9}'
md5sum d_s5_b_o$order.wav ../samples/sample05.wav

./TheRealAudioCodec -i ../samples/sample06.wav -e -b 4096 -k $order -o e_s6_b_o$order > /dev/null
./TheRealAudioCodec -i e_s6_b_o$order.cod -d -b 4096 -o d_s6_b_o$order > /dev/null
ls -lh e_s6_b_o$order.cod ../samples/sample06.wav | awk '{print $5, $9}'
md5sum d_s6_b_o$order.wav ../samples/sample06.wav

./TheRealAudioCodec -i ../samples/sample07.wav -e -b 4096 -k $order -o e_s7_b_o$order > /dev/null
./TheRealAudioCodec -i e_s7_b_o$order.cod -d -b 4096 -o d_s7_b_o$order > /dev/null
ls -lh e_s7_b_o$order.cod ../samples/sample07.wav | awk '{print $5, $9}'
md5sum d_s7_b_o$order.wav ../samples/sample07.wav

rm d_s* e_s*