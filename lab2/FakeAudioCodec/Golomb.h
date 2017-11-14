#include <iostream>
#include <fstream>

class Golomb {
	public:
		Golomb(int m);
		void enconde(std::vector<std::vector<short>> residuals);
		void decode(int* end);
	private:
		const int m;
		const int b;
		ofstream out;		
}