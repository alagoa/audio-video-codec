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
	hists = (counter*) malloc(sizeof(counter) * snd_info.channels);
	if (hists == NULL)
	{
		std::cout << "Error in malloc!\n";
		return;
	}
	for (int i = 0; i < snd_info.channels; ++i)
	{
		hists[i] = counter();
	}

	while ((readcount = reader (snd_file, data, BUFFER_LEN, snd_info.format)))
	{	
		process_data(data, readcount, snd_info.channels);
	}
	print_histogram();
	save_histogram();
}

void AudioEntropy::process_data(short *data, int count, int channels){
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

int AudioEntropy::reader(SNDFILE* sndfile, void* data_ptr, sf_count_t items, int format){
	switch(format & 0x0F){
	case SF_FORMAT_PCM_U8:
	case SF_FORMAT_PCM_S8:
	case SF_FORMAT_PCM_16:
		std::cout << "read short\n";
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

	for(int i = 0; i < snd_info.channels; ++i) 
	{	
		std::stringstream fname;
		fname << i << "channel.txt";
		std::cout << "saving " << fname.str() << "\n";
		std::ofstream fout(fname.str());
		std::stringstream out;

		for (auto &e : hists[i]) {
			out << e.first << " " << e.second << '\n';	
		}
		
		fout << out.str();
		fout.close();
	}	
}