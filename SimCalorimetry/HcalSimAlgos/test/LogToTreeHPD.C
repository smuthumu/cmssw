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

void LogToTreeHPD(string name = "step2hpd_HcalHPDntuple"){
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
	double energy = 0;
	int photons = 0;
	double time = 0;
	double tof = 0;
	double tzero = 0;
	std::vector<double>* signalTot = 0;
	
	//create output root file and tree
	TFile* outfile = TFile::Open((name+".root").c_str(),"RECREATE");
	TTree* tree = new TTree("tree","Hcal SiPM ntuple");
	tree->Branch("event"     , &nevent          , "nevent/I");
	tree->Branch("id"        , &id              , "id/i");
	tree->Branch("subdet"    , &subdet          , "subdet/I");
	tree->Branch("ieta"      , &ieta            , "ieta/I");
	tree->Branch("iphi"      , &iphi            , "iphi/I");
	tree->Branch("depth"     , &depth           , "depth/I");
	tree->Branch("energy"    , &energy          , "energy/D");
	tree->Branch("photons"   , &photons         , "photons/I");
	tree->Branch("time"      , &time            , "time/D");
	tree->Branch("tof"       , &tof             , "tof/D");
	tree->Branch("tzero"     , &tzero           , "tzero/D");
	tree->Branch("signalTot" , "vector<double>" , &signalTot);

	//keep lines that begin with header, feed into tree
	int nlines = 0;
	if(infile.is_open()){
		while(getline(infile,line)){
			++nlines;
			if(nlines%10000==0) cout << "read " << nlines << endl;
			if(line.compare(0,13,"HcalHPDntuple")==0){
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
					energy  = 0;
					photons = 0;
					time    = 0;
					tof     = 0;
					tzero   = 0;
					
					delete signalTot      ; signalTot       = new vector<double>();

					nevent = getOptionValue<int>(fields[2]);
				}
				else if(fields[1]=="id"        ) id      = getOptionValue<uint32_t>(fields[2]);
				else if(fields[1]=="subdet"    ) subdet  = getOptionValue<int>(fields[2]);
				else if(fields[1]=="ieta"      ) ieta    = getOptionValue<int>(fields[2]);
				else if(fields[1]=="iphi"      ) iphi    = getOptionValue<int>(fields[2]);
				else if(fields[1]=="depth"     ) depth   = getOptionValue<int>(fields[2]);
				else if(fields[1]=="energy"    ) energy  = getOptionValue<double>(fields[2]);
				else if(fields[1]=="photons"   ) photons = getOptionValue<int>(fields[2]);
				else if(fields[1]=="time"      ) time    = getOptionValue<double>(fields[2]);
				else if(fields[1]=="tof"       ) tof     = getOptionValue<double>(fields[2]);
				else if(fields[1]=="tzero"     ) tzero   = getOptionValue<double>(fields[2]);
				else if(fields[1]=="signalTot" ) transform(fields.begin()+2,fields.end(),back_inserter(*signalTot  ),getOptionValue<double>);
				
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