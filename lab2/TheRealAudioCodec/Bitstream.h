#include <fstream>
#include <algorithm>
#include <sndfile.h>
#include "structures.h"

class Bitstream
{
	public:
		Bitstream();
		Bitstream(std::string file_name, std::string mode);
		~Bitstream();
		void close();
		void writeBit(int value);
		int readBit();
		void writeBits(int value, int n_bits);
		int readBits(int n_bits);
		int getByteCount();
		void writeFile(encoded_data_t golomb_encoded, SF_INFO snd_info, int order);
		encoded_data_t readFile(SF_INFO *new_snd_info, int *dec_order, int *new_m);


	private:
		std::fstream file;
		std::string mode;
		uchar buff;
		int pos;
		int byte_count;
};