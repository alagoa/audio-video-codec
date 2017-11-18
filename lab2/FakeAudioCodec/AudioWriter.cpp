#include "AudioWriter.h"

AudioWriter::AudioWriter(){

}


AudioWriter::AudioWriter(std::string filename, sf_count_t frames,
 						 int samplerate, int channels, int format, int sections, int seekable){
	memset (&snd_info, 0, sizeof (snd_info)) ;
	snd_info.frames = frames;
	f_plswork = frames;
	snd_info.samplerate = samplerate;
	snd_info.channels = channels;
	snd_info.format = format;
	snd_info.sections = sections;
	snd_info.seekable = seekable;
	snd_file = sf_open(filename.c_str(), SFM_WRITE, &snd_info);
	if (snd_file == NULL)
	{
		std::cout << sf_strerror(snd_file) << "\n";
		return;
	}
}


int AudioWriter::write_values(audio_data_t const &data){
	short *buffer;
	snd_info.frames = f_plswork;
	if (! (buffer = (short*)malloc (snd_info.channels * snd_info.frames * sizeof (short))))
	{	
		printf ("Malloc failed.\n") ;
		return -1;
	}

	for (int chan = 0 ; chan < snd_info.channels; ++chan)
		for (int i = 0; i < snd_info.frames; ++i)
			buffer[chan + 2 * i] = data[chan][i]; 

	if (sf_write_short (snd_file, buffer, snd_info.channels * snd_info.frames) !=
											snd_info.channels * snd_info.frames)
		return -1;
	sf_close (snd_file);
	free (buffer);
	return 0;
}

AudioWriter::~AudioWriter(){

}

void AudioWriter::close(){
}