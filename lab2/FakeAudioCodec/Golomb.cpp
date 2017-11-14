#include "Golomb.h"

Golomb::Golomb(int m) {
	this->m = m;
	this->b = std::log2(m);
	out.open("encoded.cod");

}

Golomb::encode(std::vector<std::pair<short,short>> residuals) {
	int q,r,transf;
	for(auto &e : residuals) {
		// Left
		transf = e.first >= 0 ? 2*e.first : (2*std::abs(e.first))-1;
		q = transf / m;
		r = transf - q*m;

		// Right
		transf = e.second >= 0 ? 2*e.second : (2*std::abs(e.second))-1;
		q = transf / m;
		r = transf - q*m;

		out << q << "," << r << "\n";
	}

	out.close();
}

Golomb::decode() {

}