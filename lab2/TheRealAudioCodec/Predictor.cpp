#include "Predictor.h"

Predictor::Predictor() {
}
/*
void Predictor::order1_predict(short* buff) {

	short rem_L;
	short rem_R;

	rem_L = buff[2] - buff[0];
	rem_R = buff[3] - buff[1];


	// In the future this will be Golomb and streams
	residuals[0].push_back(rem_L);
	residuals[1].push_back(rem_R);
}
*/

audio_data_t Predictor::predict(audio_data_t values, int *pred_order) {
	short rem;
	audio_data_t tmp;
	AudioEntropy *ae = new AudioEntropy(values);
	tmp = audio_data_t(values);
	double prev_entropy = ae->entropy();
	double current_entropy = -1.0;
	std::cout << "Original entropy: " << prev_entropy << "\n";
	short order = 1;
	bool next_order = true;
	while(next_order) 
	{
		for (unsigned int k = 0; k < values.size(); ++k) 
		{
			tmp[k][0]= values[k][0] - 0;
			for (unsigned int i = 0; i < values[k].size()-1; ++i)
			{
				rem = values[k][i+1] - values[k][i];
				tmp[k][i+1] = rem;
			}

		}
		ae = new AudioEntropy(tmp);
		current_entropy = ae->entropy();
		if(current_entropy >= prev_entropy) {
			next_order = false;
			*pred_order = order-1;
			std::cout << "Chosen order: " << *pred_order << "\n";
		}
		else {
			prev_entropy = current_entropy;
			values = tmp;
			order++;
			std::cout << "order " << order-1 << " entropy: " << prev_entropy << "\n";
		}
		
	}
	return values;
}

audio_data_t Predictor::reverse(audio_data_t residuals, int order) {
	for(int i = order; i > 0; i--) {
		for(uint k = 0; k < residuals.size(); ++k) {
			for(uint i = 0; i < residuals[k].size()-1; ++i) {
				residuals[k][i+1] = residuals[k][i+1] + residuals[k][i];
			}
		}
		
	}
	return residuals;
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
