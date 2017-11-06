#include "AudioEntropy.h"

AudioEntropy::AudioEntropy(){

}

AudioEntropy::~AudioEntropy(){
	free(hists);
	sf_close(snd_file);
}

AudioEntropy::AudioEntropy(std::string filename){
	short data [BUFFER_LEN];
	int readcount;
	memset (&snd_info, 0, sizeof (snd_info)) ;
	snd_file = sf_open(filename.c_str(), SFM_READ, &snd_info);
	if (snd_file == NULL)
	{
		std::cout << sf_strerror(snd_file) << "\n";
		return;
	}
	hists = (counter*) malloc(sizeof(counter) * (snd_info.channels + 1));						// All channels + MONO
	if (hists == NULL)
	{
		std::cout << "Error in malloc!\n";
		return;
	}
	for (int i = 0; i < snd_info.channels + 1; ++i)
	{
		hists[i] = counter();
	}

	while ((readcount = reader (snd_file, data, BUFFER_LEN, snd_info.format)))
	{	
		process_data(data, readcount, snd_info.channels);
	}

	// Process mono channel with round up integer division
	for (counter::iterator it = hists[snd_info.channels].begin(); it != hists[snd_info.channels].end(); ++it) {
		it->second = (it->second + (snd_info.channels - 1)) / snd_info.channels;
	}

	//print_histogram();
	save_histogram();
}


double AudioEntropy::entropy(){
	double sum = 0;
	double prob_A = 0;
	for (counter::iterator it = hists[snd_info.channels].begin(); it != hists[snd_info.channels].end(); ++it)
	{
		prob_A = ((double)it->second / (double)snd_info.frames);
		sum += prob_A * std::log2(prob_A);
	}
	return -sum;
} 

void AudioEntropy::process_data(short *data, int count, int channels){
	int chan;
	int k;
	// Process channels
	for (chan = 0 ; chan < channels ; chan ++) {
		for (k = chan ; k < count ; k+= channels) {
			hists[chan][ data[k] ]++;	
			hists[snd_info.channels][ data[k] ] ++;	// Add values to mono channel
		}
	}
}


void AudioEntropy::print_histogram(){
	for(int i = 0; i < snd_info.channels+1; ++i) 
	{
		std::cout << "channel " << i << "\n";
		for (auto &e : hists[i])
		{
			std::cout << "[" << e.first << "] = " << e.second << '\n';	
		}
	}
}

int AudioEntropy::reader(SNDFILE* sndfile, void* data_ptr, sf_count_t items, int format){
	switch(format & 0x0F){
	case SF_FORMAT_PCM_U8:
	case SF_FORMAT_PCM_S8:
	case SF_FORMAT_PCM_16:
		return sf_read_short(sndfile, (short*)data_ptr, items);
		break;
	case SF_FORMAT_PCM_24:
	case SF_FORMAT_PCM_32:
		return sf_read_int(snd_file, (int*)data_ptr, items);
		break;
	case SF_FORMAT_FLOAT:
		return sf_read_float(sndfile, (float*)data_ptr, items);
		break;
	case SF_FORMAT_DOUBLE:
		return sf_read_double(sndfile, (double*)data_ptr, items);
		break;
	}
	return -1;
}

void AudioEntropy::save_histogram() {
	std::ofstream fout("histogram.dat");
	std::stringstream out;
	for (auto const& x : hists[snd_info.channels]) {
		out << x.first << " ";
		for(int k = 0; k < snd_info.channels+1; k++) {
			counter::iterator innerit = hists[k].find(x.first);
			// Value in current map
			if(hists[k].end() != innerit) {
				out << hists[k][x.first] << " ";
			}
			else {
				out << "0 ";
			}
		}
		out << "\n";
	}
	fout << out.str();
	fout.close();
}