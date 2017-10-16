#include "Histogram.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <climits>
#include <algorithm>

std::stringstream readFileStream(std::string filename) {
	std::ifstream finput;
	finput.open(filename);
	std::stringstream input;
	input << finput.rdbuf();
	finput.close();	
	return input;
}

int main(int argc, char* argv[]) {
	std::string usage = "Usage: ./Histogram <input file> <number of bins> <mode>\n\n"
						"\t<input file>\n"
						"\t<number of bins>\t:  0 for no bins\n"				
						"\t<mode>\t\t:  '-n' for numerical input; '-a' for alpha input\n";
	
	if(argc < 4 || argv[1] == std::string("--help") || argv[1] == std::string("-h")) {
		std::cout << usage;
		return 0;
	}
	
	int bins = atoi(argv[2]);
	if(argv[3] == std::string("-n")) {
		std::ifstream fin(argv[1]);
		int min = INT_MAX;
		int max = INT_MIN;
		int n;
    	while (fin.good()){
    		fin >> n;
	        if (n < min)
	            min = n;
	        else if (n > max)
	            max = n;
	    }

	    if(bins >= max - min) {
	    	std::cout << "Error: Number of bins has to be lower that range of symbols.\n";
			return 0;
    	}
    	std::stringstream input = readFileStream(argv[1]);
    	Histogram h (min, max, bins, input, NUMERIC_MODE);
    	h.printValues();
		//h.printHistogramByValue();
		//std::cout << h.uniqueSymbols();
		//h.printHistogramByOccurency();
		//h.saveHistogram();
	}

	else if(argv[3] == std::string("-a")) {
		std::stringstream input = readFileStream(argv[1]);
		std::string input_string = input.str();
		unsigned char min = UCHAR_MAX;
		unsigned char max = 0;
		// Strip non alphas
		input_string.erase(
			std::remove_if(input_string.begin(), input_string.end(), 
				[&min, &max](char c) {
					if (!isalpha(c))
						return true;
					if(c < min)
						min = c;
					else if (c > max)
						max = c;
			 		return false; 
				}
			), input_string.end());
	    if(bins >= max - min) {
	    	std::cout << "Error: Number of bins has to be lower that range of symbols.\n";
			return 0;
    	}

    	Histogram h ((int)min, (int)max, bins, input, ALPHA_MODE);
    	//h.printValues();
		h.printHistogramByValue();
		//std::cout << h.uniqueSymbols();
		//h.printHistogramByOccurency();
		//h.saveHistogram();
	}
	else {
		std::cout << usage;
	}
	return 0;
}