#include <iostream>
#include <fstream>
#include "structures.h"
#include <cmath>

class Golomb {
	public:
		Golomb();
		Golomb(int m);
		uint encode(audio_data_t residuals);
		audio_data_t decode();
		void f_write(std::string file_name);
	private:
		int m;
		int b;
		std::ofstream out;
		
};