#include "FCM.h"

FCM::FCM(int order, std::string input) : order(order){
	std::string symbol;
	std::string context;
	//Strip all symbols but letters and spaces
	input.erase(std::remove_if(input.begin(), input.end(), 
		[](char c) { 
			return !isalpha(c) && c!=0x20; 
		}), input.end());
	len = input.length();
	for(unsigned int i = order; i < len; i++) 
	{
		context = input.substr(i-order, order);
		symbol = std::string(1, input[i]);
		map[context][symbol]++;
	}
	current_context = input.substr(len-order, order);
	data = input;
}

FCM::~FCM() {

}

void FCM::printModelInfo(){
	for(auto &e : map) {
		for (auto it = e.second.begin(); it != e.second.end(); ++it)
			std::cout << "[" << e.first << "][" << it->first << "] = " << it->second << "\n";
	}
}

std::string FCM::guessNext(){
	InnerCounter* inner_map = &map[current_context];
	std::string best_guess;
	std::vector<unsigned int> weights;
	std::vector<std::string> symbols;
	//std::default_random_engine generator;
	std::mt19937 gen(std::time(0));
	std::discrete_distribution<int> dist;
	//unsigned int max = 0;
	for (auto it = inner_map->begin(); it != inner_map->end(); ++it)
	{
		weights.push_back(it->second);
		symbols.push_back(it->first);
		/*
		if (it->second > max)
		{
			max = it->second;
			best_guess = it->first;
		}
		*/
	}
	dist = std::discrete_distribution<int>(weights.begin(), weights.end());
	best_guess = symbols[dist(gen)];
	
	//data += best_guess;
	current_context = current_context.substr(1,order-1);
	current_context += best_guess;
	map[current_context][best_guess]++;
	
	return best_guess;
}

void FCM::printContextInfo(std::string contx){
	if (!map.count(contx))
		return;
	InnerCounter* inner_map = &map[contx];
	for (InnerCounter::iterator it = inner_map->begin(); it != inner_map->end(); ++it)
		std::cout << "[" << contx << "][" << it->first << "] = " << it->second << "\n";
}
void FCM::printContextInfo(){
	if (!map.count(current_context))
		return;
	InnerCounter* inner_map = &map[current_context];
	for (InnerCounter::iterator it = inner_map->begin(); it != inner_map->end(); ++it)
		std::cout << "[" << current_context << "][" << it->first << "] = " << it->second << "\n";
}

double FCM::getEntropy(){
	double sum = 0;
	std::string c_contx;
	for (unsigned int i = order; i < len; ++i)
	{
		c_contx = data.substr(i-order,order);
		sum += std::log(probOfSymbol(c_contx, std::string(1, data[i])));
	}
	return -(1.0/(double)len)*sum;

}

double FCM::probOfSymbol(std::string contx, std::string symbol){
	double symbol_val;
	double sum;
	InnerCounter* inner_map;
	if ( !map.count(contx) )
		return 0;
	inner_map = &map[contx];
	if ( !inner_map->count(symbol) )
		return 0;
	sum = 0;
	symbol_val = inner_map->at(symbol);
	for (InnerCounter::iterator it = inner_map->begin(); it != inner_map->end(); ++it)
		sum += it->second;
	return symbol_val/sum;
}