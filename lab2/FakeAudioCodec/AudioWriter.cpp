#include "AudioWriter.h"

AudioWriter::AudioWriter(){

}


AudioWriter::AudioWriter(std::string filename, sf_count_t frames,
 int samplerate, int channels, int format){
	memset (&snd_info, 0, sizeof (snd_info)) ;
	snd_info.frames = frames;
	f_plswork = frames;
	snd_info.samplerate = samplerate;
	snd_info.channels = channels;
	snd_info.format = format;
	snd_file = sf_open(filename.c_str(), SFM_WRITE, &snd_info);
	if (snd_file == NULL)
	{
		std::cout << sf_strerror(snd_file) << "\n";
		return;
	}
}


int AudioWriter::write_values(audio_data_t data){
	short *buffer;
	snd_info.frames = f_plswork;
	if (! (buffer = (short*)malloc (snd_info.channels * snd_info.frames * sizeof (short))))
	{	
		printf ("Malloc failed.\n") ;
		return -1;
	}

	for (int k = 0 ; k < snd_info.frames ; k+=snd_info.channels)
	{
		for (int i = 0; i < snd_info.channels; ++i)
		{
			buffer[k + i] = data[i][k]; 
		}
	}
	if (sf_write_short (snd_file, buffer, snd_info.channels * snd_info.frames) !=
											snd_info.channels * snd_info.frames)
		return -1;
	sf_close (snd_file) ;
	free (buffer) ;
	return 0;
}

AudioWriter::~AudioWriter(){

}

void AudioWriter::close(){
}