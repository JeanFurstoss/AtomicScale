#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include "AtomHicExport.h"
#include "AtomicSystem.h"
#include "MathTools.h"
#include <string>

class ATOMHIC_EXPORT Descriptors {
private:
	std::string name = "none";
	AtomicSystem *_MySystem;
	MathTools *MT;
	unsigned int dim; // dimension of descriptors
	std::vector<unsigned int> nbDat; // nbDat[f] number of data with filter f
	unsigned int nbDatMax; // maximum number of data in all filter
	double *_Descriptors; // [f*nbDatMax*dim+i*dim+d] component d of descriptor i with filter f
	// label associated to each data, used for supervised learning (e.g. labeling of a GMM for structural analysis)
	unsigned int *Labels_uint; // [f*nbDatMax+i] = l with l the unsigned int label of descriptor i with filter f
	std::vector<std::string> Labels; // Labels[l] = name of the label l
	unsigned int *LabelsSize; // [f*Labels.size()+l] = number of descriptor having label l in filter f
	// used to filtering the data (none by default but could be by element for instance i.e. the descriptor list is separate for the different types)
	unsigned int nbFilter = 0; //
	std::string FilteringType = "none";
	std::vector<std::string> FilterValue; // FilterValue[f] = name of the f filter
	unsigned int nbMaxFilter = 100;

public:
	// constructors
	Descriptors(){};
	Descriptors(const std::string& FilenameOrDir); // constructor from file reading 
	Descriptors(const std::string& FilenameOrDir, const std::string& DescriptorName, const std::string& _FilteringType); // constructor from file reading 
	// methods
	void printDescriptorsPropToDatabase(std::ofstream &writefile);
	// getters
	double *getDescriptors(){ return _Descriptors; }
	unsigned int *getLabels_uint(){ return Labels_uint; }
	unsigned int getLabelsSize(const unsigned int &f, unsigned int &l){ return LabelsSize[f*Labels.size()+l]; }
	unsigned int getNbLabels(){ return Labels.size(); }
	std::string getLabels(const unsigned int &l){ return Labels[l]; }
	unsigned int getDim(){ return dim; }
	unsigned int getNbDat(const unsigned int &f){ return nbDat[f]; }
	unsigned int getNbDatMax(){ return nbDatMax; }
	unsigned int getNbFilter(){ return nbFilter; }
	std::string getFilteringType(){ return FilteringType; }
	std::string getFilterValue(const unsigned int &f){ return FilterValue[f]; }
	// destructor
	~Descriptors();
	
};

#endif
