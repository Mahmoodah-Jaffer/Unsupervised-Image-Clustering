#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "clusterer.h"

int main(int argc, char* argv[]){

	using namespace std;
	using namespace JFFMAH001;
	Clusterer cl;

	if (argc < 2){
		std::cout << "Missing folder with images to be clustered";
		return 0;
	}

//Obtain arguments
	std::string dataset = string(argv[1]);
	std::string outFile = "std::cout";
	std::string clusters = "10";
	std::string width =  "1";
	//std::cout << dataset;

	if (argc == 4){
		if (string(argv[2]) == "-o"){
			std::string outFile = string(argv[3]);
			std::cout << outFile;
		}
		else if (string(argv[2]) == "-k"){
			std::string clusters =  string(argv[3]);
			std::cout << clusters;
		}
		else if (string(argv[2]) == "-bin"){
			std::string width =  string(argv[3]);
			std::cout << width;
		}
	}

	else{

		//check for output file name
		if (string(argv[2]) == "-o"){
			std::string outFile = string(argv[3]);
			std::cout << outFile;
		}
		else{
			std::cout << outFile;
		}

		//check for number of clusters
		if (string(argv[2]) == "-k"){
			std::string clusters =  string(argv[3]);
			std::cout << clusters;

		}
		else if (string(argv[4]) == "-k"){
			std::string clusters =  string(argv[5]);
			std::cout << clusters;
		}
		else{
			std::cout << clusters;
		}

		//check for width of histogram
		if (string(argv[2]) == "-bin"){
			std::string width =  string(argv[3]);
			std::cout << width;
		}
		else if (string(argv[4]) == "-bin"){
			std::string width =  string(argv[5]);
			std::cout << width;
		}
		else if (string(argv[6]) == "-bin"){
			std::string width =  string(argv[7]);
			std::cout << width;
		}
		else{
			std::cout << width;
		}

	}

	cl.readImages(dataset, stoi(width), stoi(clusters));
}
