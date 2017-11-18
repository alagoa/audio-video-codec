#include "Golomb.h"

Golomb::Golomb(){

}

Golomb::Golomb(int m){
	this->m = m;
	this->b = std::log2(m);

}

uint Golomb::encode_to_file(audio_data_t const &residuals, short predictor_order) {
	int q,r,transf;
	uint final_size = 0;
	//init_size = residuals.size() * residuals[0].size() * sizeof(short);
	out.open("encoded.cod", std::ofstream::out);
	out << residuals[0].size() << " " << residuals.size() -1 << " " << predictor_order << "\n";
	for(auto &e : residuals) {
		for(auto &v : e) {
			transf = v >= 0 ? 2*v : (2*std::abs(v))-1;
			q = transf / m;
			r = transf - q*m;
			final_size += (q ) + b;
			out << q << " " << r << "\n";
		 }
		 out << "\n";
	}
	out.close();
	return final_size / 8;
}

uint Golomb::encode(audio_data_t const &residuals, encoded_data_t *out) {
	int q,r,transf;
	uint final_size = 0;
	//init_size = residuals.size() * residuals[0].size() * sizeof(short);
	for(auto &e : residuals) {
		out->push_back(encoded_channel_t());
		for(auto &v : e) {
			transf = v >= 0 ? 2*v : (2*std::abs(v))-1;
			q = transf / m;
			r = transf - q*m;
			final_size += (q + 1) + b;
			out->back().push_back(std::make_pair(q, r));
		 }
	}
	return final_size / 8;
}

short Golomb::decode_file(audio_data_t *decoded) {
	std::ifstream in("encoded.cod");
	//in.open("encoded.cod");
	//char linha[20];
	std::string line;
	short num;
	int num_chan = 0;
	int q, r;
	int frames_per_chan = 0;
	short order = 0;
	in >> frames_per_chan;
	in >> num_chan;
	in >> order;
	for (int i = 0; i < num_chan; ++i)
	{
		decoded->push_back(channel_data_t(frames_per_chan));
		for (int k = 0; k < frames_per_chan; ++k)
		{
	 		in >> q;
	 		in >> r;
	 		num = q * m + r;
	 		num = num % 2 == 0 ? num/2 : ((num/-2)-1);
	 		decoded->back().push_back(num);
		}
	}
	return order;
}

uint Golomb::decode(encoded_data_t const &encoded, audio_data_t *decoded) {
	short num;
	int num_chan = encoded.size();
	int q, r;
	int frames_per_chan = encoded[0].size();
	for (int i = 0; i < num_chan; ++i)
	{
		decoded->push_back(channel_data_t(frames_per_chan));
		for (int k = 0; k < frames_per_chan; ++k)
		{
	 		q = encoded[i][k].first;
	 		r = encoded[i][k].second;
	 		num = q*m + r;
	 		num = num%2 == 0 ? num/2 : ((num/-2)-1);
	 		decoded->back()[k] = num;
		}
	}
	return decoded->size() * decoded->back().size();
}