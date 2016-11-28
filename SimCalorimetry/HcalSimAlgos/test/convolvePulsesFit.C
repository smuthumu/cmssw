#include <TFile.h>
#include <TMath.h>
#include <TH1.h>
#include <TF1.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TRandom3.h>
#include <TSpectrum.h>
#include <TVirtualFFT.h>
#include "Math/PdfFuncMathCore.h"

#include <cmath>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>

using namespace std;

static double const root2(sqrt(2));

class PulseFunctor {
	public:
		PulseFunctor(vector<double> vec_, unsigned nbin_) : vec(vec_), nbin(nbin_), graph(NULL) {
			//ensure normalization
			double sum = 0;
			double sumc = 0;
			for(unsigned v = 0; v < vec.size(); ++v){
				sum += vec[v];
				if(v<nbin) sumc += vec[v];
			}
			vector<double> x(nbin,0);
			vector<double> y(nbin,0);
			for(unsigned v = 0; v < nbin; ++v){
				if(v<nbin){
					x[v] = v;
					y[v] = vec[v]/sumc;
				}
				vec[v] /= sum;
			}
			//make graph
			graph = new TGraph(nbin,x.data(),y.data());
			graph->SetTitle("");
			graph->SetMarkerStyle(20);
		}
		double operator()(double t){
			double result = 0;
			int it = int(t+0.5);
			if(it>=0 and it<vec.size()){
				result = vec[it];
			}
			return result;
		}
		
		//member variables
		vector<double> vec;
		unsigned nbin;
		TGraph* graph;
};

class ConvolveFunctor {
	public:
		ConvolveFunctor(vector<double> base_, int option_) : base(base_), nbin(base.size()), option(option_) {}
		double operator()(double *x, double *p){
			int i(x[0]+0.5);
			double result = 0;
			for(unsigned j = 0; j < min((unsigned)(i+1),nbin); ++j){
				if(option==1) result += base[j]*(ROOT::Math::lognormal_pdf(i-j,p[0],p[1]));
				else if(option==2) result += base[j]*(ROOT::Math::landau_pdf(i-j,p[1],p[0]));
				else if(option==3) result += base[j]*((p[0]*ROOT::Math::lognormal_pdf(i-j,p[1],p[4])+p[2]*ROOT::Math::lognormal_pdf(i-j,p[3],p[4]))/(p[0]+p[2]));
				else if(option==4) result += base[j]*((p[0]*ROOT::Math::lognormal_pdf(i-j,p[1],p[2])+p[3]*ROOT::Math::lognormal_pdf(i-j,p[4],p[5]))/(p[0]+p[3]));
			}
			return result;
		}
		
		//member variables
		vector<double> base;
		unsigned nbin;
		int option;
};

double Y11TimePDF(double t) {
  return exp(-0.0635-0.1518*t)*pow(t, 2.528)/2485.9;
}

double onePulse(double t, double A, double sigma, double theta, double m) {
  return (t<theta) ? 0 : A*TMath::LogNormal(t,sigma,theta,m);
}

double analyticPulseShapeHE(double t) {
  // taken from fit to laser measurement taken by Iouri M. in Spring 2016.
  double A1(5.204/6.94419), sigma1_shape(0.5387), theta1_loc(-0.3976), m1_scale(4.428);
  double A2(1.855/6.94419), sigma2_shape(0.8132), theta2_loc(7.025),   m2_scale(12.29);
  return
    onePulse(t,A1,sigma1_shape,theta1_loc,m1_scale) +
    onePulse(t,A2,sigma2_shape,theta2_loc,m2_scale);
}

template <class F1, class F2>
vector<double> convolve(unsigned nbin, F1 f1, F2 f2){
//vector<double> convolve(unsigned nbin, double (*f1)(double), double (*f2)(double)){
	vector<double> result(2*nbin-1,0.);
	for(unsigned i = 0; i < 2*nbin-1; ++i){
		for(unsigned j = 0; j < min(i+1,nbin); ++j){
			double tmp = f1(j)*f2(i-j);
			if(std::isnan(tmp) or std::isinf(tmp)) continue;
			result[i] += tmp;
		}
	}
	return result;
}

