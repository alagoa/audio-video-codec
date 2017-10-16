#include <unordered_map>
#include <string> 
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <ctime>
#include <boost/serialization/unordered_map.hpp>
//#define WITH_ALPHA

typedef std::unordered_map<std::string, unsigned int> InnerCounter;

class FCM {
	public:
		FCM(int order, std::string input);
		FCM();
		~FCM();

		void addSymbol(std::string symbol);
		void printContextInfo(std::string contx);
		void printContextInfo();
		double getEntropy();
		std::string guessNext();
		double probOfSymbol(std::string contx, std::string symbol, double* num_ap);
		void printModelInfo();
		void genRandom();
	private:
		friend class boost::serialization::access;
		template <typename Archive>
  		void serialize(Archive &ar, const unsigned int version) { 
  			ar & map;
  			ar & symbols_list;
  			ar & order;
  			ar & current_context;
  			ar & data;
  			ar & len;
  			ar & alpha;
  		}

		std::unordered_map<std::string, InnerCounter> map;
		InnerCounter symbols_list;
		std::string current_context;
		std::string data;
		int order;
		unsigned int len;
		std::mt19937 gen;
		const double alpha = 0.1;
};