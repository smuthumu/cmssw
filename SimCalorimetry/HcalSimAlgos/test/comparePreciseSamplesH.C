#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TDirectory.h>

//pulse shape vectors
#include "pulses.h"

#include <string>
#include <sstream>
#include <vector>

using namespace std;

TH1F* makePulseHisto(vector<double>& pulse, int bstart, string name){
	//make pulse template histo (from convolution)
	TH1F* htempT = new TH1F(name.c_str(),"",250,0,250);
	for(unsigned b = 0; b < pulse.size(); ++b){
		htempT->SetBinContent(b+bstart,pulse[b]);
	}
	htempT->Scale(1.0/htempT->Integral(0,htempT->GetNbinsX()+1));
	htempT->SetMarkerColor(kBlack);
	htempT->SetMarkerSize(0.75);
	htempT->GetXaxis()->SetTitle("time [ns]");
	htempT->GetYaxis()->SetTitle("a.u.");

	return htempT;
}

void comparePreciseSamplesH(vector<string> fnames={"step2prec_HcalSiPMntuple.root"}, vector<string> names={"step2prec"}, double emin=0, double emax=0, int depth=0, bool hpd=false){
	int legynum = fnames.size();
	legynum += 2;
	if(depth>0) legynum += 1;
	if(hpd) legynum += 1;
	float textSize = 0.04;
	float legx = hpd ? 0.68 : 0.18;
	TLegend* leg = new TLegend(legx,0.92-textSize*legynum,legx+0.22,0.92);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(42);
	leg->SetTextSize(textSize);
	double yminmin = 1e-8;

	//make pulse template histo (from convolution)
	TH1F* htempT = NULL;
	TH1F* htempT2 = NULL;
	if(hpd){
		htempT = makePulseHisto(s105,72,"reco");
		leg->AddEntry(htempT,"105 [HPD RECO]","p");
		htempT2 = makePulseHisto(s125,72,"sim");
		htempT2->SetMarkerColor(kRed);
		leg->AddEntry(htempT2,"125 [HPD SIM]","p");
	}
	else {
		htempT = makePulseHisto(s203,95,"reco");
		leg->AddEntry(htempT,"203 [Conv(SiPM,Y11)]","p");
	}
	double ymax = max(htempT->GetMaximum(),htempT2?htempT2->GetMaximum():0.0);
	double ymin = min(htempT->GetMinimum(0),htempT2?htempT2->GetMinimum(0):yminmin);
	
	stringstream scutleg;
	scutleg << emin << " < E_{M0} < " << emax << " GeV";
	leg->AddEntry((TObject*)NULL,scutleg.str().c_str(),"");
	stringstream scutleg2;
	scutleg2 << "depth " << depth;
	if(depth>0) leg->AddEntry((TObject*)NULL,scutleg2.str().c_str(),"");
	
	//histo name
	stringstream hs;
	hs << "signalTotPrecise_" << emin << "to" << emax;
	if(depth>0) hs << "_depth" << depth;
	
	stringstream sout;
	sout << hs.str() << "_";
	
	Color_t colors[] = {kBlue,kMagenta,kCyan+1,kOrange+1};
	Color_t markers[] = {20,24,21,25};
	vector<TH1F*> htemp(fnames.size(),NULL);
	for(unsigned i = 0; i < fnames.size(); ++i){
		TFile* file = TFile::Open(fnames[i].c_str());
		htemp[i] = (TH1F*)file->Get(hs.str().c_str());
		htemp[i]->SetDirectory(NULL);
		htemp[i]->Scale(1.0/htemp[i]->Integral(0,htemp[i]->GetNbinsX()+1));
		htemp[i]->SetMarkerColor(colors[i]);
		htemp[i]->SetMarkerStyle(markers[i]);
		htemp[i]->SetMarkerSize(0.75);
		leg->AddEntry(htemp[i],names[i].c_str(),"p");
		
		if(ymax<htemp[i]->GetMaximum()) ymax = htemp[i]->GetMaximum();
		if(ymin>htemp[i]->GetMinimum()) ymin = htemp[i]->GetMinimum(0);
		
		sout << names[i];
		if(i<fnames.size()-1) sout << "_";
		file->Close();
	}
	
	TCanvas* can = new TCanvas(sout.str().c_str(),sout.str().c_str());
	can->SetLogy();
	
	if(ymin<yminmin) ymin = yminmin;
	htempT->GetYaxis()->SetRangeUser(ymin*0.9,ymax*5);
	htempT->Draw("hist p");
	if(htempT2) htempT2->Draw("hist p same");
	leg->Draw("same");
	for(unsigned i = 0; i < fnames.size(); ++i){
		htemp[i]->Draw("hist p same");
	}
	
	can->Print((sout.str()+".png").c_str(),"png");
	
}