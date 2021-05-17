#ifndef CLUSTERER_H
#define CLUSTERER_H

#include <string>   // Needed for strings
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream> 
#include <vector>
#include <cmath> 

namespace JFFMAH001{
	class Clusterer{
	private:
		int width, height;
	public:
		Clusterer();
		~Clusterer();
		struct Feature;
		void readImages(std::string location,int bin,int k);
		int* imageFeatures(unsigned char** array, int bin);
		void kMeansClustering(std::vector<Feature>* v, int k);
	};

	//overload operator
	std::ostream& operator<<(std::ostream& os, Clusterer::Feature& feature);


	struct Clusterer::Feature{
			std::string id; //image name for feature
			double minDist; //current min distance to centroid
			int cluster; //cluster that feature belongs to
			int* hist;

		    int distance(Feature p) {
		    	int sum = 0;
		    	int power = 2;
		    	for(int i = 0; i < sizeof(hist); i++){
		    		int base = p.hist[i] - hist[i]; //base
		    		sum = sum + pow(base, power);
		    	}
		        return sum;
		    }

		    std::string getName(void){
		    	return id;
		    }
	};



}

#endif