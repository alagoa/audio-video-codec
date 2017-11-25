#include "Predictor.h"

Predictor::Predictor() {
}

void Predictor::predict_blocks(audio_data_t &values, block_data_t &b_data, short order, ushort block_size){
	AudioEntropy *ae = new AudioEntropy(values);
	std::cout << "Original entropy: " << ae->entropy() << "\n";
	delete ae;
	predict_inter_channel(values);
	b_data.reserve(values.size());
	//short order;
	for (uint chan = 0; chan < values.size(); ++chan)
	{
		channel_data_t::iterator chan_begin = values[chan].begin();
		channel_data_t::iterator chan_end = values[chan].end();
		b_data.emplace_back();
		b_data.back().reserve((values[chan].size() / block_size)+1);
		for (; chan_begin != chan_end;)
		{
			block_header b_dat = predict_single_block(chan_begin, chan_end, block_size, order);
			b_data[chan].push_back(b_dat);
		}
	}
	ae = new AudioEntropy(values);
	std::cout << "Final entropy: " << ae->entropy() << "\n";
	delete ae;
}

block_header Predictor::predict_single_block(channel_data_t::iterator &data_p, 
	 								  channel_data_t::iterator data_end,
	 								  uint block_size, short order)
{
	const channel_data_t::iterator init = data_p;
	ushort s = (ushort)block_size;
	//#define ORDER_ 2
	for (int o = 0; o < order; ++o)
	{
		data_p = init;
		int last = *data_p;
		for (s = 0; data_p < (init + block_size) - 1 && data_p != data_end-1; ++data_p, ++s) 
		{
			int conta = *(data_p + 1) - last;
			last = *(data_p + 1);
			*(data_p + 1) = conta;
		}
	}
	data_p++;
	return block_header{order, 0, (ushort)s+1};
}

void Predictor::reverse_blocks(audio_data_t &values, block_data_t const &b_data){
	for (uint chan = 0; chan < values.size(); ++chan)
	{
		channel_data_t::iterator chan_begin = values[chan].begin();
		channel_data_t::iterator chan_end = values[chan].end();
		for (uint i = 0; chan_begin != chan_end; i++)
		{
			ushort order = b_data[chan][i].order;
			reverse_single_block(chan_begin, chan_end, b_data[chan][i].size, order);
		}
	}
	reverse_inter_channel(values);
}

void Predictor::reverse_single_block(channel_data_t::iterator &data_p, 
	 channel_data_t::iterator data_end, uint block_size, short order)
{
	const channel_data_t::iterator init = data_p;
	for (int o = 0; o < order; ++o)
	{
		data_p = init;
		for (; data_p < (init + block_size) - 1 && data_p != data_end-1; ++data_p) 
		{
			*(data_p + 1) = *(data_p + 1) + *data_p;
		}
	}
	data_p++;
	return;
}
void Predictor::predict_inter_channel(audio_data_t &values){
	if (values.size() < 2)
	{
		return;
	}
	//audio_data_t tmp;
	for (uint chan = 1; chan < values.size(); ++chan)
	{
		channel_data_t::iterator main = values[0].begin();
		for (channel_data_t::iterator i = values[chan].begin(); i != values[chan].end(); ++i)
		{
			*i = *i - *main;
			++main; 
		}
	}
}

void Predictor::reverse_inter_channel(audio_data_t &values){
	if (values.size() < 2)
	{
		return;
	}
	for (uint chan = 1; chan < values.size(); ++chan)
	{
		channel_data_t::iterator main = values[0].begin();
		for (channel_data_t::iterator i = values[chan].begin(); i != values[chan].end(); ++i)
		{
			*i = *i + *main;
			++main;
		}
	}
}

short Predictor::predict(audio_data_t &values) {
	int rem;
	audio_data_t tmp;
	short order = 1;
	bool next_order = true;
	double prev_entropy;
	double current_entropy = -1.0;
	AudioEntropy *ae = new AudioEntropy(values);
	tmp = audio_data_t(values);
	prev_entropy = ae->entropy();
	std::cout << "Original entropy: " << prev_entropy << "\n";
	while(next_order) 
	{
		for (uint k = 0; k < values.size(); ++k) 
		{
			tmp[k][0] = values[k][0] - 0;
			for (uint i = 0; i < values[k].size()-1; ++i)
			{
				rem = values[k][i+1] - values[k][i];
				tmp[k][i+1] = rem;
			}
		}
		ae = new AudioEntropy(tmp);
		current_entropy = ae->entropy();
		if(current_entropy >= prev_entropy) {
			next_order = false;
			std::cout << "Chosen order: " << order-1 << "\n";
		}
		else {
			prev_entropy = current_entropy;
			values = tmp;
			order++;
			std::cout << "order " << order-1 << " entropy: " << prev_entropy << "\n";
		}
	}
	predict_inter_channel(values);
	return order-1;
}

void Predictor::reverse(audio_data_t &residuals, short order) {
	reverse_inter_channel(residuals);
	for(short o = order; o > 0; o--) {
		for(uint k = 0; k < residuals.size(); ++k) {
			for(uint i = 0; i < residuals[k].size()-1; ++i) {
				residuals[k][i+1] = residuals[k][i+1] + residuals[k][i];
			}
		}
	}
}


audio_data_t Predictor::order1_predict(audio_data_t values) {
	short rem;
	audio_data_t residuals;
	for (unsigned int k = 0; k < values.size(); ++k)// = values.begin(); i != values.end(); ++i)
	{
	//residuals[k] = std::vector<short>(values[k].size());
		residuals[k][0]= values[k][0] - 0;
		for (uint i = 0; i < values[k].size()-1; ++i)
		{
			rem = values[k][i+1] - values[k][i];
			residuals[k][i+1] = rem;
		}
	}
	return residuals;
}

	
audio_data_t Predictor::order1_reverse(audio_data_t residuals) {
	audio_data_t values;
	for(unsigned int k = 0; k < residuals.size(); ++k)
	{
		values.push_back(channel_data_t(residuals[k].size()));
		values[k][0] = residuals[k][0];
		for(uint i = 0; i < residuals[k].size()-1; ++i)
		{
			values[k][i+1] = residuals[k][i+1] + values[k][i];
		}
	}
	return values;
}
