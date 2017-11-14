#include "Golomb.h"

Golomb::Golomb(){

}

Golomb::Golomb(int m) {
	this->m = m;
	this->b = std::log2(m);

}

void Golomb::encoded(std::vector<std::vector<short>> residuals) {
	out.open("encoded.cod", std::ofstream::out);
	int i;
	int q,r,transf;
	out << residuals[0].size() << " " << residuals.size() << "\n";
	for(auto &e : residuals) {
		i=0;
		for(auto &v : e) {
			i++;
			transf = v >= 0 ? 2*v : (2*std::abs(v))-1;
			q = transf / m;
			r = transf - q*m;

			out << q << " " << r << "\n";
		 }
		 out << "\n";
	}
	out.close();
}

void Golomb::decode() {
	std::ifstream in("encoded.cod");
	//in.open("encoded.cod");
	//char linha[20];
	std::string line;
	int num_chan = 0;
	int q, r;
	int frames_per_chan = 0;
	in >> frames_per_chan;
	in >> num_chan;
	for (int i = 0; i < num_chan; ++i)
	{
		for (int k = 0; k < frames_per_chan; ++k)
		{
	 		in >> q;
	 		in >> r;
	 		std::cout << "q: " << q << " r: " << r << "\n"; 
		}
	}
}