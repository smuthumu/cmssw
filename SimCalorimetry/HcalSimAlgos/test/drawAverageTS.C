#include <string>

void drawAverageTS(string frame="frame1", bool showData=false, string suff=""){
	string fname = "step2"+suff+"_DebugCaloSamples.root";
	TFile* file = TFile::Open(fname.c_str());
	TTree* tree = (TTree*)file->Get("tree");
	
	TCanvas* can = new TCanvas();
	can->SetLogy();
	
	int legynum = 2;
	if(showData) legynum = 4;
	float textSize = 0.04;
	TLegend* leg = new TLegend(0.18,0.9-textSize*legynum,0.4,0.9);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(42);
	leg->SetTextSize(textSize);
	
	//pedestal: 17.3 fC * 0.31 = 5.4
	if(frame=="frame4") tree->Draw("Iteration$>>h2(10,-0.5,9.5)",("("+frame+"[]-17.3)*(subdet==2)").c_str(),"goff");
	else if(frame=="frame5") tree->Draw("Iteration$>>h2(10,-0.5,9.5)",("("+frame+"[]-5)*(subdet==2)").c_str(),"goff");
	else tree->Draw("Iteration$>>h2(10,-0.5,9.5)",(frame+"[]*(subdet==2)").c_str(),"goff");
	tree->Draw("Iteration$>>h2(10,-0.5,9.5)",(frame+"[]*(subdet==2)").c_str(),"goff");
	TH1* h2 = (TH1*)gDirectory->Get("h2");
	h2->SetLineColor(kRed);
	h2->SetLineWidth(2);
	h2->SetTitle("");
	h2->GetXaxis()->SetTitle("TS");
	h2->GetYaxis()->SetTitle("a.u.");
	h2->Scale(1.0/h2->Integral(0,10));
	h2->Draw("hist");
	leg->AddEntry(h2,"HE SiPM MC (pre12)","l");
	//pedestal: 3.0 fC * 0.912 - 0.49 = 2.246
	if(frame=="frame4") tree->Draw("Iteration$>>h1(10,-0.5,9.5)",("("+frame+"[]-3.0)*(subdet==1)").c_str(),"goff");
	else if(frame=="frame5") tree->Draw("Iteration$>>h1(10,-0.5,9.5)",("("+frame+"[]-2)*(subdet==1)").c_str(),"goff");
	else tree->Draw("Iteration$>>h1(10,-0.5,9.5)",(frame+"[]*(subdet==1)").c_str(),"goff");
	TH1* h1 = (TH1*)gDirectory->Get("h1");
	h1->SetLineColor(kBlue);
	h1->SetLineWidth(2);
	h1->Scale(1.0/h1->Integral(0,10));
	h1->Draw("hist same");
	leg->AddEntry(h1,"HB HPD MC (pre12)","l");
	
	if(showData){ //look at pe for data, after noise etc. (using fC2pe conversion)
		TFile* fileTB = TFile::Open("/uscms_data/d3/pedrok/raddam/muons/CMSSW_7_5_0/src/test/pion/TB_S_tuple.root");
		TTree* treeTB = (TTree*)fileTB->Get("tree");
		//pedestal: 17.3 fC * 0.31 = 5.4
		if(frame=="frame5") treeTB->Draw("Iteration$>>htb(10,-0.5,9.5)","adc[]-5","goff"); //adc
		else if(frame=="frame1") treeTB->Draw("Iteration$>>htb(10,-0.5,9.5)","(fC[]-17.3)/57.5","goff"); //pe
		else treeTB->Draw("Iteration$>>htb(10,-0.5,9.5)","fC[]-17.3","goff"); //fc
		TH1* htb = (TH1*)gDirectory->Get("htb");
		htb->SetLineColor(kBlack);
		htb->SetLineWidth(2);
		htb->Scale(1.0/htb->Integral(0,10));
		htb->Draw("hist same");
		leg->AddEntry(htb,"TB SiPM (Aug 2015 pion)","l");
		//cut out noise in HO
		TFile* fileHO = TFile::Open("/uscms_data/d3/pedrok/hf/sipm/revert/CMSSW_8_1_0_pre11/src/test/SiPMValidation/test/HOdigis_HODataFrames_.root");
		TTree* treeHO = (TTree*)fileHO->Get("tree");
		//pedestal: 11 fC * 0.907 - 0.44 = 9.5
		if(frame=="frame5") treeHO->Draw("Iteration$>>hho(10,-0.5,9.5)","(adc[]-10)*(adc[4]>70)","goff"); //adc
		else if(frame=="frame1") treeHO->Draw("Iteration$>>hho(10,-0.5,9.5)","(fC[]-11.0)*(adc[4]>70)/4.0","goff"); //pe
		else treeHO->Draw("Iteration$>>hho(10,-0.5,9.5)","(fC[]-11.0)*(adc[4]>70)","goff"); //fc
		TH1* hho = (TH1*)gDirectory->Get("hho");
		hho->SetLineColor(kMagenta);
		hho->SetLineWidth(2);
		hho->Scale(1.0/hho->Integral(0,10));
		hho->Draw("hist same");
		leg->AddEntry(hho,"HO SiPM (JetHT run 278820)","l");
	}
	
	leg->Draw("same");
}