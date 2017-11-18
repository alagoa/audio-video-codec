#include <iostream>
#include <fstream>
#include "structures.h"
#include <cmath>

class Golomb {
	public:
		Golomb();
		Golomb(int m);
		uint encode_to_file(audio_data_t const &residuals, short predictor_order);
		/**
		 * @brief      Encode the values at residuals
		 *
		 * @param[in]  residuals  The residuals values from a predictor
		 * @param      out        The encoded values will be stored here
		 *
		 * @return     { the expected size for the encoded format, in bytes }
		 */
		uint encode(audio_data_t const &residuals, encoded_data_t *out);
		/**
		 * @brief      Decode encoded.cod file
		 *
		 * @param      decoded  The decoded values
		 *
		 * @return     { the order of the predictor used }
		 */
		short decode_file(audio_data_t *decoded);
		/**
		 * @brief      Decode the vaules at encoded
		 *
		 * @param[in]  encoded  The encoded values
		 * @param      decoded  Where the decoded values will be stored
		 *
		 * @return     { the total number of itens decoded (channels * frames) }
		 */
		uint decode(encoded_data_t const &encoded, audio_data_t *decoded);
		void f_write(std::string file_name);
	private:
		int m;
		int b;
		std::ofstream out;
		
};