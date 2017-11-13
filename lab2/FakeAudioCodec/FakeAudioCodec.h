#include "AudioEntropy.h"
#include <bitset>
class FakeAudioCodec
{
public:
	FakeAudioCodec();
	FakeAudioCodec(std::string filename);
	~FakeAudioCodec();
	unsigned short to_unary_code(unsigned short val);
	std::string to_unary_code_s(unsigned short val);
private:
	counter* hists;
	int m;
};