#include <string>   // Needed for strings
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream> 
#include <vector>
#include <stdexcept>
#include <stdio.h>
#include <dirent.h>
#include <ctime>
#include <cmath> 
#include "clusterer.h"

using namespace JFFMAH001;
using namespace std;

std::vector<JFFMAH001::Clusterer::Feature> images;

JFFMAH001::Clusterer::Clusterer(){
	width = 0;
	height = 0;
} 

JFFMAH001::Clusterer::~Clusterer(){
/*	int size = images.size();
	for (int k = 0; k < size; k++){
		for (int i = 0; i<height;i++){
			delete [] images[k][i];
		}
		delete [] images[k];
	}*/

/*	int h_size = Histograms.size();
	for (int i = 0; i < size; ++i)
	{
		delete [] Histograms[i];
	}*/

	width = 0;
	height = 0;
}

void JFFMAH001::Clusterer::readImages(std::string location, int bin,int k){
	
	//get file names
	DIR*    dir;
    dirent* pdir;
    std::vector<std::string> files;
    location.append("/");

    dir = opendir(location.c_str());

    while (pdir = readdir(dir)) {
        files.push_back(pdir->d_name);
    }

    //remove directory ./ and ../
    for (vector<std::string>::iterator it = files.begin() ; it != files.end() ; ++it){
		if ("." == (*it)){
			files.erase(it);
		}
		if (".." == (*it)){
			files.erase(it);
		}
	}

	//get dimensions and add binary block to array
	for (vector<std::string>::iterator it = files.begin() ; it != files.end() ; ++it){
		//std::cout << *it << endl;
		std::string dimfile = location;
		std::string file = dimfile.append(*it);

		

		std::cout << file << endl; //check file name and directory is correct
		


		ifstream ppmfile((file).c_str(), ios_base::in | ios_base::binary); //open file and specify it is a binary file
		if(!ppmfile) { 
			cout << "Couldn't open " << file << endl; 
		}else{
			Feature image_f; //create feature object
			image_f.id = file; //set image id to filename
			std::string discard;
			std::getline(ppmfile, discard);//discard P6 line
			//std::cout << discard << endl;

			bool comment = false;
			std::string dim;
			while(comment == false){


				std::getline(ppmfile, discard);
				std::string temp;
				stringstream ss(discard);

				ss >> std::ws;
				std::getline(ss, temp);
				//std::cout << temp << endl;

				if (temp[0] != '#'){
					comment = true;
					dim = temp;
					//std::cout << dim << endl;
				}

			}
			//get dimensions
			std::string word = "";
			for (auto x: dim){
				if ((x == ' ')){
					//std::cout << "width: " << word << endl; //get Nrows
					width = stoi(word);
					word = "";
				}
				else{
					word = word +x;
				}
			}
			//std::cout << "height: " << word << endl; //get Ncols
			height = stoi(word);

			std::getline(ppmfile, discard);
			std::string intensity; //read 255
			stringstream ss(discard);

			ss >> std::ws;
			std::getline(ss, intensity);
			//std::cout << intensity << endl;
			ss >> std::ws; //get rid of final newline before data block

			//Read binary data block
			int components = 3;
			int length = height*width*components;
			unsigned char* image = new unsigned char [length]; //data block with height and widthx3 - colours
			ppmfile.read((char*)image, length);

			//convert to 2D array
			unsigned char** imgslice = new unsigned char*[height];
			for(int h = 0; h < height; h++){
				imgslice[h] = new unsigned char[width*components];
				for (int w = 0; w < (width*components); w++){
					imgslice[h][w] = image[h+w];
				} 
			}

			//convert images to greyscale
			unsigned char** greyscale = new unsigned char*[height];
			for(int j = 0; j < height; j++){
				greyscale[j] = new unsigned char[width];
				int c = 0;
				for(int i = 0; i < (width*components); i = i + components){
					int red = imgslice[j][i + 0];//first component
					int green = imgslice[j][i + 1]; //second component
					int blue = imgslice[j][i + 2]; //third component
					//apply greyscale formula
					int grey = 0.21*red + 0.72*green + 0.07*blue;
					greyscale[j][c] = grey;	
					c++;			
				}
			}
			int* feature = imageFeatures(greyscale, bin);
			image_f.hist = feature;
			image_f.minDist = INT8_MAX;
			images.push_back( image_f); //push back image feature
		}
		ppmfile.close();
	} 
	//return vector with image features
    //return images;
    //kMeansClustering(&images,k);

}

int* JFFMAH001::Clusterer::imageFeatures(unsigned char** array, int bin){

/*	for (vector<unsigned char**>::iterator it = array.begin() ; it != array.end() ; ++it){
		int* histogram = new int[256/bin]; //new histogram for each image
		for(int h = 0; h < height; h++){
			for(int w = 0; w < width; w++){
				int index = round(((*it)[h][w])/bin);
				histogram[index] = histogram [index] + 1;
			}
		}
		Histograms.push_back(histogram);
	}

	return Histograms; //return vector with the image feature*/
	int* histogram = new int[256/bin]; //new histogram for each image
	for(int h = 0; h < height; h++){
		for(int w = 0; w < width; w++){
			int index = round(((array)[h][w])/bin);
			histogram[index] = histogram [index] + 1;
		}
	}

	return histogram;

}

void kMeansClustering(std::vector<JFFMAH001::Clusterer::Feature>* v, int k){
	//Initiliase clusters
	vector<JFFMAH001::Clusterer::Feature> centroids;
	int n = floor(v->size()/k);
	srand(time(0));  // need to set the random seed
	for (int i = 0; i < k; ++i) {
	    centroids.push_back(v->at(rand() % n));
	}

	//Assign points to cluster
	for (vector<JFFMAH001::Clusterer::Feature>::iterator c = begin(centroids); c != end(centroids); ++c) {
   
    int clusterId = c - begin(centroids); //get cluster index

	    for (vector<JFFMAH001::Clusterer::Feature>::iterator it = v->begin();it != v->end(); ++it) {
	        JFFMAH001::Clusterer::Feature p = *it;
	        int dist = c->distance(p);
	        if (dist < p.minDist) {
	            p.minDist = dist;
	            p.cluster = clusterId;
	        }
	        *it = p;
	    }
	}

	vector<int> nPoints;
	vector<int> sumHist;

	// Initialise with zeroes
	for (int j = 0; j < k; ++j) {
	    nPoints.push_back(0);
	    sumHist.push_back(0);
	}	


	// Iterate over points to append data to centroids
	for (vector<JFFMAH001::Clusterer::Feature>::iterator it = v->begin(); it != v->end(); ++it) {
	    int clusterId = it->cluster;
	    nPoints[clusterId] += 1;

	    for(int i = 0; i < sizeof(it->hist); i++){
	    	sumHist[clusterId] += it->hist[i];
	    }
	    it->minDist = INT8_MAX;  // reset distance
	}

	// Compute the new centroids
	for (vector<JFFMAH001::Clusterer::Feature>::iterator c = begin(centroids); c != end(centroids); ++c) {
	    int clusterId = c - begin(centroids);
	    for(int i = 0; i < sizeof(c->hist); i++){
	    	c->hist[i] = sumHist[clusterId] / nPoints[clusterId];
	    }
	}


}

//Overload << operator to view the results of the clusterer
ostream& operator<<(ostream& os, JFFMAH001::Clusterer::Feature& feature){
	os << feature.getName() << " ";
	return os;
}


