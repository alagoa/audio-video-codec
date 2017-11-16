#include "FakeAudioCodec.h"

FakeAudioCodec::FakeAudioCodec(){

}

FakeAudioCodec::FakeAudioCodec(std::string filename){
	m = 512;
	unsigned short q;
	std::string q_s;
	unsigned short r;
	std::cout << "begin AudioEntropy" << "\n";
	AudioEntropy ae = AudioEntropy(filename);
	std::cout << "done AudioEntropy" << "\n";
	this->hists = ae.get_counters();
	this->snd_info = ae.get_snd_info();
	std::cout << "start printing" << "\n";
	std::cout << "m = " << get_m();
	/*
	for (auto it = hists[0].begin(); it != hists[0].end(); ++it)
	{
		q = ((unsigned short)(it->first) / m);
		q_s = to_unary_code_s(q);
		r = ((unsigned short)it->first) - q * m;
		std::cout << q_s << std::bitset<16>(r) << "\n";
	}
	*/

}
std::string FakeAudioCodec::to_unary_code_s(unsigned short val){
	std::string unary = "0";
	//std::cout << "num: " << val << "\n";
	//unsigned int unary = 0;
	for (int i = 1; i <= val + 1; ++i)
		unary.insert(0, "1");
		//unary += (1 << i);
	return unary;
}

unsigned short FakeAudioCodec::to_unary_code(unsigned short val){
	unsigned short unary = 0;
	for (int i = 1; i <= val + 1; ++i)
		unary += (1 << i);
	return unary;
}

FakeAudioCodec::~FakeAudioCodec(){

}

