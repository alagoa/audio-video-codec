#include "Golomb.h"

Golomb::Golomb(){
	this->m = 0;
	this->b = 0;
}

/*
uint Golomb::encode(audio_data_t residuals, SF_INFO snd_info, int order) {
	out.open("encoded.cod", std::ofstream::out);
	int q,r,transf;
	uint final_size = 0;
	out << residuals[0].size()	<< " ";				// Frames per channel
	out << residuals.size() 	<< " ";				// Number of channels
	out << snd_info.samplerate 	<< " ";
	out << snd_info.format 		<< " ";
	out << snd_info.sections 	<< " ";
	out << snd_info.seekable 	<< " ";
	out << order << " ";
	out << m << "\n";
	for(auto &e : residuals) {
		for(auto &v : e) {
			transf = v >= 0 ? 2*v : (2*std::abs(v))-1;
			q = transf / m;
			r = transf - q*m;
			final_size += (q +1 ) + b;
			out << q << " " << r << "\n";
		 }
		 out << "\n";
	}
	out.close();
	return final_size;
}
*/
/*
audio_data_t Golomb::decode(SF_INFO *snd_info, int *order) {
	std::ifstream in("encoded.cod");
	audio_data_t residuals;
	//in.open("encoded.cod");
	//char linha[20];
	std::string line;
	short num;
	int q, r;
	in >> snd_info->frames;
	in >> snd_info->channels;
	in >> snd_info->samplerate;
	in >> snd_info->format;
	in >> snd_info->sections;
	in >> snd_info->seekable;
	in >> *order;
	in >> this->m;
	for (int i = 0; i < snd_info->channels; ++i)
	{
		residuals.push_back(channel_data_t(snd_info->frames));
		for (int k = 0; k < snd_info->frames; ++k)
		{
	 		in >> q;
	 		in >> r;
	 		num = q*m + r;
	 		num = num%2 == 0 ? num/2 : ((num/-2)-1);
	 		residuals[i][k] = num;
		}
	}
	return residuals;
}
*/
uint Golomb::real_encode(audio_data_t const &residuals, encoded_data_t *out) {
	uint q,r;
	uint final_size = 0;
	golomb_transform_t tranformed_data;
	find_m(residuals, tranformed_data);
	for(auto &e : tranformed_data) {
		out->push_back(encoded_channel_t());
		for(auto &v : e) {
			q = v / m;
			r = v - q * m;
			final_size += (q + 1) + b;
			out->back().push_back(std::make_pair(q, r));
		 }
	}
	return final_size / 8;
}

ushort Golomb::find_m(audio_data_t const &residuals, golomb_transform_t &transf_data){
	uint q,transf, prev_size, final_size, original;
	bool next_m;
	original = residuals.size() * residuals[0].size() * sizeof(short);
	prev_size = UINT_MAX;
	final_size = 0;
	m = 2;
	b = std::log2(m);
	std::cout << "Original size = " << original << "\n";
	next_m = true;
	std::cout << "\tTrying m = " << m << "...\n";
	for(auto &e : residuals) {
		channel_transform_t channel;
		for(auto &v : e) {
			transf = v >= 0 ? 2*v : (2*std::abs(v))-1;
			q = transf / m;
			final_size += ((q+1) + b);
			channel.push_back(transf);
		 }
		 transf_data.push_back(channel);
	}
	final_size = final_size/8;
	std::cout << "\t\tsize for current m = " << final_size << "\n";
	prev_size = final_size;
	final_size = 0;
	m <<= 1;
	b = std::log2(m);

	while(next_m) {
		std::cout << "\tTrying m = " << m << "...\n";
		for(auto &e : transf_data) {
			for(auto &v : e) {
				q = v / m;
				final_size += ((q+1) + b);
			 }
		}
		final_size = final_size/8;
		std::cout << "\t\tsize for current m = " << final_size << "\n";
		if(final_size < prev_size ) {
			prev_size = final_size;
			final_size = 0;
			m <<= 1;
			b = std::log2(m);
		}
		else {
			next_m = false;
		}
	}
	m >>= 1;
	std::cout << "chosen m -> " << m << "\n";
	return m;
}

void Golomb::real_decode(encoded_data_t const &encoded, audio_data_t *decoded) {
	if (m == 0)
		return;
	int num;
	int num_chan = encoded.size();
	uint q, r;
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
}

void Golomb::real_decode(encoded_data_t const &encoded, audio_data_t *decoded, ushort new_m) {
	int num;
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
	 		num = q * new_m + r;
	 		num = num%2 == 0 ? num/2 : ((num/-2)-1);
	 		decoded->back()[k] = num;
		}
	}
}

