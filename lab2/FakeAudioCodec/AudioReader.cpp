#include "AudioReader.h"

AudioReader::AudioReader(){

}

AudioReader::AudioReader(std::string filename){
	//short data [BUFFER_LEN];
	//int readcount;
	memset (&snd_info, 0, sizeof (snd_info)) ;
	snd_file = sf_open(filename.c_str(), SFM_READ, &snd_info);
	if (snd_file == NULL)
	{
		std::cout << sf_strerror(snd_file) << "\n";
		return;
	}
	get_reader(snd_info.format);
	/*
	while ((readcount = reader (snd_file, data, BUFFER_LEN, snd_info.format)))
	{	
		process_data(data, readcount, snd_info.channels);
	}
	*/
}

audio_data_t AudioReader::get_values(){
	audio_data_t values;
	short data [BUFFER_LEN];
	int readcount, chan, k;
	while((readcount = read(data))){
		for (int i = 0; i < readcount; i += snd_info.channels)
		{
			values.push_back(std::vector< short >(snd_info.channels));
			for (int chan = 0; chan < snd_info.channels; ++chan)
			{
				values[i][chan] = data[i + chan];
			}
		}
	}
	return values;
}

int AudioReader::read(void* data_ptr){
	return reader_f(sndfile, data_ptr, BUFFER_LEN);
}

void AudioReader::get_reader(int format){
	switch(format & 0x0F){
	case SF_FORMAT_PCM_U8:
	case SF_FORMAT_PCM_S8:
	case SF_FORMAT_PCM_16:
		//std::cout << "oi 16" << "\n";
		reader_f = read_short;
		//return sf_read_short(sndfile, (short*)data_ptr, items);
		break;
	case SF_FORMAT_PCM_24:
	case SF_FORMAT_PCM_32:
		//std::cout << "oi 32" << "\n";
		reader_f = read_int;
		//return sf_read_int(snd_file, (int*)data_ptr, items);
		break;
	case SF_FORMAT_FLOAT:
		reader_f = read_float;
		//return sf_read_float(sndfile, (float*)data_ptr, items);
		break;
	case SF_FORMAT_DOUBLE:
		reader_f = read_double;
		//return sf_read_double(sndfile, (double*)data_ptr, items);
		break;
	}
}
AudioReader::~AudioReader(){

}

void AudioReader::close(){
	sf_close(snd_file);
}

inline int AudioReader::read_short(SNDFILE* sndfile, void* data_ptr, sf_count_t items){
	return sf_read_short(sndfile, (short*)data_ptr, items);
}

inline int AudioReader::read_int(SNDFILE* sndfile, void* data_ptr, sf_count_t items){
	return sf_read_int(snd_file, (int*)data_ptr, items);
}

inline int AudioReader::read_float(SNDFILE* sndfile, void* data_ptr, sf_count_t items){
	return sf_read_float(sndfile, (float*)data_ptr, items);
}

inline int AudioReader::read_double(SNDFILE* sndfile, void* data_ptr, sf_count_t items){
	return sf_read_double(sndfile, (double*)data_ptr, items);
}