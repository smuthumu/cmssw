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

void LogToTreeSingle(string name = "step2_HcalSiPMntuple"){
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
	double fCtoGeV = 0.;
	double samplingFactor = 0.;
	double photoelectronsToAnalog = 0.;
	double simhitToPhotoelectrons = 0.;
	double energy = 0.;
	int photons = 0;
	double time = 0.;
	double tof = 0.;
	double tzero = 0.;
	double tzero_corrected = 0.;
	std::vector<double>* t_pe = 0;
	std::vector<int>* t_bin = 0;
	std::vector<double>* elapsedTime = 0;
	std::vector<int>*    sampleBin = 0;
	std::vector<int>*    preciseBin = 0;
	std::vector<int>*    pe = 0;
	std::vector<int>*    hitPixels = 0;
	std::vector<std::vector<double>>* signal = 0;
	std::vector<double>* signalTot = 0;
	int sumPE = 0;
	int sumHits = 0;
	
	//create output root file and tree
	TFile* outfile = TFile::Open((name+"_single.root").c_str(),"RECREATE");
	TTree* tree = new TTree("tree","Hcal SiPM ntuple");
	tree->Branch("event"                 , &nevent                  , "nevent/I");
	tree->Branch("id"                    , &id                      , "id/i");
	tree->Branch("subdet"                , &subdet                  , "subdet/I");
	tree->Branch("ieta"                  , &ieta                    , "ieta/I");
	tree->Branch("iphi"                  , &iphi                    , "iphi/I");
	tree->Branch("depth"                 , &depth                   , "depth/I");
	tree->Branch("fCtoGeV"               , &fCtoGeV                 , "fCtoGeV/D");
	tree->Branch("samplingFactor"        , &samplingFactor          , "samplingFactor/D");
	tree->Branch("photoelectronsToAnalog", &photoelectronsToAnalog  , "photoelectronsToAnalog/D");
	tree->Branch("simhitToPhotoelectrons", &simhitToPhotoelectrons  , "simhitToPhotoelectrons/D");
	tree->Branch("energy"                , &energy                  , "energy/D"      );
	tree->Branch("photons"               , &photons                 , "photons/I"     );
	tree->Branch("time"                  , &time                    , "time/D"        );
	tree->Branch("tof"                   , &tof                     , "tof/D"         );
	tree->Branch("tzero"                 , &tzero                   , "tzero/D"       );
	tree->Branch("tzero_corrected"       , &tzero_corrected         , "tzero_corrected/D");
	tree->Branch("t_pe"                  , "vector<double>"         , &t_pe);
	tree->Branch("t_bin"                 , "vector<int>"            , &t_bin);
	tree->Branch("elapsedTime"           , "vector<double>"         , &elapsedTime);
	tree->Branch("sampleBin"             , "vector<int>"            , &sampleBin);
	tree->Branch("preciseBin"            , "vector<int>"            , &preciseBin);
	tree->Branch("pe"                    , "vector<int>"            , &pe);
	tree->Branch("hitPixels"             , "vector<int>"            , &hitPixels);
	tree->Branch("signal"                , "vector<vector<double> >", &signal);
	tree->Branch("signalTot"             , "vector<double>"         , &signalTot);
	tree->Branch("sumPE"                 , &sumPE                   , "sumPE/I");
	tree->Branch("sumHits"               , &sumHits                 , "sumHits/I");

	//keep lines that begin with header, feed into tree
	int nlines = 0;
	bool doFill = true;
	if(infile.is_open()){
		while(getline(infile,line)){
			++nlines;
			if(nlines%10000==0) cout << "read " << nlines << endl;
			if(line.compare(0,14,"HcalSiPMntuple")==0){
				vector<string> fields;
				process(line,' ',fields);
				if(fields.size()<3) continue;
				if(fields[1]=="event"){
					if(doFill) tree->Fill();
					//reset
					doFill = true;
					id                     = 0;
					subdet                 = 0;
					ieta                   = 0;
					iphi                   = 0;
					depth                  = 0;
					fCtoGeV                = 0;
					samplingFactor         = 0;
					photoelectronsToAnalog = 0;
					simhitToPhotoelectrons = 0;
					sumPE                  = 0;
					sumHits                = 0;
					
					energy          = 0;
					photons         = 0;
					time            = 0;
					tof             = 0;
					tzero           = 0;
					tzero_corrected = 0;
					delete t_pe           ; t_pe            = new vector<double>();
					delete t_bin          ; t_bin           = new vector<int>();
					delete elapsedTime    ; elapsedTime     = new vector<double>();
					delete sampleBin      ; sampleBin       = new vector<int>();
					delete preciseBin     ; preciseBin      = new vector<int>();
					delete pe             ; pe              = new vector<int>();
					delete hitPixels      ; hitPixels       = new vector<int>();
					delete signal         ; signal          = new vector<vector<double>>();
					delete signalTot      ; signalTot       = new vector<double>();

					nevent = getOptionValue<int>(fields[2]);
				}
				else if(fields[1]=="id"                    ) id                     = getOptionValue<uint32_t>(fields[2]);
				else if(fields[1]=="subdet"                ) subdet                 = getOptionValue<int>(fields[2]);
				else if(fields[1]=="ieta"                  ) ieta                   = getOptionValue<int>(fields[2]);
				else if(fields[1]=="iphi"                  ) iphi                   = getOptionValue<int>(fields[2]);
				else if(fields[1]=="depth"                 ) depth                  = getOptionValue<int>(fields[2]);
				else if(fields[1]=="fCtoGeV"               ) fCtoGeV                = getOptionValue<double>(fields[2]);
				else if(fields[1]=="samplingFactor"        ) samplingFactor         = getOptionValue<double>(fields[2]);
				else if(fields[1]=="photoelectronsToAnalog") photoelectronsToAnalog = getOptionValue<double>(fields[2]);
				else if(fields[1]=="simhitToPhotoelectrons") simhitToPhotoelectrons = getOptionValue<double>(fields[2]);
				else if(fields[1]=="energy"                ) energy                 = getOptionValue<double>(fields[2]);
				else if(fields[1]=="photons"               ) photons                = getOptionValue<int>(fields[2]);
				else if(fields[1]=="time"                  ) time                   = getOptionValue<double>(fields[2]);
				else if(fields[1]=="tof"                   ) tof                    = getOptionValue<double>(fields[2]);
				else if(fields[1]=="tzero"                 ) tzero                  = getOptionValue<double>(fields[2]);
				else if(fields[1]=="tzero_corrected"       ){tzero_corrected        = getOptionValue<double>(fields[2]); if(fields.size()>3) doFill = false; }
				else if(fields[1]=="t_pe"                  ) transform(fields.begin()+2,fields.end(),back_inserter(*t_pe           ),getOptionValue<double>);
				else if(fields[1]=="t_bin"                 ) transform(fields.begin()+2,fields.end(),back_inserter(*t_bin          ),getOptionValue<int>);
				else if(fields[1]=="elapsedTime"           ) transform(fields.begin()+2,fields.end(),back_inserter(*elapsedTime    ),getOptionValue<double>);
				else if(fields[1]=="sampleBin"             ) transform(fields.begin()+2,fields.end(),back_inserter(*sampleBin      ),getOptionValue<int>);
				else if(fields[1]=="preciseBin"            ) transform(fields.begin()+2,fields.end(),back_inserter(*preciseBin     ),getOptionValue<int>);
				else if(fields[1]=="pe"                    ) transform(fields.begin()+2,fields.end(),back_inserter(*pe             ),getOptionValue<int>);
				else if(fields[1]=="hitPixels"             ) transform(fields.begin()+2,fields.end(),back_inserter(*hitPixels      ),getOptionValue<int>);
				else if(fields[1]=="signal"                ) {
					for(unsigned i = 2; i <= fields.size()-10; i+=10){
						vector<double> stmp;
						transform(fields.begin()+i,fields.begin()+i+10,back_inserter(stmp),getOptionValue<double>);
						signal->push_back(stmp);
					}
				}
				else if(fields[1]=="signalTot"             ) transform(fields.begin()+2,fields.end(),back_inserter(*signalTot  ),getOptionValue<double>);
				else if(fields[1]=="sumPE"                 ) sumPE                  = getOptionValue<int>(fields[2]);
				else if(fields[1]=="sumHits"               ) sumHits                = getOptionValue<int>(fields[2]);
				
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