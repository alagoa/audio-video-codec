#include "Histogram.h"
#include <climits> 

std::stringstream readFileStream() {
	std::ifstream finput;
	finput.open("wavsample01.txt");
	int i = 0;
	std::stringstream input;
	input << finput.rdbuf();
	finput.close();	
	return input;
}

int main(int argc, char* argv[]) {
	if(argc < 2) {
		std::cout << "Usage: ./out <number of bins>\n";
		return 0;
	}
	std::ifstream fin("wavsample01.txt");
    int min = INT_MAX,max = INT_MIN, i=0, n;
    while (fin >> n){
        if (n < min)
            min = n;
        else if (n > max)
            max = n;
        if (!fin)
        	break;
    }    
    
    int bins = atoi(argv[1]);
    if(bins >= max - min) {
    	std::cout << "Error: Number of bins has to be lower that number count.";
    	return 0;
    }

    std::stringstream input = readFileStream();
    Histogram h (min, max, bins, input.str());
    
	//h.printValues();
	h.printHistogramByValue();
	//std::cout << h.uniqueSymbols();
	//h.printHistogramByOccurency();
	h.saveHistogram();

	return 0;
}