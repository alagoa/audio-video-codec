#include "AudioReader.h"
#include "AudioWriter.h"
#include "Predictor.h"
#include "Golomb.h"
#include "Bitstream.h"
#include <sndfile.h>

int main(int argc, char const *argv[])
{
	int m = std::atoi(argv[2]);
	SF_INFO snd_info;
	AudioReader *ar = new AudioReader(argv[1]);
	audio_data_t values = ar->get_values();
	memset (&snd_info, 0, sizeof (snd_info));
	ar->get_info(&snd_info);

	Predictor predictor;
	Golomb golomb(m);
	Bitstream bs_write("encoded_bitstream.cod", "w");

	// Encoding
	bs_write.writeFile(golomb.real_encode(predictor.predict(values)));


	Bitstream bs_read("encoded_bitstream.cod", "r");
	AudioWriter a_out(std::string("out.wav"), snd_info.frames, 
		snd_info.samplerate, snd_info.channels, snd_info.format, snd_info.sections, snd_info.seekable);				// meter isto tudo para dentro do ficheiro encoded

	// Decoding
	a_out.write_values(predictor.reverse(golomb.real_decode(bs_read.readFile())));
	
	return 0;
}