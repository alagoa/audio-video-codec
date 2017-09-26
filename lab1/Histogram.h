#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <fstream>
#include <vector>
#include <climits>


#define NUMERIC_MODE 0
#define ALPHA_MODE 1

struct bin
{
    int min;
    int max;
};

class Histogram {
	public:
		Histogram(int min, int max, int n_bins, std::stringstream &input, int mode);
		~Histogram();

		void printHistogramByValue();
		void printHistogramByOccurency();
		void printValues();
		void saveHistogram();
		int uniqueSymbols();

	private:
		void calculateBins();
		int getBin(int number);

		std::vector<std::pair<int,int>> vec;
		int min, max, n_bins, mode;
		bin bins[20000];
};