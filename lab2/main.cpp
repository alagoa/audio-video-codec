#include "FCM.h"

std::stringstream readFileStream(std::string filename) {
	std::ifstream finput;
	finput.open(filename);
	std::stringstream input;
	input << finput.rdbuf();
	finput.close();	
	return input;
}

int main(int argc, char* argv[]) {
	std::stringstream input = readFileStream(argv[1]);
	FCM testing = FCM(3, input.str());
	std::cout << "Next will be: " << testing.guessNext() << std::endl;
}