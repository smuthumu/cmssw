#include <TMath.h>
#include <TH1.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TRandom3.h>

#include <cmath>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>

using namespace std;

typedef std::vector<unsigned int> photonTimeHist;

unsigned nbin(250);
static double const root2(sqrt(2));

double Y11TimePDF(double t) {
  return exp(-0.0635-0.1518*t)*pow(t, 2.528)/2485.9;
}

//static const double Y11RANGE(80.);
static const double Y11RANGE(nbin);
static const double Y11MAX(0.04);

double generatePhotonTime(TRandom3* random) {
  double result(0.);
  while (true) {
    result = random->Rndm()*Y11RANGE;
    if (random->Rndm()*Y11MAX < Y11TimePDF(result))
      return result;
  }
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

//const int nBins_(35*2+1);
const int nBins_(nbin*2);
vector<double> shape(nBins_,0.);
bool computedShape = false;
vector<double>& computeShape(){
	if(computedShape) return shape;

	double norm = 0.;
	for (int j = 0; j < nBins_; ++j) {
		shape[j] = analyticPulseShapeHE(j/2.);
		norm += (shape[j]>0) ? shape[j] : 0.;
	}
	
	for (int j = 0; j < nBins_; ++j) {
		shape[j] /= norm;
		//cout << shape[j] << ", ";
	}
	//cout << endl;
	
	computedShape = true;
	return shape;
}

double getShape(double time) {
	vector<double>& shape = computeShape();
	int jtime = static_cast<int>(time*2 + 0.5);
	if (jtime>=0 && jtime<nBins_)
		return shape[jtime];
	return 0.;
}

double* convolve(double (*f1)(double), double (*f2)(double)){
	double* result = new double[2*nbin-1];
	for(unsigned i = 0; i < 2*nbin-1; ++i){
		result[i] = 0;
		for(unsigned j = 0; j < min(i+1,nbin); ++j){
			double tmp = f1(j)*f2(i-j);
			if(std::isnan(tmp) or std::isinf(tmp)) continue;
			result[i] += tmp;
		}
	}
	return result;
}

void convolvePulsesPE(unsigned nevents=1e6,unsigned npe=200){
	//instantiate generator
	TRandom3* random = new TRandom3();
	
	double* result = convolve(analyticPulseShapeHE,Y11TimePDF);
	//double* result = convolve(getShape,Y11TimePDF);
	
	const double dt(0.5);
	const double invdt(1./dt);
	//double tzero(6.0-25*(5-1));
	//double tzero_bin(-tzero/dt);
	double tzero_bin(0);
	
	TH1F* h2 = new TH1F("h2","",nbin,-0.5,double(nbin)-0.5);
	for(unsigned i = 0; i < nbin; ++i){
		h2->SetBinContent(i+1,Y11TimePDF((double)i));
	}
	
	TH1F* hresult = new TH1F("hresult","",250,-0.5,250-0.5);
	for(unsigned i = 0; i < nevents; ++i){
		if(i % 10000 == 0) cout << "Generating " << i << "/" << nevents << endl;
		
		double t_pe(0.);
		int t_bin(0);
		double elapsedTime(0.);
		photonTimeHist precisionTimedPhotons(25*10/dt,0);
		list<pair<double, double>> pulses;
		vector<double> signal(25*10/dt,0);

		for(unsigned ipe = 0; ipe < npe; ++ipe){
			t_pe = generatePhotonTime(random);
			//t_pe = h2->GetRandom();
			t_bin = int(t_pe/dt+tzero_bin+0.5);
			if(t_bin>=0 && static_cast<unsigned int>(t_bin) < precisionTimedPhotons.size()){
				precisionTimedPhotons[t_bin] += 1;
			}
		}
		for(unsigned tbin = 0; tbin < precisionTimedPhotons.size(); ++tbin){
			double hitPixels = precisionTimedPhotons[tbin];
			pulses.push_back(pair<double, double>(elapsedTime, hitPixels));
			
			auto pulse = pulses.begin();
			while (pulse != pulses.end()) {
				double timeDiff = elapsedTime - pulse->first;
				double pulseBit = getShape(timeDiff)*pulse->second;
				signal[tbin] += pulseBit*invdt;
			
				//if (timeDiff > 1 && getShape(timeDiff) < 1e-6)
				if (timeDiff > 1 && getShape(timeDiff) < 1e-7)
					pulse = pulses.erase(pulse);
				else
					++pulse;
			}
			elapsedTime += dt;
		}
		for(unsigned isig = 0; isig < signal.size(); ++isig){
			hresult->Fill(isig*0.5,signal[isig]);
		}
	}

	//normalize
	hresult->Scale(1./hresult->Integral(0,hresult->GetNbinsX()+1));
	hresult->SetLineColor(kBlack);
	hresult->SetLineWidth(2);
	
	//for plotting
	nbin = 250;
	double *x = new double[nbin];
	double *yconv = new double[nbin];
	double sconv = 0;
	for(unsigned i = 0; i < nbin; ++i){
		x[i] = i;
		yconv[i] = result[i]; sconv += yconv[i];
	}
	
	//normalize
	double max = 0;
	for(unsigned i = 0; i < nbin; ++i){
		yconv[i] /= sconv; if(yconv[i] > max) max = yconv[i];
	}
	
	//graphs
	TGraph *gconv;
	//legend
	TLegend* leg = new TLegend(0.55,0.82,0.9,0.92);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.05);
	leg->SetTextFont(42);
	leg->AddEntry(hresult,"toy MC convolution","p");
	
	gconv = new TGraph(nbin,x,yconv);
	gconv->SetMarkerColor(kBlack);
	gconv->SetMarkerStyle(20);
	leg->AddEntry(gconv,"numerical convolution","p");
	
	//draw
	stringstream cname;
	cname << "convolvePulsesPE_pe" << npe << "_n" << nevents;
	TCanvas* can = new TCanvas(cname.str().c_str(),cname.str().c_str());
	can->cd();
	hresult->SetTitle("");
	max = 0.05; //manual
	hresult->GetYaxis()->SetRangeUser(1e-7,max);
	hresult->GetYaxis()->SetTitle("arbitrary units");
	hresult->GetXaxis()->SetTitle("t [ns]");
	hresult->SetMarkerStyle(20);
	hresult->SetMarkerColor(kBlue);
	hresult->Draw("hist p");
	gconv->Draw("p same");
	leg->Draw("same");
	
	can->Print((cname.str()+".png").c_str(),"png");
	
	can->SetLogy();
	can->Print((cname.str()+"_logy.png").c_str(),"png");
	
	//just print central region
	for(int i = 1; i <= nbin; ++i){
		//cout << hresult->GetBinContent(i) << ", ";
		cout << result[i] << ", ";
	}
	cout << endl;

}
