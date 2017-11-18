#include "Bitstream.h"

Bitstream::Bitstream(std::string file_name, std::string mode) {
	this->pos = 0;
	this->buff = 0;
	this->byte_count = 0;
	std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
	this->mode = mode;
	if(this->mode == "r") {
		file.open(file_name, std::fstream::in | std::fstream::binary);
	}
	else {
		file.open(file_name, std::fstream::out | std::fstream::binary);	
	}
}

Bitstream::Bitstream() {
}

Bitstream::~Bitstream() {

}

void Bitstream::close() {	
	file.close();
}

void Bitstream::writeBit(int value) {
	// If at the start of the buffer, make pos = 1
	if(pos == 0) {
		pos++;
	}
	// If there are conditions to write to the file
	else if(pos == 256) {
		file.write(reinterpret_cast<char*>(buff), sizeof(char));
		pos = 1;
		byte_count ++;
	}
	buff |= (value * pos);
	pos = pos << 1;
}

void Bitstream::writeBits(int value, int n_bits) {
	// Send each bit to buffer, from the most significant to the least significant
	while(n_bits-- > 0)
		writeBit((value >> n_bits) & 0x01);
}

int Bitstream::readBit() {
	if(pos == 0 || pos == 256) {
		file.read(reinterpret_cast<char*>(buff), sizeof(char));
		pos = 1;
	}
	int read_bit = (buff & pos) == 0 ? 0 : 1;
	pos = pos << 1;
	return read_bit;
}

int Bitstream::readBits(int n_bits) {
	int tmp, read_buffer = 0;
	while(n_bits-- > 0) {
		tmp = readBit();
		read_buffer = (read_buffer << 1) | tmp;
	}

	return read_buffer;
}

void Bitstream::writeFile(encoded_data_t golomb_encoded, SF_INFO snd_info, int order) {
	/* TODO */
}

encoded_data_t Bitstream::readFile(SF_INFO *new_snd_info, int *dec_order, int *new_m) {
	encoded_data_t result;
	/* TODO */
	return result;
}