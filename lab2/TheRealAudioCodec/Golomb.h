#include <iostream>
#include <fstream>
#include "structures.h"
#include <cmath>

class Golomb {
	public:
		Golomb();
		Golomb(int m);
		void encode(audio_data_t residuals);
		encoded_data_t real_encode(audio_data_t residuals);
		audio_data_t decode();
		audio_data_t real_decode(encoded_data_t values);
		void f_write(std::string file_name);
	private:
		int m;
		int b;
		std::ofstream out;
		
};