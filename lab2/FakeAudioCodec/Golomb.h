#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

class Golomb {

	typedef std::vector<std::vector<std::pair<int,int>>> encoded_data;
	public:
		Golomb();
		Golomb(int m);
		void encode(std::vector<std::vector<short>> residuals);
		std::vector<std::vector<short>> decode();
		void f_write(std::string file_name);
	private:
		int m;
		int b;
		std::ofstream out;
		
};