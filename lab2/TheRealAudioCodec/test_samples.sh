#!/bin/bash

./TheRealAudioCodec ../samples/sample01.wav > /dev/null
ls -lh encoded_bitstream.cod ../samples/sample01.wav
md5sum out.wav ../samples/sample01.wav

./TheRealAudioCodec ../samples/sample02.wav > /dev/null
ls -lh encoded_bitstream.cod ../samples/sample02.wav
md5sum out.wav ../samples/sample02.wav

./TheRealAudioCodec ../samples/sample03.wav > /dev/null
ls -lh encoded_bitstream.cod ../samples/sample03.wav
md5sum out.wav ../samples/sample03.wav

./TheRealAudioCodec ../samples/sample04.wav > /dev/null
ls -lh encoded_bitstream.cod ../samples/sample04.wav
md5sum out.wav ../samples/sample04.wav

./TheRealAudioCodec ../samples/sample05.wav > /dev/null
ls -lh encoded_bitstream.cod ../samples/sample05.wav
md5sum out.wav ../samples/sample05.wav

./TheRealAudioCodec ../samples/sample06.wav > /dev/null
ls -lh encoded_bitstream.cod ../samples/sample06.wav
md5sum out.wav ../samples/sample06.wav

./TheRealAudioCodec ../samples/sample07.wav > /dev/null
ls -lh encoded_bitstream.cod ../samples/sample07.wav
md5sum out.wav ../samples/sample07.wav
