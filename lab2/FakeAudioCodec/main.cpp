#include "AudioReader.h"
#include "Predictor.h"
#include "Golomb.h"

int main(int argc, char const *argv[])
{


	AudioReader *ar = new AudioReader(argv[1]);
	audio_data_t values = ar->get_values();
	delete ar;

	/*
	std::vector<short> channel1 = {5, 10, 15, 20, 25, 30, 35, 40};
	std::vector<short> channel2 = {5, 15, 25, 35, 45, 55, 65, 75};

	std::vector<std::vector<short>> values;
	values.push_back(channel1);
	values.push_back(channel2);
*/
	Predictor predictor;
	Golomb golomb(32);
	golomb.encoded(predictor.predict(values));
	/*
	std::vector<std::vector<short>> original = predictor.order1_reverse(golomb.decode());
	
	for(auto &e : original) {
		std::cout << "channel-> ";
		for(auto &v : e) {
			std::cout << v << ", ";
		}
		std::cout << "\n";
	}

	*/
}