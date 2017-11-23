#include "Golomb.h"

Golomb::Golomb(){
	this->m = 0;
	this->b = 0;
}

uint Golomb::real_encode(audio_data_t const &residuals, encoded_data_t *out) {
	uint q,r;
	uint final_size;
	golomb_transform_t tranformed_data;
	final_size = find_m(residuals, tranformed_data);
	out->reserve(residuals.size());
	for(auto &e : tranformed_data) {
		out->emplace_back();
		out->back().reserve(residuals[0].size());
		for(auto &v : e) {
			q = v >> b;
			r = v % m;
			out->back().emplace_back(q, r);
		 }
	}
	return final_size;
}

ushort Golomb::find_m(audio_data_t const &residuals, golomb_transform_t &transf_data){
	uint q,transf, final_size, original, min_size = UINT_MAX;
	ushort tmp_m;
	original = residuals.size() * residuals[0].size() * sizeof(short);
	final_size = 0;
	tmp_m = 2;
	b = 1;
	std::cout << "Original size = " << original << "\n";
	std::cout << "\tTrying m = " << tmp_m << "...\n";
	transf_data.reserve(residuals.size());
	for(auto &e : residuals) {
		transf_data.emplace_back();
		transf_data.back().reserve(e.size());
		for(auto &v : e) {
			transf = v >= 0 ? 2*v : (2*(-v))-1;
			q = transf / tmp_m;
			final_size += ((q+1) + b);
			transf_data.back().emplace_back(transf);
		 }
	}
	final_size = final_size/8;
	std::cout << "\t\tsize for current m = " << final_size << "\n";
	final_size = 0;
	tmp_m <<= 1;
	b++;

	while(tmp_m < std::pow(2, 15)) {
		std::cout << "\tTrying m = " << tmp_m << "...\n";
		//add b for each value
		final_size += b * transf_data.size() * transf_data[0].size();
		//add 1 as in (q+1) for each value
		final_size += transf_data.size() * transf_data[0].size();
		//if it is already bigger we can discard this b
		if (final_size/8 > min_size)
		{
			final_size = 0;
			tmp_m <<= 1;
			b++;
			continue;
		}
		for(auto &e : transf_data) {
			for(auto &v : e) {
				q = v / tmp_m;
				final_size += ((q));
			 }
		}
		final_size = final_size/8;
		std::cout << "\t\tsize for current m = " << final_size << "\n";
		if(final_size < min_size ) {
			min_size = final_size;
			m = tmp_m;
		}
		final_size = 0;
		tmp_m <<= 1;
		b++;
	}
	std::cout << "chosen m -> " << m << "\n";
	b = std::log2(m);
	return min_size;
}

void Golomb::real_decode(encoded_data_t const &encoded, audio_data_t *decoded) {
	if (m == 0)
		return;
	int num;
	int num_chan = encoded.size();
	uint q, r;
	uint frames_per_chan = encoded[0].size();
	decoded->reserve(num_chan);
	for (int i = 0; i < num_chan; ++i)
	{
		decoded->push_back(channel_data_t());
		decoded->back().reserve(frames_per_chan);
		for (int k = 0; k < frames_per_chan; ++k)
		{
	 		q = encoded[i][k].first;
	 		r = encoded[i][k].second;
	 		num = q*m + r;
	 		num = num%2 == 0 ? num/2 : ((num/-2)-1);
	 		decoded->back().push_back(num);
		}
	}
}

void Golomb::real_decode(encoded_data_t const &encoded, audio_data_t *decoded, ushort new_m) {
	int num;
	int num_chan = encoded.size();
	int q, r;
	uint frames_per_chan = encoded[0].size();
	decoded->reserve(num_chan);
	for (int i = 0; i < num_chan; ++i)
	{
		decoded->emplace_back();
		decoded->back().reserve(frames_per_chan);
		for (int k = 0; k < frames_per_chan; ++k)
		{
	 		q = encoded[i][k].first;
	 		r = encoded[i][k].second;
	 		num = q * new_m + r;
	 		num = num%2 == 0 ? num/2 : ((num/-2)-1);
	 		decoded->back().push_back(num);
		}
	}
}

