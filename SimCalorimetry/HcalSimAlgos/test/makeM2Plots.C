#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TDirectory.h>
#include <TStyle.h>

#include <string>
#include <vector>

using namespace std;

void makeHisto(TTree* Events, string drawname, string hname, string cutname, string xname, string yname, string pre, bool h2){
	string oname = pre+"_"+hname;
	TCanvas* can = new TCanvas(oname.c_str(),oname.c_str());
	if(h2) {
		can->SetRightMargin(0.15);
		can->SetLogz();
		if(hname=="hM2M0" || hname=="hM2M0zoom") {
			can->SetGridx();
			can->SetGridy();
		}
	}
	else {
		can->SetLogy();
	}
	
	Events->Draw(drawname.c_str(),cutname.c_str(),h2?"colz":"");
	TH1* htemp = (TH1*)gDirectory->Get(hname.c_str());
	htemp->SetTitle("");
	htemp->GetXaxis()->SetTitle(xname.c_str());
	htemp->GetYaxis()->SetTitle(yname.c_str());
	if(!h2) htemp->SetLineWidth(2);
	can->Update();
	
	can->Print((oname+".png").c_str(),"png");
}

void makeM2Plots(string fname, string pre, string tag="HLT"){
	TFile *file = TFile::Open(fname.c_str());
	TTree* Events = (TTree*)file->Get("Events");
	
	string fulltag = "HBHERecHitsSorted_hbhereco__"+tag+".obj.obj";
	Events->SetAlias("subdet",(fulltag+".id().subdet()").c_str());
	Events->SetAlias("M2",(fulltag+".energy()").c_str());
	Events->SetAlias("M0",(fulltag+".eraw()").c_str());
	Events->SetAlias("chi2",(fulltag+".chi2()").c_str());
	Events->SetAlias("time",(fulltag+".time()").c_str());
	gStyle->SetPalette(57);
	
	makeHisto(Events,"M2:M0>>hM2M0(100,0,500,100,0,500)","hM2M0","subdet==2&&M2>0","M0 energy [GeV]","M2 energy [GeV]",pre,true);
	makeHisto(Events,"M2:M0>>hM2M0zoom(100,0,100,100,0,100)","hM2M0zoom","subdet==2&&M2>0","M0 energy [GeV]","M2 energy [GeV]",pre,true);
	makeHisto(Events,"time>>htime","htime","subdet==2&&M2>5","M2 time [ns]","events",pre,true);
	makeHisto(Events,"time:M2>>htimeM2","htimeM2","subdet==2&&M2>5","M2 energy [GeV]","M2 time [ns]",pre,true);
	makeHisto(Events,"log10(chi2)>>hlogchi2","hlogchi2","subdet==2&&M2>0","log10(#chi^{2})","events",pre,true);
	makeHisto(Events,"log10(chi2):M2>>hlogchi2M2","hlogchi2M2","subdet==2&&M2>0","M2 energy [GeV]","log10(#chi^{2})",pre,true);
}
