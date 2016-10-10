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

void LogToTree(string name = "step2_HcalSiPMntuple"){
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
	std::vector<double>* energy = 0;
	std::vector<int>* photons = 0;
	std::vector<double>* time = 0;
	std::vector<double>* tof = 0;
	std::vector<double>* tzero = 0;
	std::vector<double>* tzero_corrected = 0;
	std::vector<std::vector<double>>* t_pe = 0;
	std::vector<std::vector<int>>* t_bin = 0;
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
	TFile* outfile = TFile::Open((name+".root").c_str(),"RECREATE");
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
	tree->Branch("energy"                , "vector<double>"         , &energy         );
	tree->Branch("photons"               , "vector<int>"            , &photons        );
	tree->Branch("time"                  , "vector<double>"         , &time           );
	tree->Branch("tof"                   , "vector<double>"         , &tof            );
	tree->Branch("tzero"                 , "vector<double>"         , &tzero          );
	tree->Branch("tzero_corrected"       , "vector<double>"         , &tzero_corrected);
	tree->Branch("t_pe"                  , "vector<vector<double> >", &t_pe);
	tree->Branch("t_bin"                 , "vector<vector<int> >"   , &t_bin);
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
	if(infile.is_open()){
		while(getline(infile,line)){
			++nlines;
			if(nlines%10000==0) cout << "read " << nlines << endl;
			if(line.compare(0,14,"HcalSiPMntuple")==0){
				vector<string> fields;
				process(line,' ',fields);
				if(fields.size()<3) continue;
				if(fields[1]=="event"){
					tree->Fill();
					//reset
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
					
					delete energy         ; energy          = new vector<double>();
					delete photons        ; photons         = new vector<int>();
					delete time           ; time            = new vector<double>();
					delete tof            ; tof             = new vector<double>();
					delete tzero          ; tzero           = new vector<double>();
					delete tzero_corrected; tzero_corrected = new vector<double>();
					delete t_pe           ; t_pe            = new vector<vector<double>>();
					delete t_bin          ; t_bin           = new vector<vector<int>>();
					delete elapsedTime    ; elapsedTime     = new vector<double>();
					delete sampleBin      ; sampleBin       = new vector<int>();
					delete preciseBin     ; preciseBin      = new vector<int>();
					delete pe             ; pe              = new vector<int>();
					delete hitPixels      ; hitPixels       = new vector<int>();
					delete signal         ; signal          = new vector<vector<double>>();
					delete signalTot      ; signalTot       = new vector<double>();

					nevent = getOptionValue<int>(fields[2]);
				}
				else if(fields[1]=="id"                    ) id                     = getOptionValue<int>(fields[2]);
				else if(fields[1]=="subdet"                ) subdet                 = getOptionValue<int>(fields[2]);
				else if(fields[1]=="ieta"                  ) ieta                   = getOptionValue<int>(fields[2]);
				else if(fields[1]=="iphi"                  ) iphi                   = getOptionValue<int>(fields[2]);
				else if(fields[1]=="depth"                 ) depth                  = getOptionValue<int>(fields[2]);
				else if(fields[1]=="fCtoGeV"               ) fCtoGeV                = getOptionValue<double>(fields[2]);
				else if(fields[1]=="samplingFactor"        ) samplingFactor         = getOptionValue<double>(fields[2]);
				else if(fields[1]=="photoelectronsToAnalog") photoelectronsToAnalog = getOptionValue<double>(fields[2]);
				else if(fields[1]=="simhitToPhotoelectrons") simhitToPhotoelectrons = getOptionValue<double>(fields[2]);
				else if(fields[1]=="energy"                ) transform(fields.begin()+2,fields.end(),back_inserter(*energy         ),getOptionValue<double>);
				else if(fields[1]=="photons"               ) transform(fields.begin()+2,fields.end(),back_inserter(*photons        ),getOptionValue<double>);
				else if(fields[1]=="time"                  ) transform(fields.begin()+2,fields.end(),back_inserter(*time           ),getOptionValue<double>);
				else if(fields[1]=="tof"                   ) transform(fields.begin()+2,fields.end(),back_inserter(*tof            ),getOptionValue<double>);
				else if(fields[1]=="tzero"                 ) transform(fields.begin()+2,fields.end(),back_inserter(*tzero          ),getOptionValue<double>);
				else if(fields[1]=="tzero_corrected"       ) transform(fields.begin()+2,fields.end(),back_inserter(*tzero_corrected),getOptionValue<double>);
				else if(fields[1]=="t_pe"                  ) {
					//nentries per vector given by photons
					//cout << fields.size() << endl;
					int begin = 0;
					int end = 0;
					for(int i = 0; i < int(photons->size()); ++i){
						if(i>0) begin += photons->at(i-1);
						end += photons->at(i);
						//cout << begin << " " << end << endl;
						vector<double> stmp;
						transform(fields.begin()+2+begin,fields.begin()+2+end,back_inserter(stmp),getOptionValue<double>);
						t_pe->push_back(stmp);
					}
				}
				else if(fields[1]=="t_bin"                 ) {
					//nentries per vector given by photons
					int begin = 0;
					int end = 0;
					for(int i = 0; i < int(photons->size()); ++i){
						if(i>0) begin += photons->at(i-1);
						end += photons->at(i);
						vector<int> stmp;
						transform(fields.begin()+2+begin,fields.begin()+2+end,back_inserter(stmp),getOptionValue<int>);
						t_bin->push_back(stmp);
					}
				}
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