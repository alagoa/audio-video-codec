#include "Golomb.h"

Golomb::Golomb(){
	this->m = 0;
	this->b = 0;
}


void Golomb::encode_blocks(audio_data_t const &residuals, block_data_t &b_data, encoded_data_t *out){
	audio_data_t transf;
	transf.reserve(residuals.size());
	ushort m_block; 
	out->reserve(residuals.size());
	for (uint chan = 0; chan < residuals.size(); ++chan)
	{
		out->emplace_back(residuals[chan].size());
		transf.emplace_back();
		transf.back().reserve(residuals[chan].size());
		for (int i = 0; i < residuals[chan].size(); ++i)
		{
			transf[chan].push_back(residuals[chan][i] >= 0 ? 2*residuals[chan][i] : (2*(-residuals[chan][i]))-1);
		}
		channel_data_t::iterator chan_begin = transf[chan].begin();
		channel_data_t::iterator chan_end = transf[chan].end();
		encoded_channel_t::iterator encoded_begin = (*out).back().begin();
		encoded_channel_t::iterator encoded_end = (*out).back().end();
		for (uint b = 0; b < b_data[chan].size(); ++b, chan_begin += BLOCK_SIZE, encoded_begin+=BLOCK_SIZE)
		{
			m_block = find_block_m(chan_begin, chan_end, BLOCK_SIZE);
			b_data[chan][b].m = m_block;
			encode_block(chan_begin, chan_end, encoded_begin, encoded_end, BLOCK_SIZE, m_block);
		}
	}
}

void Golomb::encode_block(channel_data_t::iterator data_p, 
	 					  channel_data_t::iterator data_end,
	 					  encoded_channel_t::iterator encoded_begin,
	 					  encoded_channel_t::iterator encoded_end,
	 					  uint block_size,
	 					  ushort m_block)
{
	const channel_data_t::iterator init_data = data_p;
	const encoded_channel_t::iterator init_encoded = encoded_begin;
	for (; data_p < (init_data + block_size) && data_p != data_end && encoded_begin != encoded_end; ++data_p, ++encoded_begin){
		uint q = *data_p / m_block;
		uint r = *data_p % m_block;
		*encoded_begin = std::make_pair(q, r);
	}
}

ushort Golomb::find_block_m(channel_data_t::iterator data_p, 
	 					   channel_data_t::iterator data_end,
	 					   uint block_size)
{
	const channel_data_t::iterator init_data = data_p;
	ushort tmp_m = 2;
	ushort b = 1;
	uint final_size = 0;
	uint min_size = UINT_MAX;
	while(tmp_m < std::pow(2, 15)) {
		std::cout << "\tTrying m = " << tmp_m << "...\n";
		//add b for each value
		final_size += b * BLOCK_SIZE;
		//add 1 as in (q+1) for each value
		final_size += BLOCK_SIZE;
		//if it is already bigger we can discard this b
		if (final_size/8 > min_size)
		{
			data_p = init_data;
			final_size = 0;
			tmp_m <<= 1;
			b++;
			continue;
		}
		for(; data_p < (init_data + block_size) && data_p != data_end; ++data_p) {
			final_size += *data_p / tmp_m;
		}
		final_size = final_size/8;
		std::cout << "\t\tsize for current m = " << final_size << "\n";
		if(final_size < min_size) {
			min_size = final_size;
			m = tmp_m;
		}
		data_p = init_data;
		final_size = 0;
		tmp_m <<= 1;
		b++;
	}
	std::cout << "chosen m -> " << m << "\n";
	b = std::log2(m);
	return m;
	//for (; data_p < (init + block_size) - 1 && data_p != data_end-1; ++data_p, ++encoded_begin) 
}

uint Golomb::real_encode(audio_data_t const &residuals, encoded_data_t *out) {
	uint q,r;
	uint final_size;
	golomb_transform_t tranformed_data;
	final_size = find_m(residuals, tranformed_data);
	out->reserve(residuals.size());
	for(auto &e : tranformed_data) {
		out->emplace_back();
		out->back().reserve(residuals[0].size());
		for(auto &v : e) {
			q = v >> b;
			r = v % m;
			out->back().emplace_back(q, r);
		 }
	}
	return final_size;
}

