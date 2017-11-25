#!/bin/bash

order=2
block_size=16000

time $(./TheRealAudioCodec -i ../samples/sample01.wav -e -b $block_size -k $order -o e_s1_b_o$order > /dev/null)
./TheRealAudioCodec -i e_s1_b_o$order.cod -d -b $block_size -o d_s1_b_o$order > /dev/null
ls -lh e_s1_b_o$order.cod ../samples/sample01.wav ../samples/sample01.flac | awk '{print $5, $9}'
md5sum d_s1_b_o$order.wav ../samples/sample01.wav

time $(./TheRealAudioCodec -i ../samples/sample02.wav -e -b $block_size -k $order -o e_s2_b_o$order > /dev/null)
./TheRealAudioCodec -i e_s2_b_o$order.cod -d -b $block_size -o d_s2_b_o$order > /dev/null
ls -lh e_s2_b_o$order.cod ../samples/sample02.wav ../samples/sample02.flac | awk '{print $5, $9}'
md5sum d_s2_b_o$order.wav ../samples/sample02.wav

time $(./TheRealAudioCodec -i ../samples/sample03.wav -e -b $block_size -k $order -o e_s3_b_o$order > /dev/null)
./TheRealAudioCodec -i e_s3_b_o$order.cod -d -b $block_size -o d_s3_b_o$order > /dev/null
ls -lh e_s3_b_o$order.cod ../samples/sample03.wav ../samples/sample03.flac | awk '{print $5, $9}'
md5sum d_s3_b_o$order.wav ../samples/sample03.wav

time $(./TheRealAudioCodec -i ../samples/sample04.wav -e -b $block_size -k $order -o e_s4_b_o$order > /dev/null)
./TheRealAudioCodec -i e_s4_b_o$order.cod -d -b $block_size -o d_s4_b_o$order > /dev/null
ls -lh e_s4_b_o$order.cod ../samples/sample04.wav ../samples/sample04.flac | awk '{print $5, $9}'
md5sum d_s4_b_o$order.wav ../samples/sample04.wav

time $(./TheRealAudioCodec -i ../samples/sample05.wav -e -b $block_size -k $order -o e_s5_b_o$order > /dev/null)
./TheRealAudioCodec -i e_s5_b_o$order.cod -d -b $block_size -o d_s5_b_o$order > /dev/null
ls -lh e_s5_b_o$order.cod ../samples/sample05.wav ../samples/sample05.flac | awk '{print $5, $9}'
md5sum d_s5_b_o$order.wav ../samples/sample05.wav

time $(./TheRealAudioCodec -i ../samples/sample06.wav -e -b $block_size -k $order -o e_s6_b_o$order > /dev/null)
./TheRealAudioCodec -i e_s6_b_o$order.cod -d -b $block_size -o d_s6_b_o$order > /dev/null
ls -lh e_s6_b_o$order.cod ../samples/sample06.wav ../samples/sample06.flac | awk '{print $5, $9}'
md5sum d_s6_b_o$order.wav ../samples/sample06.wav

time $(./TheRealAudioCodec -i ../samples/sample07.wav -e -b $block_size -k $order -o e_s7_b_o$order > /dev/null)
./TheRealAudioCodec -i e_s7_b_o$order.cod -d -b $block_size -o d_s7_b_o$order > /dev/null
ls -lh e_s7_b_o$order.cod ../samples/sample07.wav ../samples/sample07.flac | awk '{print $5, $9}'
md5sum d_s7_b_o$order.wav ../samples/sample07.wav

#rm d_s* e_s*