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
	std::string __;
	std::stringstream input = readFileStream(argv[1]);
	FCM testing = FCM(4, input.str());
	//std::cout << "Next will be: " << testing.guessNext() << "\n";
	std::cerr << "Entropy: " << testing.getEntropy() << "\n";
	while(1){
		//std::cin >> __;
		//testing.printContextInfo();
		std::cout << testing.guessNext();
	}
}