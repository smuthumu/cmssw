#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TDirectory.h>
#include <TF1.h>
#include <TPaveStats.h>

void fitGeant4TimePDF(){
	TFile* _file0 = TFile::Open("step2prec1K_HcalSiPMntuple.root");
	TTree* tree = (TTree*)_file0->Get("tree");
	TCanvas* can = new TCanvas("Geant4TimePDF","Geant4TimePDF");
	can->SetLogy();
	//tree->Draw("log(Sum$((time-tof)*energy)/Sum$(energy))>>htemp","subdet==2&&Sum$(time-tof)>0&&(signalTot[4]+signalTot[5])*fCtoGeV*photoelectronsToAnalog>0&&(signalTot[4]+signalTot[5])*fCtoGeV*photoelectronsToAnalog<100");
	tree->Draw("log(time-tof)>>htemp","subdet==2&&time-tof>0&&(signalTot[4]+signalTot[5])*fCtoGeV*photoelectronsToAnalog>0&&(signalTot[4]+signalTot[5])*fCtoGeV*photoelectronsToAnalog<100");
	TH1F* htemp = (TH1F*)gDirectory->Get("htemp");
	TF1* gaus3 = new TF1("gaus3","gaus(0)+gaus(3)",htemp->GetXaxis()->GetXmin(),htemp->GetXaxis()->GetXmax());
	//gaus3->SetParameters(4e3,1,2,3e3,4,2);
	gaus3->SetParameters(1e4,0,2,3e4,4,2);
	htemp->Fit(gaus3);
	htemp->SetTitle("");
	can->Update();
	TPaveStats *ps = (TPaveStats*)(htemp->GetListOfFunctions()->FindObject("stats"));
	//move stat box
	ps->SetX1NDC(0.6);
	ps->SetY1NDC(0.45);
	ps->SetX2NDC(0.85);
	ps->SetY2NDC(0.65);
	can->Modified();
	can->Update();
}