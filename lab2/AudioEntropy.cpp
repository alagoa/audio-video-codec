#include "AudioEntropy.h"

AudioEntropy::AudioEntropy(){

}

AudioEntropy::~AudioEntropy(){
	free(hists);
}

AudioEntropy::AudioEntropy(std::string filename){
	double data [BUFFER_LEN];
	int readcount;
	memset (&snd_info, 0, sizeof (snd_info)) ;
	snd_file = sf_open(filename.c_str(), SFM_READ, &snd_info);
	if (snd_file == NULL)
	{
		std::cout << sf_strerror(snd_file) << "\n";
		return;
	}
	hists = (std::unordered_map<double, int>*) malloc(sizeof(std::unordered_map<double, int>) * snd_info.channels);
	if (hists == NULL)
	{
		std::cout << "Error in malloc!\n";
		return;
	}
	for (int i = 0; i < snd_info.channels; ++i)
	{
		hists[i] = std::unordered_map<double, int>();
	}
	while ((readcount = sf_read_double (snd_file, data, BUFFER_LEN)))
	{	
		process_data(data, readcount, snd_info.channels);
	}
	print_histogram();
}

void AudioEntropy::process_data(double *data, int count, int channels){
	int chan;
	int k;
	for (chan = 0 ; chan < channels ; chan ++){
		for (k = chan ; k < count ; k+= channels)
			hists[chan][ data[k] ]++;
	}
}

void AudioEntropy::print_histogram(){
	for(int i = 0; i < snd_info.channels; ++i) 
	{
		std::cout << "channel " << i << "\n";
		for (auto &e : hists[i])
		{
			std::cout << "[" << e.first << "] = " << e.second << '\n';	
		}
	}
}