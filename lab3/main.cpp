#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include "Player.h"

namespace po = boost::program_options;

int handleArgs(int argc, char* argv[], std::string &input, double &fps, int &wait, int &loop) {
	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help,h", 												"produce help message")
	    ("input,i", po::value<std::string>(&input),				"input file")
	    ("fps,f", po::value<double>(&fps)->default_value(15),	"set fps")
	    ("wait,w", po::value<int>(&wait),		"do not play video immediately")
	    ("loop,l", po::value<int>(&loop),		"loops the video")
	    ;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help") || !vm.count("input")) {
    	std::cout << desc << "\n";
    	return 0;
    }

	if(vm.count("input")) {	
		if(!boost::filesystem::exists(vm["input"].as<std::string>())) {
			std::cout << "Input file (" << vm["input"].as<std::string>() <<") not found." << "\n";
			return 0;
		}
	}

	if(vm.count("wait"))
		wait = 1;
	else
		wait = 0;

	if(vm.count("loop"))
		loop = 1;
	else
		loop = 0;

	if(vm["fps"].as<double>() < 0) {
		std::cout << "FPS needs to be a positive number.\n";
		return 0;
	}

    return 1;
}

int main(int argc, char** argv)
{
	std::string input_file;
	double fps;	
	int wait, loop;

	if(!handleArgs(argc, argv, input_file, fps, wait, loop))
		return 0;

/*  Player player = Player(input_file, fps, wait, loop); */
	Player player = Player();
	player.load_file(input_file);
	player.setFPS(fps);
	if(loop)
		player.loop();
	if(wait)
		player.pause();

	player.play();
	return 0;
}