#include "Golomb.h"

Golomb::Golomb(){

}

Golomb::Golomb(int m) {
	this->m = m;
	this->b = std::log2(m);

}

void Golomb::encode(audio_data_t residuals) {
	out.open("encoded.cod", std::ofstream::out);
	int i;
	int q,r,transf;
	out << residuals[0].size() << " " << residuals.size() -1 << " " << residuals[residuals.size()-1][0] << "\n";
	for(auto &e : residuals) {
		i=0;
		for(auto &v : e) {
			i++;
			transf = v >= 0 ? 2*v : (2*std::abs(v))-1;
			q = transf / m;
			r = transf - q*m
;			out << q << " " << r << "\n";
		 }
		 out << "\n";
	}
	out.close();
}

audio_data_t Golomb::decode() {
	std::ifstream in("encoded.cod");
	audio_data_t residuals;
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
		residuals.push_back(channel_data_t(frames_per_chan));
		for (int k = 0; k < frames_per_chan; ++k)
		{
	 		in >> q;
	 		in >> r;
	 		num = q*m + r;
	 		num = num%2 == 0 ? num/2 : ((num/-2)-1);
	 		residuals[i][k] = num;
		}
	}
	channel_data_t order_vector;
	order_vector.push_back(order);
	residuals.push_back(order_vector);
	return residuals;
}

encoded_data_t real_encode(audio_data_t residuals) {

}

audio_data_t real_decode(encoded_data_t values) {

}

