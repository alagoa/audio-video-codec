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
	FCM(1, input.str());
}