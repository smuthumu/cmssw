#include <TFile.h>
#include <TTree.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;

//generalization for processing a line
void process(string line, char delim, vector<string>& fields){
	stringstream ss(line);
	string field;
	while(getline(ss,field,delim)){
		fields.push_back(field);
	}
}

template <class O> O getOptionValue(const string& val){
	stringstream sval(val);
	O tmp;
	sval >> tmp;
	return tmp;
}

void LogToTreeCaloSamples(string name = "step2_DebugCaloSamples"){
	//open input text file
	ifstream infile((name+".log").c_str());
	string line;
	
	//variables
	uint32_t id = 0;
	int subdet = 0;
	int ieta = 0;
	int iphi = 0;
	int depth = 0;
	vector<double>* frame1 = 0;
	vector<double>* frame2 = 0;
	vector<double>* frame3 = 0;
	vector<double>* frame4 = 0;
	vector<double>* frame5 = 0;
	
	//create output root file and tree
	TFile* outfile = TFile::Open((name+".root").c_str(),"RECREATE");
	TTree* tree = new TTree("tree","Hcal SiPM ntuple");
	tree->Branch("id"     , &id              , "id/i");
	tree->Branch("subdet" , &subdet          , "subdet/I");
	tree->Branch("ieta"   , &ieta            , "ieta/I");
	tree->Branch("iphi"   , &iphi            , "iphi/I");
	tree->Branch("depth"  , &depth           , "depth/I");
	tree->Branch("frame1" , "vector<double>" , &frame1);
	tree->Branch("frame2" , "vector<double>" , &frame2);
	tree->Branch("frame3" , "vector<double>" , &frame3);
	tree->Branch("frame4" , "vector<double>" , &frame4);
	tree->Branch("frame5" , "vector<double>" , &frame5);

	//keep lines that begin with header, feed into tree
	int nlines = 0;
	if(infile.is_open()){
		while(getline(infile,line)){
			++nlines;
			if(nlines%10000==0) cout << "read " << nlines << endl;
			if(line.compare(0,10,"CaloSample")==0){
				vector<string> fields;
				process(line,' ',fields);
				if(fields.size()<3) continue;
				if(fields[1]=="DetId:"){
					tree->Fill();
					//reset
					id                     = 0;
					subdet                 = 0;
					ieta                   = 0;
					iphi                   = 0;
					depth                  = 0;
					
					delete frame1; frame1 = new vector<double>();
					delete frame2; frame2 = new vector<double>();
					delete frame3; frame3 = new vector<double>();
					delete frame4; frame4 = new vector<double>();
					delete frame5; frame5 = new vector<double>();

					id     = getOptionValue<uint32_t>(fields[2]);
					if(fields.size()==3) continue;
					subdet = getOptionValue<int>(fields[3]);
					ieta   = getOptionValue<int>(fields[4]);
					iphi   = getOptionValue<int>(fields[5]);
					depth  = getOptionValue<int>(fields[6]);
					
				}
				else if(fields[1]=="HitResponse:" ) transform(fields.begin()+2,fields.end(),back_inserter(*frame1),getOptionValue<double>);
				else if(fields[1]=="pe2fC:"       ) transform(fields.begin()+2,fields.end(),back_inserter(*frame2),getOptionValue<double>);
				else if(fields[1]=="TimeSlew:"    ) transform(fields.begin()+2,fields.end(),back_inserter(*frame3),getOptionValue<double>);
				else if(fields[1]=="addPedestals:") transform(fields.begin()+2,fields.end(),back_inserter(*frame4),getOptionValue<double>);
				else if(fields[1]=="fC2adc:"      ) transform(fields.begin()+2,fields.end(),back_inserter(*frame5),getOptionValue<double>);

			}
		}
		//last fill
		tree->Fill();
	}
	
	infile.close();
	
	//save output
	outfile->cd();
	tree->Write();
	outfile->Close();
}