#include "FCM.h"
#include <sstream>
#include <fstream>

std::stringstream readFileStream(std::string filename) {
	std::ifstream finput;
	std::stringstream input;
	finput.open(filename);
	input << finput.rdbuf();
	finput.close();	
	return input;
}

int main(int argc, char* argv[]) {
	unsigned long l = 0;
	int order;
	std::string __;
	std::stringstream input;
	std::string usage = "Usage: ./FCM <input file> <order>\n";
	
	if(argc < 3 || argv[1] == std::string("--help") || argv[1] == std::string("-h")) {
		std::cout << usage;
		return 0;
	}
	order = std::stoi(argv[2]);
	input = readFileStream(argv[1]);
	FCM testing = FCM(order, input.str());
	//std::cout << "Next will be: " << testing.guessNext() << "\n";
	std::cerr << "Entropy: " << testing.getEntropy() << "\n";
	while(1){
		//std::cin >> __;
		//testing.printContextInfo();
		std::cout << testing.guessNext();
		l++;
		std::cerr << '\r' << l;
	}
}