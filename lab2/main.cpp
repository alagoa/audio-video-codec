#include "FCM.h"
#include <sstream>
#include <fstream>
#include <chrono>
#include <csignal>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace boost::archive;

std::chrono::time_point<std::chrono::system_clock> start;
std::chrono::time_point<std::chrono::system_clock> end;
volatile unsigned long l = 0;

void saveModel(FCM fcm) {
	std::ofstream fout("fcm.bin");
	{
		text_oarchive oa{fout};
		oa << fcm;
	}
}

void loadModel(FCM &fcm) {
	std::ifstream fin("fcm.bin");
	{
		text_iarchive ia{fin};
		ia >> fcm;
	}
	
	fcm.genRandom();
}

std::stringstream readFileStream(std::string filename) {
	std::ifstream  finput;
	std::stringstream input;
	finput.open(filename);
	input << finput.rdbuf();
	finput.close();	
	return input;
}

void signal_handler(int signal)
{
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds;
	elapsed_seconds = end-start;
	double words_per_sec = ((double)l)/elapsed_seconds.count();
	std::cerr << "\nSymbols per sec: " << words_per_sec << "\n";
	std::exit(0);
}

int main(int argc, char* argv[]) {
	std::signal(SIGINT, signal_handler);
	int order;
	std::chrono::duration<double> elapsed_seconds;
	#ifdef DEBUG
		std::string __;
	#endif /* DEBUG */
	std::stringstream input;
	std::string usage = "Usage: ./FCM <input file> <order>\n";
	
	if(argc < 3 || argv[1] == std::string("--help") || argv[1] == std::string("-h")) {
		std::cout << usage;
		return 0;
	}
	order = std::stoi(argv[2]);
	input = readFileStream(argv[1]);
	start = std::chrono::system_clock::now();
	FCM testing = FCM(order, input.str());
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;
	std::cerr << "Init in: " << elapsed_seconds.count() << "s\n";
	saveModel(testing);
	FCM good;
	loadModel(good);
	//std::cerr << "Entropy: " << testing.getEntropy() << "\n";
	start = std::chrono::system_clock::now();
	while(l<10000){
		#ifdef DEBUG
			std::cin >> __;
			good.printContextInfo();
		#endif /* DEBUG */
		std::cout << good.guessNext();
		l++;
		//std::cerr << '\r' << l;
	}
}