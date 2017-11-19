#include <iostream>
#include <sndfile.h>
#include <fstream>
#include <cmath>
#include "structures.h"
#include <climits>

typedef std::vector< uint > channel_transform_t;
typedef std::vector< channel_transform_t > golomb_transform_t;
class Golomb {
	public:
		Golomb();
		/**
		 * @brief      Encode the values at residuals
		 *
		 * @param[in]  residuals  The residuals values from a predictor
		 * @param      out        The encoded values will be stored here
		 *
		 * @return     { the expected size for the encoded format, in bytes }
		 */
		uint real_encode(audio_data_t const &residuals, encoded_data_t *out);
		/**
		 * @brief      Calculate the best m to use in this dataset; It also fills the transf_data with the corrected values for golomb coding (only positive values)
		 *
		 * @param      residuals    The residuals
		 * @param      transf_data  The transformed data to use in golomb coding
		 *
		 * @return     The best m to use
		 */
		ushort find_m(audio_data_t const &residuals, golomb_transform_t &transf_data);
		ushort get_m(){return m;};
		/**
		 * @brief      Decode the vaules at encoded with the m found when encoding
		 *
		 * @param[in]  encoded  The encoded values
		 * @param      decoded  Where the decoded values will be stored
		 */
		void real_decode(encoded_data_t const &encoded, audio_data_t *decoded);
		/**
		 * @brief      Decode the vaules at encoded
		 *
		 * @param      encoded  The encoded values
		 * @param      decoded  Where the decoded values will be stored
		 * @param[in]  new_m    The new m to be used
		 */
		void real_decode(encoded_data_t const &encoded, audio_data_t *decoded, ushort new_m);
	private:
		ushort m;
		ushort b;
};