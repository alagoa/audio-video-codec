#include <sndfile.h>
#include <utility>
#include <string>
#include <stdio.h>
#include <iostream>
#include "structures.h"
#include <cstring>

class AudioWriter
{
public:
	AudioWriter();
	AudioWriter(std::string filename, sf_count_t frames,
 int samplerate, int channels, int format);
	~AudioWriter();
	void close();
	int write_values(audio_data_t data);
	//int write(void* data_ptr);
private:
	/*
	void get_reader(int format);
	int read_short(void* data_ptr, sf_count_t items);
	int read_int(void* data_ptr, sf_count_t items);
	int read_float(void* data_ptr, sf_count_t items);
	int read_double(void* data_ptr, sf_count_t items);
	*/
	sf_count_t f_plswork;
	SF_INFO snd_info;
	SNDFILE* snd_file;
	//int (AudioReader::*reader_f)(void*, sf_count_t) = NULL;
	
};