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
		 * @return     The final size of the encoded data
		 */
		ushort find_m(audio_data_t const &residuals, golomb_transform_t &transf_data);
		ushort find_block_m(channel_data_t::iterator data_p, 
	 					   channel_data_t::iterator data_end,
	 					   uint block_size);
		ushort get_m(){return m;};
		/**
		 * @brief      Decode the vaules at encoded with the m found when encoding
		 *
		 * @param[in]  encoded  The encoded values
		 * @param      decoded  Where the decoded values will be stored
		 */
		void real_decode(encoded_data_t const &encoded, audio_data_t *decoded);
		void encode_blocks(audio_data_t const &residuals, block_data_t &b_data, encoded_data_t *out);
		void encode_block(channel_data_t::iterator data_p, 
	 					  		  channel_data_t::iterator data_end,
	 					  		  encoded_channel_t::iterator encoded_begin,
	 					  		  encoded_channel_t::iterator encoded_end,
	 					  		  uint block_size,
	 					  		  ushort m_block);
		/**
		 * @brief      Decode the vaules at encoded
		 *
		 * @param      encoded  The encoded values
		 * @param      decoded  Where the decoded values will be stored
		 * @param[in]  new_m    The new m to be used
		 */
		void real_decode(encoded_data_t const &encoded, audio_data_t *decoded, ushort new_m);
		void decode_block(encoded_channel_t::const_iterator encoded_begin,
						  encoded_channel_t::const_iterator encoded_end,
						  channel_data_t::iterator decoded_begin,
						  channel_data_t::iterator decoded_end,
						  uint block_size,
						  ushort m_block);
		void decode_blocks(encoded_data_t const &encoded,
						   block_data_t &b_data, 
						   audio_data_t *decoded);
	private:
		ushort m;
		ushort b;
};