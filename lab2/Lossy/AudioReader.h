#include <sndfile.h>
#include <utility>
#include <string>
#include <stdio.h>
#include <iostream>
#include "structures.h"
#include <cstring>

class AudioReader
{
public:
	AudioReader();
	AudioReader(std::string filename);
	~AudioReader();
	void close();
	void get_values(audio_data_t &values);
	int read(void* data_ptr);
	int get_info(SF_INFO* info);

private:
	void get_reader(int format);
	int read_short(void* data_ptr, sf_count_t items);
	int read_int(void* data_ptr, sf_count_t items);
	int read_float(void* data_ptr, sf_count_t items);
	int read_double(void* data_ptr, sf_count_t items);
	SF_INFO snd_info;
	SNDFILE* snd_file;
	int (AudioReader::*reader_f)(void*, sf_count_t) = NULL;
	
};