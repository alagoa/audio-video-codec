#include <stdio.h>
#include <sndfile.h>
#include <string>
#include <iostream>
#include <cstring>
#include <unordered_map>

#define BUFFER_LEN 1024

class AudioEntropy
{
public:
	AudioEntropy();
	AudioEntropy(std::string filename);
	~AudioEntropy();
	void process_data(double *data, int count, int channels);
	void print_histogram();
private:
	SF_INFO snd_info;
	SNDFILE* snd_file;
	std::unordered_map<double, int>* hists;
};