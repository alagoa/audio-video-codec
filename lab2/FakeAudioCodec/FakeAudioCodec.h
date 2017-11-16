#include "AudioEntropy.h"
#include <bitset>
#include <numeric>
#include <vector>

class FakeAudioCodec
{
public:
	FakeAudioCodec();
	FakeAudioCodec(std::string filename);
	~FakeAudioCodec();
	unsigned short to_unary_code(unsigned short val);
	std::string to_unary_code_s(unsigned short val);
	int get_m();
private:
	const counter* hists;
	SF_INFO snd_info;
	int m;
};