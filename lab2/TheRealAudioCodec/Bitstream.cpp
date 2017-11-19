#include "Bitstream.h"
#define BIT_SIZE(t) (sizeof(t) * 8)

Bitstream::Bitstream(std::string file_name, std::string mode) {
	this->w_buff_pos = 7;
	this->w_buff = 0;
	this->r_buff = 0;
	this->r_buff_pos = -1;
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
	if (mode != "r")
	{
		file.write(&w_buff, sizeof(char));
	}	
	file.close();
}

int Bitstream::writeBit(char value) {
	w_buff |= (value & 0x01) << w_buff_pos;
	w_buff_pos--;
	if(w_buff_pos >= 0)
		return 0;
	//ja se pode escrever 1 byte
	file.write(&w_buff, sizeof(char));
	w_buff = 0;
	w_buff_pos = 7;
	byte_count++;
	return 1;
}

int Bitstream::write_unary(uint unary){
	int num_bytes_escritos = 0;
	for (uint i = 0; i < unary; ++i)
	{
		num_bytes_escritos += writeBit(1);
	}
	num_bytes_escritos += writeBit(0);
	//num de bits por escrever;
	return (unary + 1) - (num_bytes_escritos * 8);
}

int Bitstream::write_r(uint r, int b){
	int num_bytes_escritos = 0;
	for (int i = 1; i <= b ; ++i)
	{
		num_bytes_escritos += writeBit(r >> (b - i));
	}
	return b - (num_bytes_escritos * 8);
}

char Bitstream::readBit() {
	if(r_buff_pos < 0) {
		file.read(&r_buff, sizeof(char));
		r_buff_pos = 7;
	}
	char read_bit = (r_buff >> r_buff_pos) & 0x01;
	r_buff_pos--;
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

uint Bitstream::read_unary(){
	uint num = 0;
	while(readBit())
		num++;
	return num;
}

uint Bitstream::read_r(int b){
	uint r = 0;
	for (int i = 1; i <= b; ++i)
	{
		r |= readBit() << (b - i);
	}
	return r;
}

void Bitstream::writeFile(encoded_data_t const &golomb_encoded, SF_INFO snd_info, ushort order, ushort m) {
	//ushort unary;
	int b = std::log2(m);
	//write header
	file.write((char*) &snd_info, sizeof(snd_info));
	file.write((char*) &order, sizeof(order));
	file.write((char*) &m, sizeof(m));
	for (int chan = 0; chan < snd_info.channels; ++chan)
	{
		for (int i = 0; i < snd_info.frames; ++i)
		{
			write_unary(golomb_encoded[chan][i].first);
			write_r(golomb_encoded[chan][i].second, b);
		}
	}
}
	
encoded_data_t Bitstream::readFile(SF_INFO *new_snd_info, ushort *dec_order, ushort *new_m) {
	encoded_data_t result;
	uint q, r;
	int b;
	//read header
	file.read((char*)new_snd_info, sizeof(SF_INFO));
	file.read((char*)dec_order, sizeof(ushort));
	file.read((char*)new_m, sizeof(ushort));
	b = std::log2(*new_m);
	for (int chan = 0; chan < new_snd_info->channels; ++chan)
	{
		result.push_back(encoded_channel_t());
		for (int i = 0; i < new_snd_info->frames; ++i)
		{
			q = read_unary();
			r = read_r(b);
			result.back().push_back(std::make_pair(q,r));
		}
	}
	return result;
}