#include <string>
#include <sstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <fstream>
#include <vector>

struct bin
{
    int min;
    int max;
};

class Histogram {
	public:
		Histogram(int min, int max, int n_bins, std::string input);
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
		int min, max, n_bins;
		bin bins[200000];
};