#include "AudioEntropy.h"

AudioEntropy::AudioEntropy(){

}

AudioEntropy::~AudioEntropy(){
	//free(hists);
}

AudioEntropy::AudioEntropy(audio_data_t const &values) {
	memset (&snd_info, 0, sizeof (snd_info)) ;
	snd_info.frames = values[0].size();
	snd_info.channels = values.size();
	hists.reserve(values[0].size() * 2);
	for (uint chan = 0 ; chan < values.size() ; chan ++) {
		for (uint k = 0 ; k < values[0].size() ; k++) {	
			hists[ values[chan][k] ] ++;	// Add values to mono channel
		}
	}
	// Process mono channel with round up integer division
	for (counter::iterator it = hists.begin(); it != hists.end(); ++it) {
		it->second = (it->second + (values.size() - 1)) / values.size();
	}
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

	while ((readcount = reader (snd_file, data, BUFFER_LEN, snd_info.format)))
	{	
		process_data(data, readcount, snd_info.channels);
	}

	// Process mono channel with round up integer division
	for (counter::iterator it = hists.begin(); it != hists.end(); ++it) {
		it->second = (it->second + (snd_info.channels - 1)) / snd_info.channels;
	}
	sf_close(snd_file);
	//print_histogram();
	//save_histogram();
}


double AudioEntropy::entropy(){
	double sum = 0;
	double prob_A = 0;
	for (counter::iterator it = hists.begin(); it != hists.end(); ++it)
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
			hists[ data[k] ] ++;	// Add values to mono channel
		}
	}
}


void AudioEntropy::print_histogram(){
	std::cout << "channel " << 1 << "\n";
	for (auto &e : hists)
	{
		std::cout << "[" << e.first << "] = " << e.second << '\n';	
	}
}

int AudioEntropy::reader(SNDFILE* sndfile, void* data_ptr, sf_count_t items, int format){
	switch(format & 0x0F){
	case SF_FORMAT_PCM_U8:
	case SF_FORMAT_PCM_S8:
	case SF_FORMAT_PCM_16:
		//std::cout << "oi 16" << "\n";
		return sf_read_short(sndfile, (short*)data_ptr, items);
		break;
	case SF_FORMAT_PCM_24:
	case SF_FORMAT_PCM_32:
		//std::cout << "oi 32" << "\n";
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

SF_INFO AudioEntropy::get_snd_info() {
	return snd_info;
}