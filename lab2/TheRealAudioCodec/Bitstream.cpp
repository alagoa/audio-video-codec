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

void Bitstream::writeFile_blocks(encoded_data_t const &golomb_encoded, 
								 SF_INFO snd_info,
								 block_data_t const &b_data, 
								 ushort block_size)
{
	//write header
	file.write((char*) &snd_info, sizeof(snd_info));
	file.write((char*) &block_size, sizeof(block_size));
	for (int chan = 0; chan < snd_info.channels; ++chan)
	{
		encoded_channel_t::const_iterator encoded_begin = golomb_encoded[chan].begin();
		encoded_channel_t::const_iterator encoded_end = golomb_encoded[chan].end();
		for (uint b = 0; b < b_data[chan].size(); ++b, encoded_begin += block_size){
			write_block(encoded_begin, encoded_end, block_size, b_data[chan][b]);
		}
	}
}
void Bitstream::write_block(encoded_channel_t::const_iterator data_p,
							encoded_channel_t::const_iterator data_end,
							ushort block_size,
							block_header const &b_header)
{
	const encoded_channel_t::const_iterator data_init = data_p;
	int b = std::log2(b_header.m);
	write_r(b_header.order, 16);
	write_r(b_header.m, 16);
	write_r(b_header.size, 16);
	for (; data_p < (data_init + block_size) && data_p != data_end; ++data_p)
	{
		write_unary(data_p->first);
		write_r(data_p->second, b);
	}
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

encoded_data_t Bitstream::readFile_blocks(SF_INFO *snd_info,
								          block_data_t *b_data)
{
	encoded_data_t result;
	ushort block_size;
	ushort default_block_size;
	file.read((char*)snd_info, sizeof(SF_INFO));
	file.read((char*) &default_block_size, sizeof(default_block_size));
	result.reserve(snd_info->channels);
	b_data->reserve(snd_info->channels);
	for (int chan = 0; chan < snd_info->channels; ++chan){
		block_size = default_block_size;
		result.emplace_back();
		b_data->emplace_back();
		result.back().reserve(snd_info->frames);
		b_data->back().reserve((snd_info->frames/block_size) + 1);
		double num_b = (double)snd_info->frames / (double)block_size;
		for (uint block = 0; block < ceil(num_b) ; ++block){
			block_header b_header;
			b_header.order = (short)read_r(16);
			b_header.m = (short)read_r(16);
			b_header.size = (ushort)read_r(16);
			block_size = b_header.size;
			int b = std::log2(b_header.m);
			b_data->back().push_back(b_header);
			for (uint i = 0; i < block_size; ++i)
			{
				uint q = read_unary();
				uint r = read_r(b);
				result.back().emplace_back(q,r);
			}
		}
	}
	return result;
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
	result.reserve(new_snd_info->channels);
	for (int chan = 0; chan < new_snd_info->channels; ++chan)
	{
		result.emplace_back();
		result.back().reserve(new_snd_info->frames);
		for (int i = 0; i < new_snd_info->frames; ++i)
		{
			q = read_unary();
			r = read_r(b);
			result.back().emplace_back(q,r);
		}
	}
	return result;
}