#include "Golomb.h"

Golomb::Golomb(){
	this->m = -1;
	this->b = -1;
}

Golomb::Golomb(int m) {
	this->m = m;
	this->b = std::log2(m);
}

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
			final_size += (q ) + b;
			out << q << " " << r << "\n";
		 }
		 out << "\n";
	}
	out.close();
	return final_size;
}

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

encoded_data_t Golomb::real_encode(audio_data_t residuals, int *final_m) {
	
	encoded_data_t result;
	uint q,r,transf, prev_size = UINT_MAX, final_size = 0, original = residuals.size() * residuals[0].size() * sizeof(short);
	m = 2;
	b = std::log(m);
	std::cout << "Original size = " << original << "\n";
	bool next_m = true;
	while(next_m) {
		std::cout << "\tTrying m = " << m << "...\n";
		for(auto &e : residuals) {
			std::vector<std::pair<uint,uint>> channel;
			for(auto &v : e) {
				transf = v >= 0 ? 2*v : (2*std::abs(v))-1;
				q = transf / m;
				r = transf % m;
				final_size += ((q+1) + b);
				channel.push_back(std::make_pair(q,r));
			 }
			 result.push_back(channel);
		}

		std::cout << "\t\tsize for current m = " << final_size/8 << "\n";
		if(final_size/8 < prev_size ) {
			prev_size = final_size/8;
			final_size = 0;
			m <<= 1;
			b = std::log(m);
			result.clear();
		}
		else {
			next_m = false;
		}
	}
	m >>= 1;
	*final_m = m;
	std::cout << "chosen m -> " << m << "\n";
	return result;
}

audio_data_t Golomb::real_decode(encoded_data_t encoded, int m) {
	audio_data_t result;
	short num;
	int q, r;
	this->m = m;
	for(uint i = 0; i < encoded.size(); ++i) {
		result.push_back(channel_data_t(encoded[0].size()));
		for(uint k = 0; k < encoded[0].size(); ++k) {
			q = encoded[i][k].first;
			r = encoded[i][k].second;
			num = q*m + r;
	 		num = num%2 == 0 ? num/2 : ((num/-2)-1);
	 		result[i][k] = num;
		}
	}

	return result;
}

