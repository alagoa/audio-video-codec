#include <iostream>
#include <fstream>
#include <string>
#include <istream>
#include "structures.h"
#include "AudioEntropy.h"

class Predictor {
	public:
		Predictor();
		///void order1_predict(short* buff);
		audio_data_t predict(audio_data_t values, int *pred_order);
		audio_data_t reverse(audio_data_t residuals, int order);
		audio_data_t order1_predict(audio_data_t values); 
		audio_data_t order1_reverse(audio_data_t residuals);

	private:
};