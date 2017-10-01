#include <unordered_map>
#include <string> 
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>

typedef std::unordered_map<std::string, unsigned int> InnerCounter;

class FCM {
	public:
		FCM(int order, std::string input);
		~FCM();

		void addChar(char c);
		void printContextInfo(std::string contx);
		void getEntropy();
		std::string guessNext();
		void saveModel();
		void readModel();
		void printModelInfo();
	private:
		std::unordered_map<std::string, InnerCounter> map;
		std::string current_context;
};