#include "FCM.h"
#include <sstream>
#include <fstream>
#include <chrono>
#include <csignal>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace boost::archive;
namespace po = boost::program_options;

std::chrono::time_point<std::chrono::system_clock> start;
std::chrono::time_point<std::chrono::system_clock> end;
volatile unsigned long number_of_chars = 0;

void saveModel(FCM fcm, std::string fname) {
	std::ofstream fout(fname);
	{
		text_oarchive oa{fout};
		oa << fcm;
	}
}

void loadModel(FCM &fcm, std::string fname) {
	std::ifstream fin(fname);
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
	double words_per_sec = ((double)number_of_chars)/elapsed_seconds.count();
	std::cerr << "\nSymbols per sec: " << words_per_sec << "\n";
	std::exit(0);
}

int handleArgs(int argc, char* argv[], std::string &input, int &order, std::string &output, int &output_length, std::string &save_file, std::string &load_file) {
	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help,h", 																		"produce help message")
	    ("input,i", po::value<std::string>(&input),										"input file")
	    ("order,k", po::value<int>(&order)->default_value(1),							"set order of the model")
	    ("output,o", po::value<std::string>(&output)->default_value("out.txt"),			"output file")
	    ("output_length,len", po::value<int>(&output_length)->default_value(1000),		"length of the output file (in chars)")
	    ("save,s", po::value<std::string>(&save_file),									"file where the model will be saved")
	    ("read,r", po::value<std::string>(&load_file),									"file with the model to be read")
	;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);


	if (vm.count("help") || (!vm.count("input") && !vm.count("read"))) {
	    std::cout << desc << "\n";
	    return 0;
	}
	
	if(!boost::filesystem::exists(vm["input"].as<std::string>())) {
		std::cout << "Input file (" << vm["input"].as<std::string>() <<") not found." << "\n";
		return 0;
	}

	if(vm["order"].as<int>() < 0) {
		std::cout << "Order needs to be a positive number.\n" << desc << "\n";
		return 0;
	}

	if(vm["output_length"].as<int>() < 0) {
		std::cout << "Output length needs to be a positive number.\n" << desc << "\n";
		return 0;
	}

	return 1;
}

int main(int argc, char* argv[]) {

	// Handle input args
	int order;
	int output_length;
	int counter = 0;
	std::string input_file;
	std::string output;
	std::string save_file;
	std::string load_file;
	std::chrono::duration<double> elapsed_seconds;
	if(!handleArgs(argc, argv, input_file, order, output, output_length, save_file, load_file))
		return 0;

	std::signal(SIGINT, signal_handler);


	#ifdef DEBUG
		std::string __;
	#endif /* DEBUG */
		
	// Load model from file if requested.... 
	FCM fcm;
	if(!load_file.empty()) {
		start = std::chrono::system_clock::now();
		loadModel(fcm, load_file);
		end = std::chrono::system_clock::now();
		elapsed_seconds = end-start;
		std::cerr << "Model loaded in: " << elapsed_seconds.count() << "s\n";
	}
	//... create new model from input text if not
	else {
		std::stringstream input = readFileStream(input_file);
		start = std::chrono::system_clock::now();
		fcm = FCM(order, input.str());
		end = std::chrono::system_clock::now();
		elapsed_seconds = end-start;
		std::cerr << "Model created in: " << elapsed_seconds.count() << "s\n";
	}	
	// Save model if requested
	if(!save_file.empty()) {
		saveModel(fcm, save_file);
	}
	// Print entropy
	start = std::chrono::system_clock::now();
	std::cerr << "Entropy: " << fcm.getEntropy() << "\n";
	while(counter++ <= output_length){
		#ifdef DEBUG
			std::cin >> __;
			fcm.printContextInfo();
		#endif /* DEBUG */
		std::cout << fcm.guessNext();
		//std::cerr << '\r' << l;
	}
}