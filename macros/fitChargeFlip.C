#include "TH2D.h"
#include "TF2.h"
#include "TFile.h"
#include "TCanvas.h"
#include <iostream>

Double_t func(Double_t *x, Double_t *par) {
    Double_t etal1 = x[0];
    Double_t etal2 = x[1];
    return (par[int(2*etal1)] + par[int(2*etal2)])/(1+par[int(2*etal1)] * par[int(2*etal2)]);
}

void fitChargeFlip() 
{
    TH1::SetDefaultSumw2(); 
    TFile *fNum = new TFile("../Results/Apr2017/DY/e_SS/test.root");
    TFile *fDen = new TFile("../Results/Apr2017/DY/e_OS/test.root");
    
    TH2D *num = (TH2D*)fNum->Get("data_leptonabseta1_leptonabseta2_ee_SS");
    TH2D *den = (TH2D*)fDen->Get("data_leptonabseta1_leptonabseta2_ee_OS");
    TH2D *numMC = (TH2D*)fNum->Get("DYJets_SS_leptonabseta1_leptonabseta2_ee_SS");
    TH2D *denMC = (TH2D*)fDen->Get("DYJets_OS_leptonabseta1_leptonabseta2_ee_OS");
    num->Divide(den);
    numMC->Divide(denMC);

    TCanvas *c = new TCanvas("c","c");
    num->Draw("text");
    TF2 *fit = new TF2("fit",func,0,2.5,0,2.5,5);
    fit->SetParameters(0.005,0.01,0.05,0.1,0.5);
    TF2 *fitMC = new TF2("fitMC",func,0,2.5,0,2.5,5);
    fitMC->SetParameters(0.005,0.01,0.05,0.1,0.5);
    
    num->Fit("fit","0");
    Double_t *par = fit->GetParameters();
    Double_t *err = fit->GetParErrors();
    
    numMC->Fit("fitMC","0");
    Double_t *parMC = fitMC->GetParameters();
    Double_t *errMC = fitMC->GetParErrors();
    for( size_t i = 0; i<5; ++i )
    {
//         cout << par[i] << " " << parMC[i] << " " << par[i]/parMC[i] << endl;
        cout << "SF bin " << i << ": " << par[i]/parMC[i] << " +- " << sqrt((parMC[i]*parMC[i]*err[i]*err[i]+par[i]*par[i]*errMC[i]*errMC[i])/(parMC[i]*parMC[i]*parMC[i]*parMC[i])) << endl;
    }
    
    // SF with MC from gen matching
    cout << "\nMC from gen matching:\n";
    TFile *fMatched = new TFile("../Results/Apr2017/DY/e_charge_flip_rebinned/chFlip.root");
    TH2D *matched = (TH2D*)fMatched->Get("charge_flip_probability_eta");
    for( size_t i = 0; i<5; ++i )
    {
        parMC[i] = matched->GetBinContent(i+1);
        errMC[i] = matched->GetBinError(i+1);
        cout << "Rate MC bin " << i << ": " << parMC[i] << " +- " << errMC[i] << endl;        
    }
        
    for( size_t i = 0; i<5; ++i )
    {
        cout << "SF bin " << i << ": " << par[i]/parMC[i] << " +- " << sqrt((parMC[i]*parMC[i]*err[i]*err[i]+par[i]*par[i]*errMC[i]*errMC[i])/(parMC[i]*parMC[i]*parMC[i]*parMC[i])) << endl;
    }
    
    // Charge flip after SFs
    TH2D *matched2D = (TH2D*)fMatched->Get("charge_flip_probability");
    for( size_t i = 0; i<5; ++i )
    {
        for( size_t j = 0; j<4; ++j )
        {
            Double_t SF = par[i]/parMC[i];
            Double_t SF_error = sqrt((parMC[i]*parMC[i]*err[i]*err[i]+par[i]*par[i]*errMC[i]*errMC[i])/(parMC[i]*parMC[i]*parMC[i]*parMC[i]));
            matched2D->SetBinContent(j+1, i+1, matched2D->GetBinContent(j+1, i+1) * SF );
            matched2D->SetBinError(j+1, i+1, sqrt( pow(matched2D->GetBinError(j+1, i+1) * SF,2) + pow(matched2D->GetBinContent(j+1, i+1) * SF_error,2) ) );
        }
    } 
    TFile *fOut = new TFile("charge_flip_probability.root","RECREATE");
    matched2D->Write();
}