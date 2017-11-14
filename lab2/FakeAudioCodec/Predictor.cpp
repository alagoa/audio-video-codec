#include "Predictor.h"

Predictor::Predictor() {

}

Predictor::order1_predict(short* seq, short* buff) {

	short rem_L;
	short rem_R;

	rem_L = seq[0] - buff[0];
	rem_R = seq[1] - buff[1];


	// In the future this will be Golomb and streams
	residuals(0).push_back(rem_L);
	residuals(1).push_back(rem_R);

}

Predictor::order1_reverse(short* samples, short* buff) {
	
	short original_L;
	short original_R;

}