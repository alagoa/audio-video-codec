#include <iostream>
#include <fstream>
#include <string>
#include <istream>
#include <vector>
#include "AudioEntropy.h"

class Predictor {
	public:
		Predictor();
		///void order1_predict(short* buff);
		std::vector<std::vector<short>> predict(std::vector<std::vector<short>> values);
		std::vector<std::vector<short>> reverse(std::vector<std::vector<short>> residuals, int order);
		std::vector<std::vector<short>> order1_predict(std::vector<std::vector<short>> values); 
		std::vector<std::vector<short>> order1_reverse(std::vector<std::vector<short>> residuals);

	private:
};