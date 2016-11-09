#include <string>

void drawT0VsPulse(string fname="step2hpd_HcalHPDntuple.root", bool doPrec=false, string pname=""){
	TFile* file = TFile::Open(fname.c_str());
	TTree* tree = (TTree*)file->Get("tree");
	
	TCanvas* can = new TCanvas("can","can", 800+4, 550+28);
	can->cd();
	TPad* pad1 = new TPad("pad","",0,0,1,1);
	double pad1W = pad1->GetWw()*pad1->GetAbsWNDC();
	double pad1H = pad1->GetWh()*pad1->GetAbsHNDC();
	pad1->SetMargin(95/pad1W,135/pad1W,75/pad1H,35/pad1H);
	pad1->SetLogz();
	pad1->Draw();
	pad1->cd();
	string drawname = ":Iteration$>>h2";
	if(doPrec) drawname += "(250,0,250)";
	else drawname += "(10,0,10)";
	if((fname.find("SiPM")!=string::npos || fname.find("HPD2")!=string::npos) && fname.find("single")==string::npos) drawname = "Sum$(tzero*photons)/Sum$(photons)"+drawname;
	else drawname = "tzero"+drawname;
	string cutname = "*(subdet==2)";
	if(doPrec) cutname = "signalTotPrecise[]"+cutname;
	else cutname = "signalTot[]"+cutname;
	if(fname.find("pu")!=string::npos) {
		if(fname.find("SiPM")!=string::npos) cutname += "*((signalTot[4]+signalTot[5])*fCtoGeV*photoelectronsToAnalog>5)";
		else if(fname.find("HPD")!=string::npos) cutname += "*((signalTot[4]+signalTot[5])*0.23*0.3305>200)";
	}
	tree->Draw(drawname.c_str(),cutname.c_str(),"colz goff");
	TH1* h2 = (TH1*)gDirectory->Get("h2");
	h2->SetTitle("");
	h2->GetXaxis()->SetTitle("TS");
	h2->GetXaxis()->SetRangeUser(0,10);
	h2->GetYaxis()->SetTitle("t_{0} [ns]");
	h2->GetYaxis()->SetTitleOffset(1.0);
	h2->GetYaxis()->SetRangeUser(-250,200);
	h2->GetZaxis()->SetTitle("pe");
	
	h2->Draw("colz");
	
	if(pname.size()>0) can->Print((pname+".png").c_str(),"png");
}