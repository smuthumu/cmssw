#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TGraphErrors.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>

#ifndef SiPMClass_cxx
#define SiPMClass_cxx
#endif
#include "SiPMClass.h"

#ifndef HPD2Class_cxx
#define HPD2Class_cxx
#endif
#include "HPD2Class.h"

#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <tuple>
#include <map>
#include <iostream>

using namespace std;

void SiPMClass::Loop() {}
void HPD2Class::Loop() {}

//depth, emin, emax
typedef tuple<int,double,double> BinIndex;
typedef pair<TH1F*,TH1F*> HistoPair;
typedef map<BinIndex,HistoPair> HistoMap;

HistoPair getHistos(HistoMap& hmap, BinIndex& bin){
	//tuple compare returns true if lhs[0] < rhs[0], or !(lhs[0] < rhs[0]) and lhs[1] < rhs[1], etc.
	//therefore lower bound usually returns one ahead in this case
	auto lb = hmap.lower_bound(bin);
	lb--;	
	BinIndex lbBin = lb->first;
	if(get<0>(bin) != get<0>(lbBin) || get<1>(bin) <= get<1>(lbBin) || get<2>(bin) > get<2>(lbBin)){
		//cout << "No bin found for (" << get<0>(bin) << "," << get<1>(bin) << "," << get<2>(bin) << ")" << endl;
		return make_pair((TH1F*)NULL,(TH1F*)NULL);
	}
	//else cout << "Bin found for (" << get<0>(bin) << "," << get<1>(bin) << "," << get<2>(bin) << ") : (" << get<0>(lbBin) << "," << get<1>(lbBin) << "," << get<2>(lbBin) << ")" << endl;
	return lb->second;
}

void fillHisto(TH1F* histo, vector<double>& signalTot, double tbin){
	if(!histo) return;
	for(unsigned i = 0; i < signalTot.size(); ++i){
		histo->Fill(i*tbin,signalTot[i]);
	}
}

void fillHistos(HistoMap& hmap, vector<double>& signalTotPrecise, vector<double>& signalTot, int depth, double gain, int subdet){
	if(subdet!=2) return;
	double EM0 = (signalTot[4]+signalTot[5])*gain;
	
	//first find depth-independent histos
	BinIndex bin0(0,EM0,EM0);
	HistoPair hpair0 = getHistos(hmap,bin0);
	fillHisto(hpair0.first,signalTotPrecise,0.5);
	fillHisto(hpair0.second,signalTot,25.);
	
	//now find depth-dependent histos
	BinIndex bin(depth,EM0,EM0);
	HistoPair hpair = getHistos(hmap,bin);
	fillHisto(hpair.first,signalTotPrecise,0.5);
	fillHisto(hpair.second,signalTot,25.);
}

class SiPMClass2 : public SiPMClass {
	public:
		SiPMClass2(TTree *tree=0) : SiPMClass(tree) {}
		void Loop(HistoMap& hmap){
			Long64_t nentries = fChain->GetEntriesFast();
			//nentries = 10000;

			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(jentry % 10000 == 0) cout << "Processing " << jentry << "/" << nentries << endl;
				
				fillHistos(hmap,*signalTotPrecise,*signalTot,depth,fCtoGeV*photoelectronsToAnalog,subdet);				
			}
		}
};

class HPD2Class2 : public HPD2Class {
	public:
		HPD2Class2(TTree *tree=0) : HPD2Class(tree) {}
		void Loop(HistoMap& hmap){
			Long64_t nentries = fChain->GetEntriesFast();

			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(jentry % 10000 == 0) cout << "Processing " << jentry << "/" << nentries << endl;
				
				fillHistos(hmap,*signalTotPrecise,*signalTot,depth,0.23*0.3305,subdet);				
			}
		}
};

void plotPulses(string fname){
	//initialize histo map
	TFile* outfile = TFile::Open(("histos_"+fname).c_str(),"RECREATE");
	HistoMap hmap;
	vector<int> depths = {0,1,2,3,4,5,6,7};
	vector<double> emin = {0,100,200,300,400,500};
	vector<double> emax = {100,200,300,400,500,600};
	for(unsigned i = 0; i < depths.size(); ++i){
		for(unsigned j = 0; j < emin.size(); ++j){
			stringstream hs;
			hs << emin[j] << "to" << emax[j];
			if(depths[i]>0) hs << "_depth" << depths[i];
			TH1F* h1 = new TH1F(("signalTotPrecise_"+hs.str()).c_str(),"",250,0,250);
			TH1F* h2 = new TH1F(("signalTot_"+hs.str()).c_str(),"",10,0,250);
			hmap.emplace(BinIndex(depths[i],emin[j],emax[j]),make_pair(h1,h2));
		}
	}

	TFile* file = TFile::Open(fname.c_str());
	TTree* tree = (TTree*)file->Get("tree");
	
	//check type
	bool sipm = false;
	if(fname.find("SiPM")!=string::npos) sipm = true;
	
	//trim the tree
	vector<string> branches = {"signalTot","signalTotPrecise","depth","subdet"};
	if(sipm) {
		branches.push_back("fCtoGeV");
		branches.push_back("photoelectronsToAnalog");
	}
	tree->SetBranchStatus("*",0);
	for(auto& branch : branches){
		tree->SetBranchStatus(branch.c_str(),1);
	}
	
	//loop
	if(sipm){
		SiPMClass2 looper(tree);
		looper.Loop(hmap);
	}
	else {
		HPD2Class2 looper(tree);
		looper.Loop(hmap);
	}
	
	//save histos in file
	outfile->cd();
	for(auto hentry : hmap){
		hentry.second.first->Write();
		hentry.second.second->Write();
	}
	outfile->Close();
}
