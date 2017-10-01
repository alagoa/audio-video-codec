#include "FCM.h"

FCM::FCM(int order, std::string input) {
	std::string innerkey;
	std::string to_add;
	//Strip all symbols but letters and spaces
	input.erase(std::remove_if(input.begin(), input.end(), 
		[](char c) { 
			return !isalpha(c) && c!=0x20; 
		}), input.end());
	
	for(unsigned int i = order; i < input.length(); i++) {
		to_add = input.substr(i-order, order);
		innerkey = std::string(1, input[i]);
		if (map.count(to_add)){
			if ( map[to_add].count(innerkey) )
				map[to_add][innerkey]++;
			else
				map[to_add][innerkey] = 1;
		}
		else
			map[to_add] = InnerCounter( {{innerkey, 1}} );
	}
	current_context = input.substr(input.length()-order, order);
	std::cout << current_context << std::endl;
}

FCM::~FCM() {

}

void FCM::printModelInfo(){
	for(auto &e : map) {
		for (auto it = e.second.begin(); it != e.second.end(); ++it)
			std::cout << "[" << e.first << "][" << it->first << "] = " << it->second << std::endl;
	}
}

std::string FCM::guessNext(){
	InnerCounter* inner_map = &map[current_context];
	std::string best_guess;
	unsigned int max = 0;
	for (auto it = inner_map->begin(); it != inner_map->end(); ++it){
		if (it->second > max)
		{
			max = it->second;
			best_guess = it->first;
		}
	}
	return best_guess;
}

void FCM::printContextInfo(std::string contx){
	if (!map.count(contx))
		return;
	InnerCounter* inner_map = &map[contx];
	for (InnerCounter::iterator it = inner_map->begin(); it != inner_map->end(); ++it)
		std::cout << "[" << contx << "][" << it->first << "] = " << it->second << std::endl;
}