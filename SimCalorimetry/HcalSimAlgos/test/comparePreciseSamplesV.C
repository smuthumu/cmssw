#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TDirectory.h>

#include <string>
#include <sstream>
#include <vector>

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

static const int nbinHPD(212);
double s105[nbinHPD] = {
	0,
	0.00033394,
	0.00136382,
	0.00345003,
	0.0069245,
	0.0114013,
	0.0164575,
	0.0216718,
	0.0266611,
	0.0311094,
	0.0347871,
	0.0375579,
	0.0393753,
	0.0402696,
	0.0403301,
	0.0396845,
	0.0384793,
	0.0368628,
	0.0349727,
	0.032928,
	0.0308253,
	0.0287377,
	0.0267174,
	0.024798,
	0.022999,
	0.0213288,
	0.0197884,
	0.0183738,
	0.0170779,
	0.0158922,
	0.0148077,
	0.0138152,
	0.0129064,
	0.012073,
	0.011308,
	0.0106047,
	0.00995714,
	0.00936001,
	0.00880855,
	0.00829848,
	0.007826,
	0.00738767,
	0.00698044,
	0.00660155,
	0.00624853,
	0.00591916,
	0.00561146,
	0.00532362,
	0.00505403,
	0.00480122,
	0.00456387,
	0.00434079,
	0.0041309,
	0.0039332,
	0.00374682,
	0.00357093,
	0.00340479,
	0.00324773,
	0.00309914,
	0.00295845,
	0.00282513,
	0.00269872,
	0.00257878,
	0.0024649,
	0.00235672,
	0.00225389,
	0.0021561,
	0.00206304,
	0.00197446,
	0.00189009,
	0.00180971,
	0.00173309,
	0.00166003,
	0.00159034,
	0.00152384,
	0.00146036,
	0.00139975,
	0.00134186,
	0.00128656,
	0.00123371,
	0.0011832,
	0.0011349,
	0.00108872,
	0.00104454,
	0.00100228,
	0.000961835,
	0.00092313,
	0.000886081,
	0.000850609,
	0.000816643,
	0.000784113,
	0.000752953,
	0.000723102,
	0.0006945,
	0.000667091,
	0.000640823,
	0.000615643,
	0.000591505,
	0.000568362,
	0.00054617,
	0.000524888,
	0.000504476,
	0.000484897,
	0.000466114,
	0.000448094,
	0.000430803,
	0.000414211,
	0.000398286,
	0.000383002,
	0.000368332,
	0.000354247,
	0.000340726,
	0.000327743,
	0.000315276,
	0.000303304,
	0.000291806,
	0.000280762,
	0.000270153,
	0.000259962,
	0.000250171,
	0.000240764,
	0.000231725,
	0.000223038,
	0.000214691,
	0.000206667,
	0.000198956,
	0.000191543,
	0.000184416,
	0.000177565,
	0.000170978,
	0.000164644,
	0.000158554,
	0.000152697,
	0.000147064,
	0.000141646,
	0.000136435,
	0.000131423,
	0.000126601,
	0.000121962,
	0.000117498,
	0.000113204,
	0.000109071,
	0.000105095,
	0.000101268,
	9.76E-05,
	9.40E-05,
	9.06E-05,
	8.73E-05,
	8.42E-05,
	8.11E-05,
	7.82E-05,
	7.54E-05,
	7.27E-05,
	7.01E-05,
	6.75E-05,
	6.51E-05,
	6.28E-05,
	6.05E-05,
	5.84E-05,
	5.63E-05,
	5.43E-05,
	5.23E-05,
	5.05E-05,
	4.87E-05,
	4.69E-05,
	4.53E-05,
	4.36E-05,
	4.21E-05,
	4.06E-05,
	3.92E-05,
	3.78E-05,
	3.64E-05,
	3.52E-05,
	3.39E-05,
	3.27E-05,
	3.15E-05,
	3.00E-05,
	2.83E-05,
	2.62E-05,
	2.37E-05,
	2.10E-05,
	1.82E-05,
	1.55E-05,
	1.28E-05,
	1.04E-05,
	8.26E-06,
	6.40E-06,
	4.84E-06,
	3.58E-06,
	2.59E-06,
	1.83E-06,
	1.26E-06,
	8.47E-07,
	5.57E-07,
	3.57E-07,
	2.24E-07,
	1.37E-07,
	8.17E-08,
	4.77E-08,
	2.71E-08,
	1.51E-08,
	8.17E-09,
	4.32E-09,
	2.23E-09,
	1.12E-09,
	5.50E-10,
	2.62E-10,
	1.20E-10,
	5.28E-11,
	2.12E-11,
	7.35E-12,
	1.76E-12
};

