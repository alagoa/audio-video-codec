#include "Golomb.h"

Golomb::Golomb(){

}

Golomb::Golomb(int m) {
	this->m = m;
	this->b = std::log2(m);

}

void Golomb::encode(audio_data_t residuals, SF_INFO snd_info, int order) {
	out.open("encoded.cod", std::ofstream::out);
	int q,r,transf;

	
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
			out << q << " " << r << "\n";
		 }
		 out << "\n";
	}
	out.close();
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

encoded_data_t Golomb::real_encode(audio_data_t residuals) {
	encoded_data_t result;
	uint q,r,transf;
	for(auto &e : residuals) {
		std::vector<std::pair<uint,uint>> channel;
		for(auto &v : e) {
			transf = v >= 0 ? 2*v : (2*std::abs(v))-1;
			q = transf / m;
			r = transf - q*m;
			channel.push_back(std::make_pair(q,r));
		 }
		 result.push_back(channel);
	}
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

