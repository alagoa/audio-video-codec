#include "AudioReader.h"
#include "Predictor.h"
#include "Golomb.h"

int main(int argc, char const *argv[])
{

	//AudioReader ar(argv[1]);
	//audio_data_t original = ar.get_values();
	std::vector<short> channel1 = {5, 10, 15, 20, 25, 30, 35, 40};
	std::vector<short> channel2 = {5, 15, 25, 35, 45, 55, 65, 75};

	std::vector<std::vector<short>> values;
	values.push_back(channel1);
	values.push_back(channel2);

	Predictor predictor;
	Golomb golomb;
	golomb.encode(predictor.order1_predict(values));
}