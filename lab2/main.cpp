#include "AudioEntropy.h"

int main(int argc, char const *argv[])
{
	AudioEntropy ae(argv[1]);
	double en = ae.entropy();
	std::cout << "entropy: " << en << std::endl;
	return 0;
}