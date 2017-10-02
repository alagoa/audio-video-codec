#include <unordered_map>
#include <string> 
#include <algorithm>
#include <iostream>
#include <cmath>

typedef std::unordered_map<std::string, unsigned int> InnerCounter;

class FCM {
	public:
		FCM(int order, std::string input);
		~FCM();

		void addChar(char c);
		void printContextInfo(std::string contx);
		double getEntropy();
		std::string guessNext();
		double probOfSymbol(std::string contx, std::string symbol);
		void saveModel();
		void readModel();
		void printModelInfo();
	private:
		std::unordered_map<std::string, InnerCounter> map;
		std::string current_context;
		std::string data;
		int order;
		unsigned int len;
};