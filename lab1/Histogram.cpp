#include "Histogram.h"

Histogram::Histogram(int min, int max, int n_bins, std::string input) {
	//std::cout << "i'm in";
	this->min = min;
	this->max = max;
	this->n_bins = n_bins;
	//std::cout << "goin to calculateBins";
	std::stringstream stream;
	stream << input;		
	std::map<int, int> map;

	if(n_bins != 0) {
		calculateBins();
		int n;
		while(stream >> n) {			
	   		int bin_idx = getBin(n);
	   		
			if(map.count(bin_idx) == 0) {
				map[bin_idx] = 1;
			}
			else {
				map[bin_idx] = map.find(bin_idx)->second + 1;
			}		
			if(!stream)
	      		break;
		}
	}
	else {
		int n;
		while(stream >> n) {			
	   		//int bin_idx = getBin(n);
			if(map.count(n) == 0) {
				map[n] = 1;
			}
			else {
				map[n] = map.find(n)->second + 1;
			}		
			if(!stream)
	      		break;
		}
	}
	
	
	//std::cout << "calculated Bins!";
	// std::cout << input;

	for (std::map<int,int>::iterator it=map.begin(); it!=map.end(); ++it) {
		vec.push_back(std::make_pair(it->first, it->second));
	}

	


/*  if(n_bins = 0)
	// Strip chars
	input.erase(std::remove_if(input.begin(), input.end(), [](char c) { return !isalpha(c); } ), input.end());

	// Map occurences
	std::map<char, int> map;
	for(int i = 0; i < input.length(); i++) {
		if(map.count(input.at(i)) == 0) {
			map[input.at(i)] = 1;
		}
		else {
			map[input.at(i)] = map.find(input.at(i))->second + 1;
		}
	}

	// Copy to vector
	typedef std::pair<char, int> elem;
	int i = 0;
	for (std::map<char,int>::iterator it=map.begin(); it!=map.end(); ++it) {
		vec.push_back(std::make_pair(it->first, it->second));
	}
*/

}

Histogram::~Histogram() {

}

void Histogram::printHistogramByValue() {

	std::stringstream out;
 
	/*
	for (std::map<char,int>::iterator it=map.begin(); it!=map.end(); ++it) {
		out << "\n" << it->first << "\t";

		for(int stars = 0; stars < it->second; stars++) {
			out << "*";
		}
	}
	*/

	if(n_bins != 0) {
		for(auto &e : vec) {
			out << "\n[" << bins[e.first].min << " , " << bins[e.first].max << "] \t\t\t";

			for(int stars = 0; stars < e.second; stars++) {
				out << "*";
			}
		}	
	}
	else {
		for(auto &e : vec) {
			out << "\n[" << e.first << "] \t";

			for(int stars = 0; stars < e.second; stars++) {
				out << "*";
			}
		}	
	}

	std::cout << out.str() + "\n";

}

void Histogram::printHistogramByOccurency() {

	std::stringstream out;

	std::vector<std::pair<int,int>> tmp = vec;
	std::sort(tmp.begin(), tmp.end(), [](auto &left, auto &right) {
    	return left.second < right.second;	
	});

	if(n_bins != 0) {
		for(auto &e : tmp) {
			out << "\n[" << bins[e.first].min << " , " << bins[e.first].max << "] \t\t\t";

			for(int stars = 0; stars < e.second; stars++) {
				out << "*";
			}
		}	
	}
	else {
		for(auto &e : tmp) {
			out << "\n[" << e.first << "] \t";

			for(int stars = 0; stars < e.second; stars++) {
				out << "*";
			}
		}	
	}

	std::cout << out.str();

}


void Histogram::printValues() {
	if(n_bins != 0) {
		for(auto &e : vec) {
			std::cout << "[" << bins[e.first].min << " , " << bins[e.first].max << "] = " << e.second << '\n';
		}
	}
	else {
		for(auto &e : vec) {
			std::cout << "[" << e.first << "] = " << e.second << '\n';
		}
	}
}

void Histogram::saveHistogram() {
	std::ofstream fout("histogram.txt");

	std::vector<std::pair<int,int>> tmp = vec;
	std::sort(tmp.begin(), tmp.end(), [](auto &left, auto &right) {
    	return left.second < right.second;	
	});

	std::stringstream out;
 
	if(n_bins != 0) {
		for(auto &e : tmp) {
			out << "\n[" << bins[e.first].min << " , " << bins[e.first].max << "] \t\t\t";

			for(int stars = 0; stars < e.second; stars++) {
				out << "*";
			}
		}	
	}
	else {
		for(auto &e : tmp) {
			out << "\n[" << e.first << "] \t\t\t";

			for(int stars = 0; stars < e.second; stars++) {
				out << "*";
			}
		}	
	}

	fout << out.str();
	fout.close();
}

int Histogram::uniqueSymbols() {
	return vec.size();
}

void Histogram::calculateBins() {

    int i;
    int bin_sizes[n_bins];

    for(i=0; i<n_bins; ++i)
        bin_sizes[i] = (max-min+1)/n_bins;

    /* distribute excess as evenly as possible across bins */
    int excess = (max-min+1)%n_bins;
    for(i=0; excess>0; --excess, i=(i+1)%n_bins)
        bin_sizes[i] += 1; 

    int n=0, k=min;
    for(i=0; i<n_bins && k<=max; ++i, ++n){
        bins[i].min=k;
        bins[i].max=k+bin_sizes[i]-1;
        k += bin_sizes[i];
    }
}

int Histogram::getBin(int number) {
    int i;
    for(i=0; i<n_bins; ++i)
        if(number >= bins[i].min && number <= bins[i].max)
            return i;
    return n_bins;
}