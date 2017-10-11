#include <unordered_map>
#include <string> 
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <ctime>
//#define WITH_ALPHA

typedef std::unordered_map<std::string, unsigned int> InnerCounter;

class FCM {
	public:
		FCM(int order, std::string input);
		~FCM();

		void addChar(char c);
		void printContextInfo(std::string contx);
		void printContextInfo();
		double getEntropy();
		std::string guessNext();
		double probOfSymbol(std::string contx, std::string symbol);
		void saveModel();
		void readModel();
		void printModelInfo();
	private:
		std::unordered_map<std::string, InnerCounter> map;
		InnerCounter symbols_list;
		std::string current_context;
		std::string data;
		int order;
		unsigned int len;
		std::mt19937 gen;
};