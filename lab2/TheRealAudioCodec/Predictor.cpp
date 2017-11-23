#include "Predictor.h"

Predictor::Predictor() {
}

void Predictor::predict_inter_channel(audio_data_t &values){
	if (values.size() < 2)
	{
		return;
	}
	//audio_data_t tmp;
	for (int chan = 1; chan < values.size(); ++chan)
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
	for (int chan = 1; chan < values.size(); ++chan)
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
