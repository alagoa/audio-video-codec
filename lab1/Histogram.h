#include <string>
#include <vector>
#include <sstream>

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
		bin bins[20000];
};