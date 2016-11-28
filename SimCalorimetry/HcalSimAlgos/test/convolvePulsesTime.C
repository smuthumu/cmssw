#include <TFile.h>
#include <TMath.h>
#include <TH1.h>
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

double Geant4TimePDF(double t) {
	//return ROOT::Math::lognormal_pdf(t,-71.71,21.76);
	//return (4087*ROOT::Math::lognormal_pdf(t,0.9368,1.198)+3344*ROOT::Math::lognormal_pdf(t,4.288,1.271))/(4087+3344);
	//return ROOT::Math::lognormal_pdf(t,0.9368,1.198);
	//return ROOT::Math::lognormal_pdf(t,0.9368,1.5);
	return ROOT::Math::landau_pdf((t+7.402)/0.4637)/0.4637;
	//return (9298*ROOT::Math::lognormal_pdf(t,0.724,1.48)+29360*ROOT::Math::lognormal_pdf(t,4.742,1.025))/(9298+29360);
}

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

void convolvePulsesTime(bool do_fft=false){
	//instantiate generator
	TRandom3* random = new TRandom3();
	
	unsigned nbin = 250;
	vector<double> result1 = convolve(nbin,analyticPulseShapeHE,Y11TimePDF);
	PulseFunctor pulse1(result1,nbin);
	pulse1.graph->SetMarkerColor(kBlack);
	pulse1.graph->GetXaxis()->SetTitle("t [ns]");
	pulse1.graph->GetYaxis()->SetTitle("arbitrary units");

	vector<double> result2 = convolve(nbin,pulse1,Geant4TimePDF);
	PulseFunctor pulse2(result2,nbin);
	pulse2.graph->SetMarkerColor(kBlue);
	
	//legend
	TLegend* leg = new TLegend(0.55,0.82,0.9,0.92);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.05);
	leg->SetTextFont(42);
	leg->AddEntry(pulse1.graph,"Conv(Y11,SiPM)","p");
	leg->AddEntry(pulse2.graph,"Conv(Y11,SiPM,Geant4)","p");
	
	//draw
	stringstream cname;
	cname << "convolvePulsesTime";
	TCanvas* can = new TCanvas(cname.str().c_str(),cname.str().c_str());
	can->cd();
	can->SetLogy();
	can->SetGridx();
	can->SetGridy();
	
	double ymin = 1e-10;
	for(unsigned b = 0; b < nbin; ++b){
		double y1 = pulse1.graph->GetY()[b];
		if(y1>0&&y1<ymin) ymin = y1;
		double y2 = pulse2.graph->GetY()[b];
		if(y2>0&&y2<ymin) ymin = y2;
	}
	
	TFile* fsim = TFile::Open("histos_step2prec1Kbin_HcalSiPMntuple.root");
	TH1F* hsim = (TH1F*)fsim->Get("signalTotPrecise_0to100");
	hsim->Scale(1.0/hsim->Integral(0,hsim->GetNbinsX()+1));
	hsim->SetMarkerStyle(24);
	hsim->SetMarkerColor(kMagenta);
	hsim->GetXaxis()->SetLimits(-95,155);
	
	double ymax = max(*max_element(pulse1.graph->GetY(),pulse1.graph->GetY()+nbin),*max_element(pulse2.graph->GetY(),pulse2.graph->GetY()+nbin));
	pulse1.graph->GetYaxis()->SetRangeUser(ymin,ymax);
	pulse1.graph->Draw("ap");
	pulse2.graph->Draw("p same");
	hsim->Draw("hist p same");
	leg->Draw("same");
	
	can->Print((cname.str()+"_logy.png").c_str(),"png");
	
	//just print central region
	for(int i = 1; i <= nbin; ++i){
		cout << pulse2.vec[i] << ", ";
	}
	cout << endl;
	
	if(!do_fft) return;
	
	int nbinconv = 156*2;
	
	//make mirror-symmetric distributions for FFT
	double *source = new double[nbinconv];
	for(int b = 95; b <= 250; ++b){
		source[b-95] = hsim->GetBinContent(b);
		source[nbinconv-1-(b-95)] = source[b-95];
	}
	double *response = new double[nbinconv];
	for(unsigned r = 0; r < nbinconv/2; ++r){
		response[r] = pulse1.vec[r];
		response[nbinconv-1-r] = response[r];
	}
	
	TVirtualFFT *fft_own = TVirtualFFT::FFT(1, &nbinconv, "R2C M K");

	//fft for sim distribution
	fft_own->SetPoints(source);
	fft_own->Transform();
	double *re_source = new double[nbinconv];
	double *im_source = new double[nbinconv];
	fft_own->GetPointsComplex(re_source,im_source);

	//fft for ideal pulse
	fft_own->SetPoints(response);
	fft_own->Transform();
	double *re_response = new double[nbinconv];
	double *im_response = new double[nbinconv];
	fft_own->GetPointsComplex(re_response,im_response);
	
	//deconvolve: divide source/response
	int nbindeconv = nbinconv/2;
	double *re_divide = new double[nbindeconv];
	double *im_divide = new double[nbindeconv];
	for(int i = 0; i < nbindeconv; ++i){
		double denom = re_source[i]*re_source[i]+re_response[i]*re_response[i];
		re_divide[i] = (re_source[i]*re_response[i]+im_source[i]*im_response[i])/denom;
		if(std::isnan(re_divide[i]) or std::isinf(re_divide[i])) re_divide[i] = 0;
		im_divide[i] = (im_source[i]*re_response[i]-re_source[i]*im_response[i])/denom;
		if(std::isnan(im_divide[i]) or std::isinf(im_divide[i])) im_divide[i] = 0;
		//cout << i << ": (" << re_source[i] << "," << im_source[i] << ") / (" << re_response[i] << "," << im_response[i] << ") = (" << re_divide[i] << "," << im_divide[i] << ")" << endl;
	}
	
	//inverse fft
	Double_t *ifft_result = new Double_t[nbindeconv];
	TVirtualFFT *ifft_own = TVirtualFFT::FFT(1, &nbindeconv, "C2R M K");
	ifft_own->SetPointsComplex(re_divide,im_divide);
	ifft_own->Transform();
	ifft_own->GetPoints(ifft_result);
	
	//TSpectrum* spec = new TSpectrum();
	//spec->Deconvolution(source,response,nbinconv,1000,1,1);

	vector<double> vsource(ifft_result,ifft_result+nbindeconv);
	PulseFunctor pulsedeconv(vsource,nbindeconv);

	//draw
	stringstream cname2;
	cname2 << "deconvolvePulsesTime";
	TCanvas* can2 = new TCanvas(cname2.str().c_str(),cname2.str().c_str());
	can2->cd();
	//can2->SetLogy();
	can2->SetGridx();
	can2->SetGridy();
	
	pulsedeconv.graph->Draw("ap");
	//can2->Print((cname2.str()+"_logy.png").c_str(),"png");
	can2->Print(cname2.str().c_str(),"png");
	
	for(int i = 0; i < nbindeconv; ++i){
		cout << vsource[i] << ", ";
	}
	cout << endl;
	
}
