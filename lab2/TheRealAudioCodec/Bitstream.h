#include <fstream>
#include <algorithm>
#include <sndfile.h>
#include <cmath>
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
		encoded_data_t readFile(SF_INFO *new_snd_info, ushort *dec_order, ushort *new_m);


	private:
		std::fstream file;
		std::string mode;
		char w_buff;
		char r_buff;
		int r_buff_pos;
		int w_buff_pos;
		int byte_count;
};