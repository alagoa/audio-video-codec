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
	std::stringstream input = readFileStream(argv[1]);
	FCM testing = FCM(2, input.str());
	std::cout << "Next will be: " << testing.guessNext() << "\n";
	std::cout << "Entropy: " << testing.getEntropy() << std::endl;
}