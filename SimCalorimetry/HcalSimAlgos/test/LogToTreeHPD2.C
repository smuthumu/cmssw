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

void LogToTreeHPD2(string name = "step2hpd_HcalHPDntuple"){
	//open input text file
	ifstream infile((name+".log").c_str());
	string line;
	
	//variables
	int nevent = 0;
	uint32_t id = 0;
	int subdet = 0;
	int ieta = 0;
	int iphi = 0;
	int depth = 0;
	std::vector<double>* energy = 0;
	std::vector<int>* photons = 0;
	std::vector<double>* time = 0;
	std::vector<double>* tof = 0;
	std::vector<double>* tzero = 0;
	std::vector<double>* signalTot = 0;
	std::vector<double>* signalTotPrecise = 0;
	
	//create output root file and tree
	TFile* outfile = TFile::Open((name+".root").c_str(),"RECREATE");
	TTree* tree = new TTree("tree","Hcal SiPM ntuple");
	tree->Branch("event"            , &nevent          , "nevent/I");
	tree->Branch("id"               , &id              , "id/i");
	tree->Branch("subdet"           , &subdet          , "subdet/I");
	tree->Branch("ieta"             , &ieta            , "ieta/I");
	tree->Branch("iphi"             , &iphi            , "iphi/I");
	tree->Branch("depth"            , &depth           , "depth/I");
	tree->Branch("energy"           , "vector<double>" , &energy         );
	tree->Branch("photons"          , "vector<int>"    , &photons        );
	tree->Branch("time"             , "vector<double>" , &time           );
	tree->Branch("tof"              , "vector<double>" , &tof            );
	tree->Branch("tzero"            , "vector<double>" , &tzero          );
	tree->Branch("signalTot"        , "vector<double>" , &signalTot);
	tree->Branch("signalTotPrecise" , "vector<double>" , &signalTotPrecise);

	//keep lines that begin with header, feed into tree
	int nlines = 0;
	if(infile.is_open()){
		while(getline(infile,line)){
			++nlines;
			if(nlines%10000==0) cout << "read " << nlines << endl;
			if(line.compare(0,14,"HcalHPD2ntuple")==0){
				vector<string> fields;
				process(line,' ',fields);
				if(fields.size()<3) continue;
				if(fields[1]=="event"){
					tree->Fill();
					//reset
					id      = 0;
					subdet  = 0;
					ieta    = 0;
					iphi    = 0;
					depth   = 0;
					delete energy          ; energy           = new vector<double>();
					delete photons         ; photons          = new vector<int>();
					delete time            ; time             = new vector<double>();
					delete tof             ; tof              = new vector<double>();
					delete tzero           ; tzero            = new vector<double>();					
					delete signalTot       ; signalTot        = new vector<double>();
					delete signalTotPrecise; signalTotPrecise = new vector<double>();

					nevent = getOptionValue<int>(fields[2]);
				}
				else if(fields[1]=="id"               ) id      = getOptionValue<uint32_t>(fields[2]);
				else if(fields[1]=="subdet"           ) subdet  = getOptionValue<int>(fields[2]);
				else if(fields[1]=="ieta"             ) ieta    = getOptionValue<int>(fields[2]);
				else if(fields[1]=="iphi"             ) iphi    = getOptionValue<int>(fields[2]);
				else if(fields[1]=="depth"            ) depth   = getOptionValue<int>(fields[2]);
				else if(fields[1]=="energy"           ) transform(fields.begin()+2,fields.end(),back_inserter(*energy         ),getOptionValue<double>);
				else if(fields[1]=="photons"          ) transform(fields.begin()+2,fields.end(),back_inserter(*photons        ),getOptionValue<int>);
				else if(fields[1]=="time"             ) transform(fields.begin()+2,fields.end(),back_inserter(*time           ),getOptionValue<double>);
				else if(fields[1]=="tof"              ) transform(fields.begin()+2,fields.end(),back_inserter(*tof            ),getOptionValue<double>);
				else if(fields[1]=="tzero"            ) transform(fields.begin()+2,fields.end(),back_inserter(*tzero          ),getOptionValue<double>);
				else if(fields[1]=="signalTot"        ) transform(fields.begin()+2,fields.end(),back_inserter(*signalTot       ),getOptionValue<double>);
				else if(fields[1]=="signalTotPrecise" ) transform(fields.begin()+2,fields.end(),back_inserter(*signalTotPrecise),getOptionValue<double>);
				
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