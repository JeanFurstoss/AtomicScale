#ifndef GAUSSIANMIXTUREMODEL_H
#define GAUSSIANMIXTUREMODEL_H

#include "AtomHicExport.h"
#include "AtomicSystem.h"
#include "MathTools.h"
#include <string>
#include "MachineLearningModel.h"
#include "KMeans.h"

class ATOMHIC_EXPORT GaussianMixtureModel : public MachineLearningModel {
private:
	bool IsDescriptor = false;
	unsigned int *nbClust; // [f] number of cluster in the GMM with filter f
	double *weights; // weights[k*nbFilter+f] weight of cluster k with filter f
	double *mu; // mu[k*dim*nbFilter+d*nbFilter+f] d component of the mean of cluster k with filter f
	long double *V; // V[k*dim2*nbFilter+d1*dim*nbFilter+d2*nbFilter+f] d1,d2 component of variance of cluster k with filter f
	long double *V_inv;
	long double *det_V;
	long double *LogLikelihood;
	double *BIC;
	
	// old variables for EM iterations
	double *weights_old;
	double *mu_old;
	long double *V_old;
	long double *V_inv_old;
	long double *det_V_old;
	
	// saved variables for training with multiple number of clusters
	unsigned int *saved_nbClust; // [i*nbFilter+f] number of cluster for run i and filter f
	double *saved_bic;
	double *saved_weights; // saved_weight[c*nbMaxClusters*nbFilter+k*nbFilter+f] = weight of cluster k for run number c with filter f
	double *saved_mu; // saved_mu[c*nbMaxClusters*dim*nbFilter+k*dim*nbFilter+d*nbFilter+f]
	long double *saved_V; // saved_V[c*nbMaxClusters*dim2*nbFilter+k*dim2*nbFilter+d1*dim*nbFilter+d2*nbFilter+f]
	long double *saved_V_inv;
	long double *saved_det_V;
	bool SavedVariablesInitialized = false;

	// saved variables for unique training
	double *train_saved_bic; // [i*nbFilter+f] BIC for run i and filter f
	double *train_saved_weights; // saved_weight[c*nbMaxClusters*nbFilter+k*nbFilter+f] = weight of cluster k for run number c with filter f
	double *train_saved_mu; // saved_mu[c*nbMaxClusters*dim*nbFilter+k*dim*nbFilter+d*nbFilter+f]
	long double *train_saved_V; // saved_V[c*nbMaxClusters*dim2*nbFilter+k*dim2*nbFilter+d1*dim*nbFilter+d2*nbFilter+f]
	long double *train_saved_V_inv;
	long double *train_saved_det_V;
	bool TrainSavedVariablesInitialized = false;

	// buffers for EM	
	double *D_i;
	double *C_di;
	double *buffer_di;
	double *E_d;

	// Variables for the labelling of the GMM
	unsigned int *ClusterLabel; // ClusterLabel[k*nbFilter+f] = l
	long double *AveLabelProb; // AveLabelProb[k*nbFilter+f] = Average probability associated to the labelled cluster
	double tolLabelSize = .7; // tolerance for warning message for repartition of descriptor in labels
	double tol2ndProb = .01; // tolerance for warning message for 2nd prob after labeling with the highest prob

	bool IsKMeans = false;
	KMeans *MyKM;
	bool IsKMeansProperties = false;
	std::vector<std::string> KMeansProperties;

	// FixedParameters
	unsigned int nbMaxClusters = 100;
	double tol_Lkh_EM = 1e-4;
	unsigned int MaxIter_EM = 100;
	double fac_elbow = 0.1; // reduction factor for considering that its is a real elbow
	unsigned int nb_bic_increase = 1;
	std::string after_elbow_choice = "Max";
	unsigned int nbInit = 1; // number of random initialization (we keep at the end the one with the highest likelihood)
	std::string InitMethod = "KMEANSPP"; // number of random initialization (we keep at the end the one with the highest likelihood)

public:
	// constructors
	GaussianMixtureModel();
	// base methods of MachineLearningModels
	void setDescriptors(Descriptors *D);
	void LabelClassification();
	void Classify();
	void ChangeFilterIndex();
	void TrainModel(unsigned int &_nbClust, unsigned int &filter_value);
	void readFixedParams();
	// specific methods of GMM
	void fitOptimalGMM(unsigned int &_nbClust_min, unsigned int &_nbClust_max);
	void RandomInit(unsigned int &_nbClust, unsigned int &filter_value);
	void InitFromKMeans(unsigned int &_nbClust, unsigned int &filter_value);
	void InitFromKMeansPP(unsigned int &_nbClust, unsigned int &filter_value);
	void SaveVariables(unsigned int &current_nbClust, unsigned int &filter_value);
	void SaveTrainVariables(unsigned int &current_nbClust, unsigned int &filter_value);
	void SetOptimalModel(unsigned int &opt_index, unsigned int &filter_value);
	void SetOptimalTrainModel(unsigned int &opt_index, unsigned int &filter_value);
	void UpdateParams(unsigned int &filter_value);
	void ComputeLogLikelihood(unsigned int &filter_value);
	void ComputeBIC(unsigned int &filter_value);
	void PrintModelParams(std::string filename);
	void PrintModelParams(std::string filename, std::vector<std::string> label_order); // specifiy the order of the label to print
	void EM(unsigned int &filter_value);
	long double Prob_Cluster(unsigned int &index_cluster, unsigned int &DescriptorIndex, unsigned int &filter_value);
	double MaximumLikelihoodClassifier(unsigned int &index_cluster, unsigned int &DescriptorIndex, unsigned int &filter_value);
	void Labelling();
	void PrintToDatabase(const std::string &name_of_database);
	void ReadModelParamFromDatabase(const std::string &name_of_database);
	void ReadProperties(std::vector<std::string> Properties);
	void SetKMeansProperties(std::vector<std::string> Properties);
	// getters
	std::vector<std::string> getAvailableDatabases();
	unsigned int getNbClust(unsigned int &filter_value){ return nbClust[filter_value]; }
	double getBIC(unsigned int &filter_value){ return BIC[filter_value]; }
	double getLogLikelihood(unsigned int &filter_value){ return LogLikelihood[filter_value]; }
	KMeans *getKMeans(){ return MyKM; }
	long double *getCov(){ return V; }
	double *getMu(){ return mu; }
	unsigned int getNbMaxCluster(){ return nbMaxClusters; }
	// destructor
	~GaussianMixtureModel();
};

#endif
