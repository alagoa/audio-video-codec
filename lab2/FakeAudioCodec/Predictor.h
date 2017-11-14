#include <iostream>
#include <fstream>
#include <string>
#include <istream>
#include <vector>

class Predictor {
	public:
		Predictor();
		void order1_predict(short* buff);
		std::vector<std::vector<short>> order1_predict(std::vector<std::vector<short>> values); 
		void order1_reverse(short* samples, short* buff);

	private:
		std::vector<std::vector<short>> residuals;
};