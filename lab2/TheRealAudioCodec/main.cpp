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
	int order;

/* REAL */

	// Encoding
	Bitstream bs_write("encoded_bitstream.cod", "w");
	audio_data_t pred = predictor.predict(values, &order);
	bs_write.writeFile(golomb.real_encode(pred), snd_info, order);
	// Decoding
	AudioWriter a_out(std::string("out.wav"));
	SF_INFO new_snd_info;
	int dec_order = -1, new_m = -1;
	Bitstream bs_read("encoded_bitstream.cod", "r");
	encoded_data_t encoded = bs_read.readFile(&new_snd_info, &dec_order, &new_m);
	a_out.write_values(predictor.reverse(golomb.real_decode(encoded, new_m), dec_order), new_snd_info);

/* FAKE */
/*
	// Encode
	audio_data_t pred = predictor.predict(values, &order);
	golomb.encode(pred, snd_info, order);

	// Decode
	AudioWriter a_out(std::string("out.wav"));
	SF_INFO new_snd_info;
	int dec_order = -1;
	audio_data_t decoded = golomb.decode(&new_snd_info, &dec_order);
	a_out.write_values(predictor.reverse(decoded, order), new_snd_info);
*/
	return 0;
}