#include "FCM.h"

FCM::FCM() {

}

FCM::FCM(int order, std::string input) : order(order){
	std::string symbol;
	std::string context;
	for(unsigned int count = 'A'; count <= 'Z'; count++) {
		symbols_list[std::string(1, count)] = 0;
		symbols_list[std::string(1, count + 32)] = 0; //A->a just add 32
	}
	//for(unsigned int count = 'a'; count <= 'z'; count++) {
	//	symbols_list[std::string(1, count)] = 1;
	//}
	symbols_list[" "] = 0; symbols_list["\n"] = 0;

	//Strip all symbols but letters and spaces
	input.erase(std::remove_if(input.begin(), input.end(), 
		[](char c) { 
			return !isalpha(c) && c!=0x20 && c!='\n'; 
		}), input.end());
	len = input.length();

	context = input.substr(0, order);
	//std::cout << context << "\n";
	symbol = std::string(1, input[order]);
	for(unsigned int i = order; i < len-1; i++) 
	{
		#ifdef WITH_ALPHA
		if (map.count(context) <= 0)
			map.emplace(context, symbols_list);
		#endif /* WITH_ALPHA */
		map[context][symbol]++;
		context.erase(0, 1) += symbol;
		symbol = std::string(1, input[i+1]);
	}
	current_context = context.erase(0, 1) + symbol;

	//current_context = input.substr(len-order, order);
	std::cout << current_context;
	data = input;
	genRandom();
}

FCM::~FCM() {

}

void FCM::genRandom() {
	gen = std::mt19937(std::time(0));
}

void FCM::printModelInfo(){
	for(auto &e : map) {
		for (auto it = e.second.begin(); it != e.second.end(); ++it)
			std::cout << "[" << e.first << "][" << it->first << "] = " << it->second << "\n";
	}
}

std::string FCM::guessNext(){
	float alpha = 0.1;
	InnerCounter* inner_map = &map[current_context];
	std::string best_guess;
	std::vector<float> weights;
	std::vector<std::string> symbols;
	//static std::mt19937 gen(std::time(0));
	std::discrete_distribution<unsigned int> dist;
	//check if inner_map is empty or you get a segfault
	if (inner_map->empty())
		inner_map = &symbols_list;

	for (auto it = inner_map->begin(); it != inner_map->end(); ++it)
	{
		weights.push_back(it->second+alpha);
		symbols.push_back(it->first);
	}
	dist = std::discrete_distribution<unsigned int>(weights.begin(), weights.end());
	best_guess = symbols[dist(gen)];
	
	//data += best_guess;
	current_context.erase(0,1) += best_guess;
	//map[current_context][best_guess]++;
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
	if (map.count(current_context) <= 0)
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