#include "DBScan.h"
#include "AtomHicConfig.h"
#include <cmath>

using namespace std; 

DBScan::DBScan(){
	this->name = "DBScan"; 
	readFixedParams();
}

void DBScan::setDescriptors(Descriptors *D){
	MachineLearningModel::setDescriptors(D);
	this->IsDescriptor = true;
}

void DBScan::TrainModel(string filter_name){
	if( !IsDescriptor ){
		cerr << "The ML model does not have descriptors, training aborted" << endl;
		exit(EXIT_FAILURE);
	}

	if( !_MyDescriptors->getIsNeighbours(filter_name) || _MyDescriptors->get_current_rc(filter_name) != eps ){
		_MyDescriptors->searchNeighbours(eps,filter_name);
	}
	unsigned int current_filter;
	bool found = false;
	for(unsigned int f=0;f<nbFilter;f++){
		if( FilterValue[f] == filter_name ){
			current_filter = f;
			found = true;
			break;
		}
	}
	if( !found ){
		cerr << "The provided filter value (" << filter_name << ") does not correspond to a filter value of the ML model, aborting" << endl;
		exit(EXIT_FAILURE);
	}
	unsigned int current_filter_neigh;
	unsigned int nbNMax = _MyDescriptors->getNbMaxNAndFilter(filter_name,current_filter_neigh);
	unsigned int nbDes = nbDat[current_filter];
	double mean_minPts = 0.;
	for(unsigned int i=0;i<nbDes;i++) mean_minPts += _MyDescriptors->getNeighbours(current_filter_neigh)[i*(nbNMax+1)];
	mean_minPts /= (double) nbDes;
	minPts = (unsigned int) mean_minPts;
	minPts--;
	cout << "Average number of neighbours : " << minPts << endl;

	cout << "Performing density based clustering.." << endl;
	unsigned int nbDatTot = 0;
        for(unsigned int f=0;f<nbFilter;f++) nbDatTot += nbDat[f];
        if( !IsClassified ){
                IsClassified = true;
                Classificator = new double[2*nbDatTot]; // first column, id of cluster ([n*2]) : -1 => undefined, 0 => noisy, i => id of cluster, second column status of point ([n*2+1]) : core (1), outlier (0) point -10 not treated (filter)
		for(unsigned int i=0;i<nbDatTot*2;i++) Classificator[i] = -10;
        }
	// DBScan algorythm
	// initialize status of point
	for(unsigned int i=0;i<nbDes;i++){
		unsigned int ind = _MyDescriptors->getFilterIndex(current_filter*nbDatMax+i);
		Classificator[ind*2] = -1;
		if( _MyDescriptors->getNeighbours(current_filter_neigh)[i*(nbNMax+1)] > minPts ) Classificator[ind*2+1] = 1;
		else Classificator[ind*2+1] = 0;
	}
	vector<unsigned int> stack;
	unsigned int clust_count = 1;
	unsigned int *temp_arr_neigh = new unsigned int[nbNMax];
	unsigned int current_nbneigh;
	for(unsigned int i=0;i<nbDes;i++){
		unsigned int ind = _MyDescriptors->getFilterIndex(current_filter*nbDatMax+i);
		if( Classificator[ind*2] != -1 || Classificator[ind*2+1] == 0 ) continue;
		unsigned int this_i = i;
		while( true ){
			if( Classificator[ind*2] == -1 ){
				Classificator[ind*2] = clust_count;
				if( Classificator[ind*2+1] == 1 ){
					current_nbneigh = _MyDescriptors->getNeighbours(current_filter_neigh)[i*(nbNMax+1)];
					for(unsigned int n=0;n<current_nbneigh;n++) temp_arr_neigh[n] = _MyDescriptors->getNeighbours(current_filter_neigh)[i*(nbNMax+1)+1+n];
					for(unsigned int i2=0;i2<current_nbneigh;i2++){
						unsigned int ind2 = _MyDescriptors->getFilterIndex(current_filter*nbDatMax+temp_arr_neigh[i2]);
						if( Classificator[ind2*2] == -1 ) stack.push_back(temp_arr_neigh[i2]);
					}
				}
			}
			if( stack.size() == 0 ) break;
			i = stack.back();
			ind = _MyDescriptors->getFilterIndex(current_filter*nbDatMax+i);
			stack.pop_back();
		}
		clust_count++;
		i = this_i;
	}
	delete[] temp_arr_neigh;	
	cout << "Done !" << endl;
        cout << "Number of cluster found : " << clust_count-1 << endl;
}

void DBScan::readFixedParams(){
	string fp;
	#ifdef FIXEDPARAMETERS
	fp = FIXEDPARAMETERS;
	#endif
	string backslash="/";
	string filename=fp+backslash+FixedParam_Filename;
	ifstream file(filename, ios::in);
	size_t pos_eps, pos_minPts;
	string buffer_s, line;
	unsigned int ReadOk(0);
	if(file){
		while(file){
			getline(file,line);
			pos_eps=line.find("DBSCAN_EPS");
			if(pos_eps!=string::npos){
				istringstream text(line);
				text >> buffer_s >> eps;
				ReadOk++;
			}
			pos_minPts=line.find("DBSCAN_MINPTS");
			if(pos_minPts!=string::npos){
				istringstream text(line);
				text >> buffer_s >> minPts;
				ReadOk++;
			}
		}
	}else{
		cerr << "Can't read /data/FixedParameters/Fixed_Parameters.dat file !" << endl;
		exit(EXIT_FAILURE);
	}
	file.close();
	if( ReadOk != 2 ){
		cerr << "Error during reading of FixedParameters.dat for DBScan, aborting" << endl;
		exit(EXIT_FAILURE);
	}
}

void DBScan::ReadProperties(vector<string> Properties){
	size_t pos_eps, pos_minPts;
	string buffer_s;
	for(unsigned int i=0;i<Properties.size();i++){
		pos_eps=Properties[i].find("DBSCAN_EPS");
		if(pos_eps!=string::npos){
			istringstream text(Properties[i]);
			text >> buffer_s >> eps;
		}
		pos_minPts=Properties[i].find("DBSCAN_MINPTS");
		if(pos_minPts!=string::npos){
			istringstream text(Properties[i]);
			text >> buffer_s >> minPts;
		}
	}
}

DBScan::~DBScan(){
	if( this->IsDescriptor ){

	}
}