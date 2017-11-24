#include "AudioReader.h"
#include "AudioWriter.h"
#include "Predictor.h"
#include "Golomb.h"
#include "Bitstream.h"
#include <sndfile.h>

int main(int argc, char const *argv[])
{
	Predictor predictor;
	Golomb golomb;
	short order;
	int m;
	audio_data_t values, decoded;
	encoded_data_t encoded_data, encoded_file;
	SF_INFO snd_info;
	AudioReader *ar;
	block_data_t block_d;
	
	//m = std::atoi(argv[2]);
	ar = new AudioReader(argv[1]);
	ar->get_values(values);
	memset (&snd_info, 0, sizeof (snd_info));
	ar->get_info(&snd_info);
/* REAL */

	// Encoding
	Bitstream bs_write("encoded_bitstream.cod", "w");
	order = 1; 
	predictor.predict_blocks(values, block_d);
	//golomb.real_encode(values, &encoded_data);
	golomb.encode_blocks(values, block_d, &encoded_data);
	m = golomb.get_m();
	bs_write.writeFile_blocks(encoded_data, snd_info, block_d, BLOCK_SIZE);
	bs_write.close();
	
	// Decoding
	AudioWriter a_out(std::string("out.wav"));
	SF_INFO new_snd_info;
	ushort dec_order = -1, new_m = -1;
	Bitstream bs_read("encoded_bitstream.cod", "r");
	//encoded_file = bs_read.readFile(&new_snd_info, &dec_order, &new_m);
	block_data_t block_from_file;
	encoded_file = bs_read.readFile_blocks(&new_snd_info, &block_from_file);
	bs_read.close();
	//golomb.real_decode(encoded_file, &decoded, new_m);
	golomb.decode_blocks(encoded_file, block_from_file, &decoded);
	//predictor.reverse(decoded, dec_order);
	predictor.reverse_blocks(decoded, block_from_file);
	a_out.write_values(decoded, new_snd_info);

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