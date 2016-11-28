{
	TFile *_file0 = TFile::Open("step2prec10K_HcalSiPMntuple.root");
	TTree* tree = (TTree*)_file0->Get("tree");
	
	TCanvas* c1 = new TCanvas("jitter_energy_weighted_vs_energy","jitter_energy_weighted_vs_energy");
	c1->SetRightMargin(0.15);
	c1->SetLogz();
	tree->Draw("Sum$((time-tof)*energy)/Sum$(energy):Sum$(energy)>>htemp(50,0,3,50,0,500)","subdet==2","colz");
	TH1* htemp = (TH1*)gDirectory->Get("htemp");
	htemp->SetTitle("");
	htemp->GetXaxis()->SetTitle("#Sigma#(){E_{sim}}");
	htemp->GetYaxis()->SetTitle("#Sigma#[]{#(){t_{sim}-t_{flight}}#upointE_{sim}}/#Sigma#(){E_{sim}}");
	c1->Update();
	
	TCanvas* c2 = new TCanvas("jitter_energy_weighted_vs_log_energy","jitter_energy_weighted_vs_log_energy");
	c2->SetRightMargin(0.15);
	c2->SetLogz();
	tree->Draw("Sum$((time-tof)*energy)/Sum$(energy):log(Sum$(energy))>>htemp2(17,-16,1,50,0,500)","subdet==2","colz");
	TH1* htemp2 = (TH1*)gDirectory->Get("htemp2");
	htemp2->SetTitle("");
	htemp2->GetXaxis()->SetTitle("log#[]{#Sigma#(){E_{sim}}}");
	htemp2->GetYaxis()->SetTitle("#Sigma#[]{#(){t_{sim}-t_{flight}}#upointE_{sim}}/#Sigma#(){E_{sim}}");
	c2->Update();
	
	TCanvas* c3 = new TCanvas("jitter_energy_weighted_slices_log_energy","jitter_energy_weighted_slices_log_energy");
	c3->SetLogy();
	//gStyle->SetPalette(kVisibleSpectrum);
	int ncolors = gStyle->GetNumberOfColors();
	int ndiv = ncolors/17;
	
	THStack* hs = new THStack(htemp2,"y");
	TList * hlist = hs->GetHists();
	TIter inext(hlist);
	TObject* object = 0;
	int counter = 0;
	
	while ((object = inext())) {
		TH1* tmp = (TH1*)object;
		tmp->Scale(1.0/tmp->Integral(0,tmp->GetNbinsX()));
		tmp->SetLineColor(gStyle->GetColorPalette(counter));
		counter += ndiv;
	}
	
	hs->Draw("nostack");
	hs->SetTitle("");
	hs->GetHistogram()->GetXaxis()->SetTitle("#Sigma#[]{#(){t_{sim}-t_{flight}}#upointE_{sim}}/#Sigma#(){E_{sim}}");
	hs->GetHistogram()->GetYaxis()->SetTitle("a.u.");
	hs->GetHistogram()->GetYaxis()->SetRangeUser(1e-5,2);
	hs->GetHistogram()->Draw("");
	hs->Draw("nostacksame");
}