ushort Golomb::find_m(audio_data_t const &residuals, golomb_transform_t &transf_data){
	uint q,transf, final_size, original, min_size = UINT_MAX;
	ushort tmp_m;
	original = residuals.size() * residuals[0].size() * sizeof(short);
	final_size = 0;
	tmp_m = 2;
	b = 1;
	std::cout << "Original size = " << original << "\n";
	std::cout << "\tTrying m = " << tmp_m << "...\n";
	transf_data.reserve(residuals.size());
	for(auto &e : residuals) {
		transf_data.emplace_back();
		transf_data.back().reserve(e.size());
		for(auto &v : e) {
			transf = v >= 0 ? 2*v : (2*(-v))-1;
			q = transf / tmp_m;
			final_size += ((q+1) + b);
			transf_data.back().emplace_back(transf);
		 }
	}
	final_size = final_size/8;
	std::cout << "\t\tsize for current m = " << final_size << "\n";
	final_size = 0;
	tmp_m <<= 1;
	b++;

	while(tmp_m < std::pow(2, 15)) {
		std::cout << "\tTrying m = " << tmp_m << "...\n";
		//add b for each value
		final_size += b * transf_data.size() * transf_data[0].size();
		//add 1 as in (q+1) for each value
		final_size += transf_data.size() * transf_data[0].size();
		//if it is already bigger we can discard this b
		if (final_size/8 > min_size)
		{
			final_size = 0;
			tmp_m <<= 1;
			b++;
			continue;
		}
		for(auto &e : transf_data) {
			for(auto &v : e) {
				q = v / tmp_m;
				final_size += ((q));
			 }
		}
		final_size = final_size/8;
		std::cout << "\t\tsize for current m = " << final_size << "\n";
		if(final_size < min_size ) {
			min_size = final_size;
			m = tmp_m;
		}
		final_size = 0;
		tmp_m <<= 1;
		b++;
	}
	std::cout << "chosen m -> " << m << "\n";
	b = std::log2(m);
	return min_size;
}

void Golomb::decode_blocks(encoded_data_t const &encoded,
						   block_data_t &b_data, 
						   audio_data_t *decoded)
{
	decoded->reserve(encoded.size());
	for (int chan = 0; chan < encoded.size(); ++chan)
	{
		decoded->emplace_back(encoded[chan].size());
		encoded_channel_t::const_iterator encoded_begin = encoded[chan].begin();
		encoded_channel_t::const_iterator encoded_end = encoded[chan].end();
		channel_data_t::iterator decoded_begin = (*decoded).back().begin();
		channel_data_t::iterator decoded_end = (*decoded).back().end();
		for (uint b = 0; b < b_data[chan].size(); ++b, decoded_begin += BLOCK_SIZE, encoded_begin+=BLOCK_SIZE){
			decode_block(encoded_begin, encoded_end, decoded_begin, decoded_end, BLOCK_SIZE, b_data[chan][b].m);
		}
	}
}	

void Golomb::decode_block(encoded_channel_t::const_iterator encoded_begin,
						  encoded_channel_t::const_iterator encoded_end,
						  channel_data_t::iterator decoded_begin,
						  channel_data_t::iterator decoded_end,
						  uint block_size,
						  ushort m_block)
{
	int num;
	const channel_data_t::iterator init_decoded = decoded_begin;
	const encoded_channel_t::const_iterator init_encoded = encoded_begin;
	for (; encoded_begin < (init_encoded + block_size) && encoded_begin != encoded_end && decoded_begin != decoded_end; ++decoded_begin, ++encoded_begin)
	{
 		uint q = encoded_begin->first;
 		uint r = encoded_begin->second;
 		num = q*m_block + r;
 		num = num%2 == 0 ? num/2 : ((num/-2)-1);
 		*decoded_begin = num;
	}
}

void Golomb::real_decode(encoded_data_t const &encoded, audio_data_t *decoded) {
	if (m == 0)
		return;
	int num;
	int num_chan = encoded.size();
	uint q, r;
	uint frames_per_chan = encoded[0].size();
	decoded->reserve(num_chan);
	for (int i = 0; i < num_chan; ++i)
	{
		decoded->push_back(channel_data_t());
		decoded->back().reserve(frames_per_chan);
		for (int k = 0; k < frames_per_chan; ++k)
		{
	 		q = encoded[i][k].first;
	 		r = encoded[i][k].second;
	 		num = q*m + r;
	 		num = num%2 == 0 ? num/2 : ((num/-2)-1);
	 		decoded->back().push_back(num);
		}
	}
}

void Golomb::real_decode(encoded_data_t const &encoded, audio_data_t *decoded, ushort new_m) {
	int num;
	int num_chan = encoded.size();
	int q, r;
	uint frames_per_chan = encoded[0].size();
	decoded->reserve(num_chan);
	for (int i = 0; i < num_chan; ++i)
	{
		decoded->emplace_back();
		decoded->back().reserve(frames_per_chan);
		for (int k = 0; k < frames_per_chan; ++k)
		{
	 		q = encoded[i][k].first;
	 		r = encoded[i][k].second;
	 		num = q * new_m + r;
	 		num = num%2 == 0 ? num/2 : ((num/-2)-1);
	 		decoded->back().push_back(num);
		}
	}
}

