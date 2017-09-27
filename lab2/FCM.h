#include <map>
#include <string> 
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>

class FCM {

	public:

		FCM(int order, std::string input);
		~FCM();

		void addChar(char c);
		void getSymbolInfo(char c);
		void getEntropy();
		void saveModel();
		void readModel();
		void printModelInfo();
	private:
		std::map<std::string, int> map;
};