#include <iostream>
#include <fstream>
#include <string>
#include <istream>
#include "structures.h"
#include "AudioEntropy.h"

class Predictor {
	public:
		Predictor();
		/**
		 * @brief      Use the predictor to optimize the values (smaller)
		 *
		 * @param      values  The array to be optimized with the predictor, this values will be changed
		 *
		 * @return     the order in which the entropy as smaller
		 */
		short predict(audio_data_t &values);
		/**
		 * @brief      Reverse the residual values
		 *
		 * @param      residuals  The residuals values, this vector will be changed and the reversed values will be stored here
		 * @param[in]  order      The order used in the predictor
		 */
		void reverse(audio_data_t &residuals, short order);
		audio_data_t order1_predict(audio_data_t values); 
		audio_data_t order1_reverse(audio_data_t residuals);

	private:
};