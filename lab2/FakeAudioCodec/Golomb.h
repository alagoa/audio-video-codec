#include <iostream>
#include <fstream>

class Golomb {

	typedef std::vector<std::vector<std::pair<int,int>>> encoded_data;
	public:
		Golomb(int m);
		enconded_data enconde(std::vector<std::vector<short>> residuals);
		void decode(int* end);
		void f_write(std::string file_name);
	private:
		const int m;
		const int b;
		ofstream out;		
}