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

void comparePreciseSamplesE(string fname="step2prec_HcalSiPMntuple.root", string name="step2prec", int depth=0, bool hpd=false){
	//get and check histos from file
	string hpre = "signalTotPrecise";
	vector<double> emin = {0,100,200,300,400,500};
	vector<double> emax = {100,200,300,400,500,600};
	TFile* file = TFile::Open(fname.c_str());
	vector<TH1F*> hists;
	vector<string> hist_leg;
	for(unsigned i = 0; i < emin.size(); ++i){
		stringstream hs;
		hs << hpre << "_" << emin[i] << "to" << emax[i];
		TH1F* htemp = (TH1F*)file->Get(hs.str().c_str());
		htemp->SetDirectory(NULL);
		stringstream scutleg;
		scutleg << emin[i] << " < E_{M0} < " << emax[i];
		if(htemp->GetEntries()>0) {
			hists.push_back(htemp);
			hist_leg.push_back(scutleg.str().c_str());
		}
	}
	file->Close();
	
	int legynum = 2;
	if(depth>0) legynum += 1;
	if(hpd) legynum += 1;
	legynum += hists.size();
	float textSize = 0.04;
	//float legx = hpd ? 0.68 : 0.18;
	float legx = 0.71;
	TLegend* leg = new TLegend(legx,0.92-(textSize+0.005)*legynum,1.0,0.92);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(42);
	leg->SetTextSize(textSize);
	leg->SetMargin(0.15);
	double yminmin = 1e-8;

	//make pulse template histo (from convolution)
	TH1F* htempT = NULL;
	TH1F* htempT2 = NULL;
	if(hpd){
		htempT = makePulseHisto(s105,70,"reco");
		leg->AddEntry(htempT,"105 [HPD RECO]","p");
		htempT2 = makePulseHisto(s125,70,"sim");
		htempT2->SetMarkerColor(kGray+1);
		leg->AddEntry(htempT2,"125 [HPD SIM]","p");
	}
	else {
		if(name.find("250")!=string::npos) htempT = makePulseHisto(s203,1,"reco");
		else htempT = makePulseHisto(s203,95,"reco");
		leg->AddEntry(htempT,"203 [Conv(SiPM,Y11)]","p");
	}
	double ymax = max(htempT->GetMaximum(),htempT2?htempT2->GetMaximum():0.0);
	double ymin = min(htempT->GetMinimum(0),htempT2?htempT2->GetMinimum(0):yminmin);
	
	leg->AddEntry((TObject*)NULL,name.c_str(),"");
	stringstream scutleg2;
	scutleg2 << "depth " << depth;
	if(depth>0) leg->AddEntry((TObject*)NULL,scutleg2.str().c_str(),"");
	
	//histo name	
	stringstream sout;
	sout << hpre << "_vsE_" << name;
	if(depth>0) sout << "_depth" << depth;
	
	Color_t colors[] = {kBlue,kMagenta+3,kMagenta,kRed,kOrange+1,kCyan+1};
	for(unsigned i = 0; i < hists.size(); ++i){
		hists[i]->Scale(1.0/hists[i]->Integral(0,hists[i]->GetNbinsX()+1));
		hists[i]->SetMarkerColor(colors[i]);
		hists[i]->SetMarkerStyle(24);
		hists[i]->SetMarkerSize(0.75);
		leg->AddEntry(hists[i],hist_leg[i].c_str(),"p");
		
		if(ymax<hists[i]->GetMaximum()) ymax = hists[i]->GetMaximum();
		if(ymin>hists[i]->GetMinimum()) ymin = hists[i]->GetMinimum(0);		
	}
	
	TCanvas* can = new TCanvas(sout.str().c_str(),sout.str().c_str(),850,500);
	can->SetLogy();
	can->SetRightMargin(0.3);
	
	if(ymin<yminmin) ymin = yminmin;
	htempT->GetYaxis()->SetRangeUser(ymin*0.9,ymax*5);
	htempT->Draw("hist p");
	if(htempT2) htempT2->Draw("hist p same");
	leg->Draw("same");
	for(unsigned i = 0; i < hists.size(); ++i){
		hists[i]->Draw("hist p same");
	}
	
	can->Print((sout.str()+".png").c_str(),"png");
	
}