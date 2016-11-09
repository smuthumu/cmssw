#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TGraphErrors.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>

#ifndef DigiClass_cxx
#define DigiClass_cxx
#include "DigiClass.h"

#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <iostream>

using namespace std;

void DigiClass::Loop() {}

void plotSinglePulse(vector<double>& data, string legname, string outname){
	TCanvas* can = new TCanvas("ctemp","ctemp");
	TH1F* htemp = new TH1F("pulse","",10,-0.5,9.5);
	for(unsigned x = 0; x < htemp->GetNbinsX(); ++x){
		htemp->SetBinContent(x+1,data[x]);
	}
	double ymin = *(min_element(data.begin(),data.end()));
	double ymax = *(max_element(data.begin(),data.end()));
	htemp->GetXaxis()->SetTitle("TS");
	htemp->GetYaxis()->SetTitle("pe");
	htemp->GetYaxis()->SetRangeUser(max(ymin*0.9,0.0),ymax*1.1);
//	htemp->GetYaxis()->SetRangeUser(0,128);
	htemp->Draw("hist");

	TPaveText* pave = new TPaveText(0.25,0.925,0.75,0.975,"NDC");
	pave->AddText(legname.c_str());
	pave->SetFillColor(0);
	pave->SetBorderSize(0);
	pave->SetTextFont(42);
	pave->SetTextSize(0.05);
	pave->Draw("same");
	
	can->Print((outname+".png").c_str(),"png");
	can->Close();
	delete htemp;
	delete can;
	delete pave;
}

class DigiClass2 : public DigiClass {
	public:
		DigiClass2(TTree *tree=0) : DigiClass(tree) {}
		void Loop(int cut, bool hpd, string plotalldir){
			if (fChain == 0) return;
			
			Long64_t nentries = fChain->GetEntriesFast();

			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(jentry % 10000 == 0) cout << "Skimming " << jentry << "/" << nentries << endl;
				if(signalTot->size()==0) continue;
				if(subdet!=2) continue;
				//if(signalTot->at(4) <= cut) continue; //cut on SOI
				if( (!hpd && (signalTot->at(4)+signalTot->at(5))*fCtoGeV*photoelectronsToAnalog <= cut) ||
					(hpd && (signalTot->at(4)+signalTot->at(5))*0.23*0.3305 <= cut) ) continue; //cut on sim equiv of M0 energy
				
				stringstream ss;
				ss << "subdet = " << subdet << " i#eta = " << ieta << " i#phi = " << iphi << " d = " << depth;
				stringstream ss2;
				ss2 << plotalldir << "/event" << jentry << "_subdet" << subdet << "_ieta" << ieta << "_iphi" << iphi << "_depth" << depth;

				if(plotalldir.size()>0) plotSinglePulse(*signalTot,ss.str(),ss2.str());
			}
			
			return;
		}		
};

void plotPulsesCalo(string fname, int cut){
	TFile* file = TFile::Open(fname.c_str());
	if(!file) return;
	TTree* tree = (TTree*)file->Get("tree");
	DigiClass2* dc = new DigiClass2(tree);
	string plotalldir = "pulses_"+fname.substr(0,fname.size()-5);
	system(("mkdir -p "+plotalldir).c_str());
	bool hpd = (fname.find("HPD")!=string::npos);
	dc->Loop(cut,hpd,plotalldir);
	system(("tar -czf "+plotalldir+".tar.gz "+plotalldir+"/").c_str());
}

#endif