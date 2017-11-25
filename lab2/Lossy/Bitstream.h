#include <fstream>
#include <algorithm>
#include <sndfile.h>
#include <cmath>
#include <iostream>
#include "structures.h"

class Bitstream
{
	public:
		Bitstream();
		Bitstream(std::string file_name, std::string mode);
		~Bitstream();
		void close();
		int writeBit(char value);
		int write_unary(uint unary);
		int write_r(uint r, int b);
		char readBit();
		void writeBits(int value, int n_bits);
		int readBits(int n_bits);
		uint read_unary();
		uint read_r(int b);
		int getByteCount();
		void writeFile(encoded_data_t const &golomb_encoded, SF_INFO snd_info, ushort order, ushort m);
		void writeFile_blocks(encoded_data_t const &golomb_encoded, 
							  SF_INFO snd_info,
							  block_data_t const &b_data, 
							  ushort block_size);
		void write_block(encoded_channel_t::const_iterator data_p,
					     encoded_channel_t::const_iterator data_end,
					     ushort block_size,
						 block_header const &b_header);

		encoded_data_t readFile(SF_INFO *new_snd_info, ushort *dec_order, ushort *new_m);
		encoded_data_t readFile_blocks(SF_INFO *snd_info, block_data_t *b_data);


	private:
		std::fstream file;
		std::string mode;
		char w_buff;
		char r_buff;
		int r_buff_pos;
		int w_buff_pos;
		int byte_count;
};