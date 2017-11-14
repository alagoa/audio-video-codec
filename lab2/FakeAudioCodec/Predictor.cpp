#include "Predictor.h"

Predictor::Predictor() {

}

void Predictor::order1_predict(short* buff) {

	short rem_L;
	short rem_R;

	rem_L = buff[2] - buff[0];
	rem_R = buff[3] - buff[1];


	// In the future this will be Golomb and streams
	residuals[0].push_back(rem_L);
	residuals[1].push_back(rem_R);
}

std::vector<std::vector<short>> Predictor::order1_predict(std::vector<std::vector<short>> values) {
	short rem;
	for (unsigned int k = 0; k < values.size(); ++k)// = values.begin(); i != values.end(); ++i)
	{
		residuals.push_back(std::vector<short>(values[k].size()));
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

void Predictor::order1_reverse(short* samples, short* buff) {
	
	short original_L;
	short original_R;

}