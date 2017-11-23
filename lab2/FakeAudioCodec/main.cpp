#include "AudioReader.h"
#include "AudioWriter.h"
#include "Predictor.h"
#include "Golomb.h"
#include <sndfile.h>

int main(int argc, char const *argv[])
{
	SF_INFO snd_info;
	uint final_size, init_size;
	short order_e = 0;
	encoded_data_t encoded;
	audio_data_t decoded, values;

	AudioReader *ar;
	Predictor predictor;
	Golomb golomb(std::atoi(argv[2]));
	AudioWriter a_out;
	
	memset (&snd_info, 0, sizeof (snd_info));
	ar = new AudioReader(argv[1]);
	ar->get_values(values);
	ar->get_info(&snd_info);
	a_out = AudioWriter(std::string("out.wav"), snd_info.frames, 
		snd_info.samplerate, snd_info.channels, snd_info.format, snd_info.sections, snd_info.seekable);
	init_size = snd_info.channels * snd_info.frames * sizeof(short); 
	
	
	order_e = predictor.predict(values);
	final_size = golomb.encode(values, &encoded);
	golomb.decode(encoded, &decoded);
	predictor.reverse(decoded, order_e);
	a_out.write_values(decoded);
	std::cout << "Inital size: " << init_size << "\n";
	std::cout << "Final size : " << final_size << "\n";
	/*
	for(auto &e : original) {
		std::cout << "channel-> ";
		for(auto &v : e) {
			std::cout << v << ", ";
		}
		std::cout << "\n";
	}
	*/
	return 0;
	
}