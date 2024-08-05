#include "MachineLearningModel.h"
#include "AtomHicConfig.h"

using namespace std;

MachineLearningModel::MachineLearningModel(){
	MT = new MathTools;
}

void MachineLearningModel::setDescriptors(Descriptors *D){ 
	_MyDescriptors = D;
	this->IsDescriptor = true;
	dim = _MyDescriptors->getDim();
	dim2 = dim*dim;
	nbDat = _MyDescriptors->getNbDat();
	buffer_vec_1_dim = new double[dim];
	buffer_vec_2_dim = new double[dim];
}

MachineLearningModel::~MachineLearningModel(){
	delete MT;
	delete[] buffer_vec_1_dim;
	delete[] buffer_vec_2_dim;
}