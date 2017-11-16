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

std::vector<std::vector<short>> Predictor::predict(std::vector<std::vector<short>> values) {
	short rem;
	std::vector<std::vector<short>> tmp;
	AudioEntropy *ae = new AudioEntropy(values);
	tmp = std::vector<std::vector<short>>(values);
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
			for (int i = 0; i < values[k].size()-1; ++i)
			{
				rem = values[k][i+1] - values[k][i];
				tmp[k][i+1] = rem;
			}

		}
		ae = new AudioEntropy(tmp);
		current_entropy = ae->entropy();
		if(current_entropy >= prev_entropy) {
			next_order = false;
			std::vector<short> order_vector;
			order_vector.push_back(order-1);
			values.push_back(order_vector);
			std::cout << "Chosen order: " << order-1 << "\n";
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

std::vector<std::vector<short>> Predictor::reverse(std::vector<std::vector<short>> residuals) {
	short order = residuals[residuals.size()-1][0];
	for(int i = order; i > 0; i--) {
		for(int k = 0; k < residuals.size()-1; ++k) {
			for(int i = 0; i < residuals[k].size()-1; ++i) {
				residuals[k][i+1] = residuals[k][i+1] + residuals[k][i];
			}
		}
		
	}
	return residuals;
}


std::vector<std::vector<short>> Predictor::order1_predict(std::vector<std::vector<short>> values) {
	short rem;
	std::vector<std::vector<short>> residuals;
	for (unsigned int k = 0; k < values.size(); ++k)// = values.begin(); i != values.end(); ++i)
	{
	//residuals[k] = std::vector<short>(values[k].size());
		residuals[k][0]= values[k][0] - 0;
		for (int i = 0; i < values[k].size()-1; ++i)
		{
			rem = values[k][i+1] - values[k][i];
			residuals[k][i+1] = rem;
		}
	}
	return residuals;
}

	
std::vector<std::vector<short>> Predictor::order1_reverse(std::vector<std::vector<short>> residuals) {
	std::vector<std::vector<short>> values;
	for(unsigned int k = 0; k < residuals.size(); ++k)
	{
		values.push_back(std::vector<short>(residuals[k].size()));
		values[k][0] = residuals[k][0];
		for(int i = 0; i < residuals[k].size()-1; ++i)
		{
			values[k][i+1] = residuals[k][i+1] + values[k][i];
		}
	}
	return values;
}
