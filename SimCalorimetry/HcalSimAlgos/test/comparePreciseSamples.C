#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TDirectory.h>

#include <string>
#include <sstream>

using namespace std;

static const unsigned int nbin(128);
double s203[nbin] = {
	0.0,
	1.8863E-08,
	1.99991E-06,
	3.57815E-05,
	0.000216018,
	0.000718102,
	0.001657747,
	0.003054478,
	0.004858919,
	0.006988234,
	0.009347089,
	0.011836918,
	0.014362703,
	0.016839241,
	0.019193729,
	0.02136829,
	0.023319161,
	0.025016386,
	0.026442198,
	0.027589387,
	0.028459374,
	0.029060688,
	0.029407241,
	0.0295167,
	0.029409576,
	0.029108005,
	0.028634935,
	0.028013211,
	0.027265172,
	0.026412346,
	0.025475038,
	0.024471731,
	0.023419786,
	0.022334638,
	0.021230096,
	0.020118344,
	0.019009944,
	0.017914032,
	0.016838226,
	0.015788921,
	0.014771195,
	0.013789413,
	0.01284642,
	0.011944651,
	0.011085732,
	0.010270373,
	0.009499103,
	0.008771626,
	0.008087456,
	0.007445719,
	0.006845277,
	0.006284802,
	0.005762759,
	0.005277534,
	0.004827412,
	0.004410614,
	0.004025365,
	0.003669856,
	0.003342321,
	0.003041004,
	0.002764219,
	0.0025103,
	0.002277684,
	0.002064838,
	0.00187033,
	0.001692768,
	0.001530864,
	0.001383399,
	0.001249225,
	0.001127256,
	0.001016487,
	0.000915992,
	0.000824891,
	0.00074238,
	0.000667708,
	0.000600187,
	0.000539179,
	0.000484097,
	0.0004344,
	0.000389595,
	0.000349227,
	0.000312882,
	0.000280179,
	0.000250771,
	0.000224343,
	0.000200608,
	0.000179302,
	0.000160187,
	0.000143048,
	0.000127689,
	0.000113932,
	0.000101616,
	9.05953E-05,
	8.07386E-05,
	7.1927E-05,
	6.40533E-05,
	5.70208E-05,
	5.07423E-05,
	4.51395E-05,
	4.01415E-05,
	3.5685E-05,
	3.17129E-05,
	2.81738E-05,
	2.50218E-05,
	2.22155E-05,
	1.97181E-05,
	1.74961E-05,
	1.552E-05,
	1.37632E-05,
	1.22018E-05,
	1.08145E-05,
	9.58241E-06,
	8.48839E-06,
	7.5173E-06,
	6.6556E-06,
	5.89119E-06,
	5.21328E-06,
	4.61224E-06,
	4.07953E-06,
	3.60749E-06,
	3.18932E-06,
	2.81899E-06,
	2.4911E-06,
	2.20086E-06,
	1.94402E-06,
	1.71678E-06,
	1.51579E-06,
	1.33804E-06,
};

void comparePreciseSamples(string fname="step2prec_HcalSiPMntuple.root", double emin=0, double emax=0){
	TFile* file = TFile::Open(fname.c_str());
	TTree* tree = (TTree*)file->Get("tree");

	int legynum = 4;
	float textSize = 0.04;
	TLegend* leg = new TLegend(0.18,0.9-textSize*legynum,0.4,0.9);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(42);
	leg->SetTextSize(textSize);

	//make pulse template histo (from convolution)
	TH1F* htempT = new TH1F("conv","",250,0,250);
	int bstart = 95;
	for(unsigned b = 0; b < nbin; ++b){
		htempT->SetBinContent(b+bstart,s203[b]);
	}
	htempT->Scale(1.0/htempT->Integral(0,htempT->GetNbinsX()+1));
	htempT->SetMarkerColor(kBlack);
	htempT->SetMarkerSize(0.75);
	htempT->GetXaxis()->SetTitle("time [ns]");
	htempT->GetYaxis()->SetTitle("a.u.");
	leg->AddEntry(htempT,"203 [Conv(SiPM,Y11)]","p");
	
	stringstream scut;
	scut << "(subdet==2)";
	if(emin>=0) scut << "*((signalTot[4]+signalTot[5])*fCtoGeV*photoelectronsToAnalog>" << emin << ")";
	if(emax>0) scut << "*((signalTot[4]+signalTot[5])*fCtoGeV*photoelectronsToAnalog<" << emax << ")";
	//scale Iteration$ to ns
	tree->Draw("Iteration$*0.5>>htempP(250,0,250)",("signalTotPrecise[]*"+scut.str()).c_str(),"goff");
	tree->Draw("Iteration$*25>>htempC(10,0,250)",("signalTot[]*"+scut.str()).c_str(),"goff");
	
	stringstream scutleg;
	if(emin>=0 && emax>0) scutleg << emin << " < E_{M0} < " << emax << " GeV";
	else if(emax>0) scutleg << "E_{M0} < " << emax << " GeV";
	else scutleg << "E_{M0} > " << emin << " GeV";
	leg->AddEntry((TObject*)NULL,scutleg.str().c_str(),"");
	
	TH1F* htempP = (TH1F*)gDirectory->Get("htempP");
	htempP->Scale(1.0/htempP->Integral(0,htempP->GetNbinsX()+1));
	htempP->SetMarkerColor(kRed);
	htempP->SetMarkerSize(0.75);
	leg->AddEntry(htempP,"PreciseSamples","p");
	
	TH1F* htempC = (TH1F*)gDirectory->Get("htempC");
	htempC->Scale(1.0/htempC->Integral(0,htempC->GetNbinsX()+1,"width"));
	htempC->SetMarkerColor(kBlue);
	htempC->SetMarkerSize(0.75);
	leg->AddEntry(htempC,"CaloSamples","p");

	stringstream sout;
	sout << fname.substr(0,fname.length()-5);
	if(emin>=0 && emax>0) sout << "_" << emin << "to" << emax;
	else if(emax>0) sout << "_lt" << emax;
	else sout << "_gt" << emin;
	
	TCanvas* can = new TCanvas(sout.str().c_str(),sout.str().c_str());
	can->SetLogy();
	
	htempT->Draw("hist p");
	htempP->Draw("hist p same");
	htempC->Draw("hist p same");
	leg->Draw("same");
	
	can->Print((sout.str()+".png").c_str(),"png");
	
}