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
#include <vector>

#define BUFFER_LEN 1024
typedef std::map<int, unsigned int> counter;

class AudioEntropy
{
public:
	AudioEntropy();
	AudioEntropy(std::string filename);
	AudioEntropy(std::vector<std::vector<short>> values);
	~AudioEntropy();
	void process_data(short *data, int count, int channels);
	void print_histogram();
	int reader(SNDFILE* sndfile, void* data_ptr, sf_count_t items, int format);
	void save_histogram();
	double entropy();
	const counter* get_counters();
	SF_INFO get_snd_info();

private:
	unsigned int average(counter::value_type v);
	SF_INFO snd_info;
	const SNDFILE* snd_file;
	counter* hists;
	int n_channels;
};