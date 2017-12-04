#include "AudioReader.h"
#include "AudioWriter.h"
#include "Predictor.h"
#include "Golomb.h"
#include "Bitstream.h"
#include <boost/program_options.hpp>
#include <sys/stat.h>
#include <sndfile.h>
#include <string>

namespace po = boost::program_options;

inline void encode_file(std::string input, std::string output, short order, int inter_chan, int do_blocks);
inline void decode_file(std::string input, std::string output, int do_blocks);
inline bool file_exists (const std::string& name);

int handleArgs(int argc, char* argv[], 
std::string &input, short &order, int &inter_chan, std::string &output, 
int &do_blocks, int &do_encode, int &do_decode) 
{
	do_encode = 0;
	do_decode = 0;
	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help,h", 																"produce help message")
	    ("input,i", po::value<std::string>(&input),								"input file")
	    ("order,k", po::value<short>(&order)->default_value(1),					"set order of the model, only used in block encoding")
	    ("inter-channel,c", po::value<int>(&inter_chan)->default_value(1),		"inter-channel decorrelation, always on (for now)")
	    ("output,o", po::value<std::string>(&output)->default_value("out"),		"output file")
	    ("blocks,b", po::value<int>(&do_blocks)->default_value(4096),			"size of the blocks, if less than 64 it will not use blocks. When decoding use -b 0 to decode without blocks (we forgot this in the encoded header)")
	    ("encode,e", po::value<int>(&do_encode)->implicit_value(1),				"encode")
	    ("decode,d", po::value<int>(&do_decode)->implicit_value(1),				"decode")
	;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help") || !vm.count("input")) {
	    std::cout << desc << "\n";
	    return 0;
	}
	
	if(vm.count("encode") && vm.count("decode")) {	
		std::cout << "Choose encode xor decode." << "\n";
		return 0;
	}

	if(vm.count("input")) {	
		if(!file_exists(vm["input"].as<std::string>())) {
			std::cout << "Input file (" << vm["input"].as<std::string>() <<") not found." << "\n";
			return 0;
		}
	}

	if (vm.count("order"))
	{
		if(vm["order"].as<short>() < 0 || vm["order"].as<short>() > 15) {
			std::cout << "Order needs to be a positive number less than 15.\n" << desc << "\n";
			return 0;
		}
	}
	if (vm.count("blocks"))
	{
		if(vm["blocks"].as<int>() < 64) {
			do_blocks = 0;
		}
	}
	return 1;
}




int main(int argc, char *argv[])
{
	std::string input;
	short order;
	int inter_chan;
	std::string output; 
	int do_blocks;
	int do_encode;
	int do_decode;

	if (!handleArgs( argc, argv,	input, 
				order, inter_chan, output, 
				do_blocks, do_encode, do_decode))
	{
		return 1;
	}
	std::cout << "in file: " << input << "\nout file:" << output << "\norder: " << order 
			  << "\ninter_chan: " << inter_chan << "\ndo_blocks: " << do_blocks << "\ndo_encode: " << do_encode
			  << "\ndo decode: " << do_decode << "\n";

	if (do_encode)
	{
		encode_file(input, output, order, inter_chan, do_blocks);
	}
	else{
		decode_file(input, output, do_blocks);
	}
	return 0;
	/*
	Predictor predictor;
	Golomb golomb;
	int m;
	audio_data_t values, decoded;
	encoded_data_t encoded_data, encoded_file;
	block_data_t block_d;
	SF_INFO snd_info;
	
	AudioReader ar = new AudioReader(input_file);
	ar->get_values(values);
	memset (&snd_info, 0, sizeof (snd_info));
	ar->get_info(&snd_info);

	// Encoding
	Bitstream bs_write("encoded_bitstream.cod", "w");
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
	return 0;
	*/
}

inline void encode_file(std::string input, std::string output, short order, int inter_chan, int do_blocks){
	Predictor predictor;
	Golomb golomb;
	audio_data_t values;
	encoded_data_t encoded_data;
	SF_INFO snd_info;
	Bitstream bs_write(output +".cod", "w");

	AudioReader* ar = new AudioReader(input);
	ar->get_values(values);
	memset (&snd_info, 0, sizeof (snd_info));
	ar->get_info(&snd_info);

	if (do_blocks)
	{
		block_data_t block_d;
		predictor.predict_blocks(values, block_d, order, do_blocks);
		golomb.encode_blocks(values, block_d, &encoded_data);
		bs_write.writeFile_blocks(encoded_data, snd_info, block_d, do_blocks);
	}
	else{
		int m;
		order = predictor.predict(values);
		golomb.real_encode(values, &encoded_data);
		m = golomb.get_m();
		bs_write.writeFile(encoded_data, snd_info, order, m);
	}
	bs_write.close();
	delete ar;
}

inline void decode_file(std::string input, std::string output, int do_blocks){
	Predictor predictor;
	Golomb golomb;
	AudioWriter a_out(std::string(output + ".wav"));
	Bitstream bs_read(input, "r");
	audio_data_t decoded;
	encoded_data_t encoded_file;
	SF_INFO snd_info;
	if (do_blocks)
	{
		block_data_t block_from_file;
		encoded_file = bs_read.readFile_blocks(&snd_info, &block_from_file);
		bs_read.close();
		golomb.decode_blocks(encoded_file, block_from_file, &decoded);
		predictor.reverse_blocks(decoded, block_from_file);
	}
	else{
		ushort dec_order = -1;
		ushort m = -1;
		encoded_file = bs_read.readFile(&snd_info, &dec_order, &m);
		bs_read.close();
		golomb.real_decode(encoded_file, &decoded, m);
		predictor.reverse(decoded, dec_order);
	}
	a_out.write_values(decoded, snd_info);
}

inline bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}