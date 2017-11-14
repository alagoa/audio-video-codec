#include "Golomb.h"

Golomb::Golomb(int m) {
	this->m = m;
	this->b = std::log2(m);
	out.open("encoded.cod");

}

Golomb::encode(std::vector<std::vector<short>> residuals) {
	int q,r,transf;
	for(auto &e : residuals) {
		for(auto &v : e) {

			transf = v >= 0 ? 2*v : (2*std::abs(v))-1;
			q = transf / m;
			r = transf - q*m;

			out << q << "," << r << "\n";
		 }
		 out << "\n\n";
	}

	out.close();
}

Golomb::decode() {

}