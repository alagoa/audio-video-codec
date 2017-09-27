#include "FCM.h"

FCM::FCM(int order, std::string input) {
	//Strip all symbols but letters and spaces
	input.erase(std::remove_if(input.begin(), input.end(), [](char c) { return !isalpha(c) && c!=0x20; } ), input.end());


	for(unsigned int i = 0; i < input.length(); i++) {
		std::stringstream to_add;
		for(unsigned j = 0; j < order; j++) {
			to_add << input.at(i+j);
		}
		if(map.count(std::string(order, input.at(i))) == 0) {
			map[std::string(order,input.at(i))] = 1;
		}
		else {
			map[std::string(order, input.at(i))] = map.find(std::string(order, input.at(i)))->second + 1;
		}
	}

	for(auto &e : map) {
		std::cout << "[" << e.first << "] = " << e.second << '\n';
	}
}

FCM::~FCM() {

}