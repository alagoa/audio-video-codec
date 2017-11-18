#include <iostream>
#include <sndfile.h>
#include <fstream>
#include "structures.h"
#include <cmath>

class Golomb {
	public:
		Golomb();
		Golomb(int m);
		void encode(audio_data_t residuals, SF_INFO snd_info, int order);
		audio_data_t decode(SF_INFO *snd_info, int *order);
		encoded_data_t real_encode(audio_data_t residuals);
		audio_data_t real_decode(encoded_data_t encoded, int new_m);
		void f_write(std::string file_name);
	private:
		int m;
		int b;
		std::ofstream out;
		
};