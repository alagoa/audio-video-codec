#include <stdio.h>
#include <sndfile.h>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <map>
#include <fstream>
#include <algorithm>

#define BUFFER_LEN 1024
typedef std::map<int, unsigned int> counter;
class AudioEntropy
{
public:
	AudioEntropy();
	AudioEntropy(std::string filename);
	~AudioEntropy();
	void process_data(short *data, int count, int channels);
	void print_histogram();
	int reader(SNDFILE* sndfile, void* data_ptr, sf_count_t items, int format);
	void save_histogram();
	double entropy();
private:
	unsigned int average(counter::value_type v);
	SF_INFO snd_info;
	SNDFILE* snd_file;
	counter* hists;
};