void comparePreciseSamplesV(vector<string> fnames={"step2prec_HcalSiPMntuple.root"}, double emin=0, double emax=0, int depth=0, bool hpd=false){
	int legynum = 3+fnames.size();
	float textSize = 0.04;
	float legx = hpd ? 0.6 : 0.18;
	TLegend* leg = new TLegend(legx,0.92-textSize*legynum,legx+0.22,0.92);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(42);
	leg->SetTextSize(textSize);

	//make pulse template histo (from convolution)
	TH1F* htempT = new TH1F("conv","",250,0,250);
	int bstart = hpd ? 72 : 95;
	int bend = hpd ? nbinHPD : nbin;
	for(unsigned b = 0; b < bend; ++b){
		if(hpd) htempT->SetBinContent(b+bstart,s105[b]);
		else htempT->SetBinContent(b+bstart,s203[b]);
	}
	htempT->Scale(1.0/htempT->Integral(0,htempT->GetNbinsX()+1));
	htempT->SetMarkerColor(kBlack);
	htempT->SetMarkerSize(0.75);
	htempT->GetXaxis()->SetTitle("time [ns]");
	htempT->GetYaxis()->SetTitle("a.u.");
	if(hpd) leg->AddEntry(htempT,"105 [HPD RECO]","p");
	else leg->AddEntry(htempT,"203 [Conv(SiPM,Y11)]","p");
	
	stringstream scutleg;
	if(emin>=0 && emax>0) scutleg << emin << " < E_{M0} < " << emax << " GeV";
	else if(emax>0) scutleg << "E_{M0} < " << emax << " GeV";
	else scutleg << "E_{M0} > " << emin << " GeV";
	leg->AddEntry((TObject*)NULL,scutleg.str().c_str(),"");
	stringstream scutleg2;
	if(depth<=0) scutleg2 << "all depths";
	else scutleg2 << "depth " << depth;
	leg->AddEntry((TObject*)NULL,scutleg2.str().c_str(),"");

	string sgain = hpd ? "0.23*0.3305" : "fCtoGeV*photoelectronsToAnalog";
	
	stringstream scut;
	scut << "(subdet==2)";
	if(emin>=0) scut << "*((signalTot[4]+signalTot[5])*" << sgain << ">" << emin << ")";
	if(emax>0) scut << "*((signalTot[4]+signalTot[5])*" << sgain << "<" << emax << ")";
	if(depth>0) scut << "*(depth==" << depth << ")";
	
	stringstream sout;
	
	Color_t colors[] = {kBlue,kMagenta,kRed,kOrange+1};
	Color_t markers[] = {20,24,21,25};
	vector<TH1F*> htemp(fnames.size(),NULL);
	for(unsigned i = 0; i < fnames.size(); ++i){
		TFile* file = TFile::Open(fnames[i].c_str());
		TTree* tree = (TTree*)file->Get("tree");

		//scale Iteration$ to ns
		stringstream hname;
		hname << "htemp" << i;
		string sdraw = "Iteration$*0.5>>"+hname.str()+"(250,0,250)";
		tree->Draw(sdraw.c_str(),("signalTotPrecise[]*"+scut.str()).c_str(),"goff");
		
		string shortname = fnames[i].substr(0,fnames[i].find("_"));
		
		htemp[i] = (TH1F*)gDirectory->Get(hname.str().c_str());
		htemp[i]->SetDirectory(NULL);
		htemp[i]->Scale(1.0/htemp[i]->Integral(0,htemp[i]->GetNbinsX()+1));
		htemp[i]->SetMarkerColor(colors[i]);
		htemp[i]->SetMarkerStyle(markers[i]);
		htemp[i]->SetMarkerSize(0.75);
		leg->AddEntry(htemp[i],shortname.c_str(),"p");
		
		sout << shortname << "_";
		file->Close();
	}

	if(emin>=0 && emax>0) sout << emin << "to" << emax;
	else if(emax>0) sout << "lt" << emax;
	else sout << "gt" << emin;
	if(depth>0) sout << "_depth" << depth;
	
	TCanvas* can = new TCanvas(sout.str().c_str(),sout.str().c_str());
	can->SetLogy();
	
	htempT->Draw("hist p");
	for(unsigned i = 0; i < fnames.size(); ++i){
		htemp[i]->Draw("hist p same");
	}
	leg->Draw("same");
	
	can->Print((sout.str()+".png").c_str(),"png");
	
}