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

int AudioReader::get_info(SF_INFO* info){
	info->frames = snd_info.frames;
	info->samplerate = snd_info.samplerate;
	info->channels = snd_info.channels;
	info->format = snd_info.format;
	info->sections = snd_info.sections;
	info->seekable = snd_info.seekable;
	return 0;
}

void AudioReader::get_values(audio_data_t &values){
	//values.push_back(snd_info.channels);
	values.reserve(snd_info.channels);
	for (int i = 0; i < snd_info.channels; ++i){
		values.emplace_back();
		values.back().reserve(snd_info.frames);
	}
	short data [BUFFER_LEN];
	int readcount;
	while((readcount = read(data))){
		for (int i = 0; i < readcount; i += snd_info.channels)
		{
			for (int chan = 0; chan < snd_info.channels; ++chan)
			{
				values[chan].push_back(data[i + chan]/32);
			}
		}
	}
}

int AudioReader::read(void* data_ptr){
	return (*this.*reader_f)(data_ptr, BUFFER_LEN);
}

void AudioReader::get_reader(int format){
	switch(format & 0x0F){
	case SF_FORMAT_PCM_U8:
	case SF_FORMAT_PCM_S8:
	case SF_FORMAT_PCM_16:
		//std::cout << "oi 16" << "\n";
		reader_f = &AudioReader::read_short;
		//return sf_read_short(sndfile, (short*)data_ptr, items);
		break;
	case SF_FORMAT_PCM_24:
	case SF_FORMAT_PCM_32:
		//std::cout << "oi 32" << "\n";
		reader_f = &AudioReader::read_int;
		//return sf_read_int(snd_file, (int*)data_ptr, items);
		break;
	case SF_FORMAT_FLOAT:
		reader_f = &AudioReader::read_float;
		//return sf_read_float(sndfile, (float*)data_ptr, items);
		break;
	case SF_FORMAT_DOUBLE:
		reader_f = &AudioReader::read_double;
		//return sf_read_double(sndfile, (double*)data_ptr, items);
		break;
	}
}
AudioReader::~AudioReader(){

}

void AudioReader::close(){
	sf_close(snd_file);
}

inline int AudioReader::read_short(void* data_ptr, sf_count_t items){
	return sf_read_short(snd_file, (short*)data_ptr, items);
}

inline int AudioReader::read_int(void* data_ptr, sf_count_t items){
	return sf_read_int(snd_file, (int*)data_ptr, items);
}

inline int AudioReader::read_float(void* data_ptr, sf_count_t items){
	return sf_read_float(snd_file, (float*)data_ptr, items);
}

inline int AudioReader::read_double(void* data_ptr, sf_count_t items){
	return sf_read_double(snd_file, (double*)data_ptr, items);
}