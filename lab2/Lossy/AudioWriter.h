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
	AudioWriter(std::string filename);
	~AudioWriter();
	void close();
	int write_values(audio_data_t const &data, SF_INFO new_snd_info);
	//int write(void* data_ptr);
private:
	sf_count_t f_plswork;
	SF_INFO snd_info;
	SNDFILE* snd_file;
	std::string filename;
};