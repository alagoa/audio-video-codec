#include <sndfile.h>
#include <utility>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstring>

#define BUFFER_LEN 1024
typedef std::vector< std::vector<short> > audio_data_t;
class AudioReader
{
public:
	AudioReader();
	AudioReader(std::string filename);
	~AudioReader();
	void close();
	audio_data_t get_values();
	int read(void* data_ptr);
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