void convolvePulsesFit(int option=1){
	//instantiate generator
	TRandom3* random = new TRandom3();
	
	unsigned nbin = 250;
	vector<double> result1 = convolve(nbin,analyticPulseShapeHE,Y11TimePDF);
	PulseFunctor pulse1(result1,nbin);
	pulse1.graph->SetMarkerColor(kBlack);
	pulse1.graph->GetXaxis()->SetTitle("t [ns]");
	pulse1.graph->GetYaxis()->SetTitle("arbitrary units");

	//TFile* fsim = TFile::Open("histos_step2prec1Kbin_HcalSiPMntuple.root");
	TFile* fsim = TFile::Open("histos_step2prec1K250bin_HcalSiPMntuple.root");
	TH1F* hsim = (TH1F*)fsim->Get("signalTotPrecise_0to100");
	hsim->Scale(1.0/hsim->Integral(0,hsim->GetNbinsX()+1));
	hsim->SetMarkerStyle(24);
	hsim->SetMarkerColor(kMagenta);
	//hsim->GetXaxis()->SetLimits(-94.5,155.5);
	
	//draw
	stringstream cname;
	cname << "convolvePulsesFit_option" << option;
	TCanvas* can = new TCanvas(cname.str().c_str(),cname.str().c_str());
	can->cd();
	can->SetLogy();
	can->SetGridx();
	can->SetGridy();
	//TH1F* haxis = new TH1F("axis","",100,0,155);
	TH1F* haxis = new TH1F("axis","",100,0,250);
	haxis->GetXaxis()->SetTitle("t [ns]");
	haxis->GetYaxis()->SetTitle("arbitrary units");
	haxis->GetYaxis()->SetRangeUser(hsim->GetMinimum(0)*0.9,hsim->GetMaximum()*1.1);
	haxis->Draw();

	TF1* fconv = NULL;
	int npars = 0;
	string name = "";	
	ConvolveFunctor conv1(pulse1.vec,option);
	if(option==1){
		npars = 2;
		name = "Lognormal fit";
		fconv = new TF1("fconv",conv1,0,nbin,npars);
		fconv->SetParameters(0.9368,1.5);
	}
	else if(option==2){
		npars = 2;
		name = "Landau fit";
		fconv = new TF1("fconv",conv1,0,nbin,npars);
		fconv->SetParameters(7.402,0.4637);
	}
	else if(option==3){
		npars = 5;
		name = "2*Lognormal fit";
		fconv = new TF1("fconv",conv1,0,nbin,npars);
		fconv->SetParameters(4087,0.9,3344,4.3,1.2);
	}
	else if(option==4){
		npars = 6;
		name = "2*Lognormal fit";
		fconv = new TF1("fconv",conv1,0,nbin,npars);
		fconv->SetParameters(4087,0.9,1.2,3344,4.3,1.2);
	}
	else return;
	fconv->SetLineColor(kBlue);
	fconv->SetLineWidth(2);
	hsim->Fit(fconv,"N");

	//legend
	int legnum = 2;
	legnum += npars+1;
	//double legymax = 0.7;
	double legymax = 0.6;
	TLegend* leg = new TLegend(0.25,legymax-legnum*0.05,0.75,legymax);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.05);
	leg->SetTextFont(42);
	leg->AddEntry(hsim,"0to100","p");
	
	leg->AddEntry(fconv,name.c_str(),"l");
	for(unsigned p = 0; p < npars; ++p){
		stringstream ss;
		ss << setprecision(3);
		ss << "p" << p << " = " << fconv->GetParameter(p) << " #pm " << fconv->GetParError(p);
		leg->AddEntry((TObject*)NULL,ss.str().c_str(),"");
	}
	stringstream sc;
	sc << "#chi^{2} / ndf = " << fconv->GetChisquare() << " / " << fconv->GetNDF();
	leg->AddEntry((TObject*)NULL,sc.str().c_str(),"");
	
	hsim->Draw("hist p same");
	fconv->Draw("same");
	leg->Draw("same");
	
	can->Print((cname.str()+"_logy.png").c_str(),"png");
	
	for(unsigned i = 0; i < nbin; ++i){
		cout << fconv->Eval(i) << ", ";
	}
	cout << endl;

}
