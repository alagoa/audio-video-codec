#include "AudioReader.h"
#include "AudioWriter.h"
#include "Predictor.h"
#include "Golomb.h"
#include <sndfile.h>

int main(int argc, char const *argv[])
{
	SF_INFO snd_info;
	AudioReader *ar = new AudioReader(argv[1]);
	audio_data_t values = ar->get_values();
	memset (&snd_info, 0, sizeof (snd_info));
	ar->get_info(&snd_info);
	AudioWriter a_out(std::string("out.wav"), snd_info.frames, 
		snd_info.samplerate, snd_info.channels, snd_info.format);
	//delete ar;

	/*
	
	std::vector<short> channel1 = {5, 10, 15, 20, 25, 30, 35, 40};
	std::vector<short> channel2 = {5, 15, 25, 35, 45, 55, 65, 75};

	std::vector<std::vector<short>> values;
	values.push_back(channel1);
	values.push_back(channel2);
*/
	Predictor predictor;
	Golomb golomb(4);
	golomb.encode(predictor.predict(values));
	
	std::vector<std::vector<short>> original = predictor.reverse(golomb.decode());
	a_out.write_values(original);
	for(auto &e : original) {
		std::cout << "channel-> ";
		for(auto &v : e) {
			std::cout << v << ", ";
		}
		std::cout << "\n";
	}
	return 0;
	
}