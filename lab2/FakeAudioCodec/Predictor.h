#include <iostream>
#include <fstream>
#include <string>
#include <istream>

class Predictor {
	public:
		Predictor();
		void order1_predict(short* seq, short* buff);
		void order1_reverse(short* samples, short* buff);

	private:
		std::vector<std::vector<short>> residuals;
}