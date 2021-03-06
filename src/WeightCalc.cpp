
#include "WeightCalc.hpp"

WeightCalc::WeightCalc(EventContainer& evContainer, bool latinoFakes)
: eventContainer(evContainer), applyDYWeight(false), applyFakeWeight(false), useElectronCorrectedPt(false), useMuonCorrectedPt(false), useTwoMuonFR(false), useTwoElectronFR(false), latinoFakes(latinoFakes), trileptonFakes(false) { }

WeightCalc::WeightCalc(EventContainer& evContainer, bool latinoFakes, bool trileptonFakes)
: eventContainer(evContainer), applyDYWeight(false), applyFakeWeight(false), useElectronCorrectedPt(false), useMuonCorrectedPt(false), useTwoMuonFR(false), useTwoElectronFR(false), latinoFakes(latinoFakes), trileptonFakes(trileptonFakes) { }

WeightCalc::~WeightCalc() 
{
    // Need to be cleaned up after detaching histrograms from file
    if( applyFakeWeight ) 
    {
        hFakeElectron->Delete();
        hFakeMuon->Delete();
        if(useTwoMuonFR) 
            hFakeMuon2->Delete();
        if(useTwoElectronFR) 
            hFakeElectron2->Delete();
        hPromptElectron->Delete();
        hPromptMuon->Delete();
    }
}

void WeightCalc::initFakeWeight(FakeContainer* fContainer)
{ 
    // Only one time
    if( applyFakeWeight )
        return;
    
    fakeContainer = fContainer;
    
    if( fakeContainer->fakeMuonFile2.size() > 0 ) 
        useTwoMuonFR = true;
    if( fakeContainer->fakeElectronFile2.size() > 0 ) 
        useTwoElectronFR = true;
    
    TFile* fFakeElectron = new TFile(fakeContainer->fakeElectronFile.c_str(),"READ");
    hFakeElectron = (TH2D*) fFakeElectron->Get(fakeContainer->fakeElectronHist.c_str());
    hFakeElectron->SetDirectory(0); // "detach" the histogram from the file
    if (fakeContainer->maxPtElectronFake <= 0.)
        fakeContainer->maxPtElectronFake = hFakeElectron->GetXaxis()->GetBinCenter(hFakeElectron->GetNbinsX());
    
    if( useTwoElectronFR )
    {
        TFile* fFakeElectron2 = new TFile(fakeContainer->fakeElectronFile2.c_str(),"READ");
        hFakeElectron2 = (TH2D*) fFakeElectron2->Get(fakeContainer->fakeElectronHist2.c_str());
        hFakeElectron2->SetDirectory(0); // "detach" the histogram from the file
        if (fakeContainer->maxPtElectronFake2 <= 0.)
            fakeContainer->maxPtElectronFake2 = hFakeElectron2->GetXaxis()->GetBinCenter(hFakeElectron2->GetNbinsX());
    }
    
    TFile* fFakeMuon = new TFile(fakeContainer->fakeMuonFile.c_str(),"READ");
    hFakeMuon = (TH2D*) fFakeMuon->Get(fakeContainer->fakeMuonHist.c_str());
    hFakeMuon->SetDirectory(0); // "detach" the histogram from the file
    if (fakeContainer->maxPtMuonFake <= 0.)
        fakeContainer->maxPtMuonFake = hFakeMuon->GetXaxis()->GetBinCenter(hFakeMuon->GetNbinsX());
    
    if( useTwoMuonFR )
    {
        TFile* fFakeMuon2 = new TFile(fakeContainer->fakeMuonFile2.c_str(),"READ");
        hFakeMuon2 = (TH2D*) fFakeMuon2->Get(fakeContainer->fakeMuonHist2.c_str());
        hFakeMuon2->SetDirectory(0); // "detach" the histogram from the file
        if (fakeContainer->maxPtMuonFake2 <= 0.)
            fakeContainer->maxPtMuonFake2 = hFakeMuon2->GetXaxis()->GetBinCenter(hFakeMuon2->GetNbinsX());
    }
    
    TFile* fPromptElectron = new TFile(fakeContainer->promptElectronFile.c_str(),"READ");
    hPromptElectron = (TH2D*) fPromptElectron->Get(fakeContainer->promptElectronHist.c_str());
    hPromptElectron->SetDirectory(0); // "detach" the histogram from the file
    if (fakeContainer->maxPtElectronPrompt <= 0.)
        fakeContainer->maxPtElectronPrompt = hPromptElectron->GetXaxis()->GetBinCenter(hPromptElectron->GetNbinsX());
    
    TFile* fPromptMuon = new TFile(fakeContainer->promptMuonFile.c_str(),"READ");
    hPromptMuon = (TH2D*) fPromptMuon->Get(fakeContainer->promptMuonHist.c_str());
    hPromptMuon->SetDirectory(0); // "detach" the histogram from the file
    if (fakeContainer->maxPtMuonPrompt <= 0.)
        fakeContainer->maxPtMuonPrompt = hPromptMuon->GetXaxis()->GetBinCenter(hPromptMuon->GetNbinsX());
    
    if( fakeContainer->fakeElectronHist.find("correctedpt") != string::npos )
    {
        useElectronCorrectedPt = true;
        cout << "Fake electron estimation from corrected pt" << endl;
    }
    else
        cout << "Fake electron estimation from pt" << endl;
    if( fakeContainer->fakeMuonHist.find("correctedpt") != string::npos )
    {
        useMuonCorrectedPt = true;
        cout << "Fake muon estimation from corrected pt" << endl;
    }
    else
        cout << "Fake muon estimation from pt" << endl;
    
    applyFakeWeight = true;
}
void WeightCalc::initDYWeight(const EventReader& reader)
{
    // Only one time
    if( applyDYWeight )
        return;
    
//     reader.getDYWeights(DYweights);
    applyDYWeight = true;
    
    // detajj weight
//     TFile* f = new TFile("DYWeight/DY_LOtoNLO_weight.root","READ");
//     hDYshapeWeight = (TH1D*) f->Get("DY_detajj_weight"); //contains shape + NLO correction
//     maxVal = 3.9;
    
    // lepton eta weight
//     TFile* f = new TFile("DYWeight/DY_SSoverOS_2Dweight_2jet_MCsubtr_Feb2017.root","READ");
//     hDYshapeWeight = (TH2D*) f->Get("data_leptonabseta2D_weight"); //contains shape + normalization
    
    // gen matched weight
    TFile* f = new TFile("DYWeight/charge_flip_probability.root","READ");
    hDYshapeWeight = (TH2D*) f->Get("charge_flip_probability"); //contains shape + normalization
    maxPtChargeHist = hDYshapeWeight->GetXaxis()->GetBinCenter(hDYshapeWeight->GetNbinsX());
}

void WeightCalc::setWeight(SampleType sampleType, const string& sampleName) 
{
    // Fake weigth
    if( applyFakeWeight && (sampleType == SampleType::FAKELEPTON || sampleType == SampleType::MCFAKELEPTON) )
    {
        if ( eventContainer.looseLeptons[1].pt() < 0 ) 
        {
            cerr << "Not enough leptons for fake weight calculation." << endl;
            throw 1;
        }
        
        unsigned int totLep = 2;
        if( trileptonFakes )
            totLep = 3;
        
        float fakeWeight, promtFakeWeight = 1;
        float p, f/*, pE, fE*/;
        unsigned int nTight = 0;
        float promptProbability[3], fakeProbability[3];
        for( unsigned int iLep=0; iLep < totLep; ++iLep ) 
        {        
//      cout << "Lept pt: " << eventContainer.looseleptonpt(iLep) << ", abs(eta): " << eventContainer.looseleptonabseta(iLep) << endl;
            if( eventContainer.looseLeptons[iLep].isElectron() )
            {
                // Fix for electrons with |eta| > 2.4
                float abseta = eventContainer.looseleptonabseta(iLep) ;
                if( abseta > 2.4 ) {
                    abseta = 2.39;
                }
                
                TH2D* hFakeElectronFinal = hFakeElectron;
                float maxPt = fakeContainer->maxPtElectronFake;
                if( useTwoElectronFR && iLep )
                {
                    hFakeElectronFinal = hFakeElectron2;
                    maxPt = fakeContainer->maxPtElectronFake2;
                }
                
                p = hPromptElectron->GetBinContent(hPromptElectron->FindBin(min(eventContainer.looseleptonpt(iLep), fakeContainer->maxPtElectronPrompt), abseta));
                if( useElectronCorrectedPt )
                    f = hFakeElectronFinal->GetBinContent(hFakeElectronFinal->FindBin(min(eventContainer.looseleptoncorrectedpt(iLep), maxPt), abseta));
                else
                    f = hFakeElectronFinal->GetBinContent(hFakeElectronFinal->FindBin(min(eventContainer.looseleptonpt(iLep), maxPt), abseta));
    //             cout << "p:\t" << p << "\tf:\t" << f << endl;
            }
            else
            {
                TH2D* hFakeMuonFinal = hFakeMuon;
                float maxPt = fakeContainer->maxPtMuonFake;
                if( useTwoMuonFR && iLep )
                {
                    hFakeMuonFinal = hFakeMuon2;
                    maxPt = fakeContainer->maxPtMuonFake2;
                }
                p = hPromptMuon->GetBinContent(hPromptMuon->FindBin(min(eventContainer.looseleptonpt(iLep), fakeContainer->maxPtMuonPrompt), abs(eventContainer.looseleptonabseta(iLep))));
                if( useMuonCorrectedPt )
                    f = hFakeMuonFinal->GetBinContent(hFakeMuonFinal->FindBin(min(eventContainer.looseleptoncorrectedpt(iLep), maxPt), abs(eventContainer.looseleptonabseta(iLep))));
                else
                    f = hFakeMuonFinal->GetBinContent(hFakeMuonFinal->FindBin(min(eventContainer.looseleptonpt(iLep), maxPt), abs(eventContainer.looseleptonabseta(iLep))));
            }
            
            if( eventContainer.looseLeptons[iLep].passesMedium() && (eventContainer.looseLeptons[iLep].isMuon() || eventContainer.looseLeptons[iLep].tripleChargeAgreement()) )
            {
                nTight++;
    //             cout << "Tight lepton" << endl;
                promptProbability[iLep] = p * (1 - f);
                fakeProbability[iLep]   = f * (1 - p);
            }
            else
            {
                promptProbability[iLep] = p * f;
                fakeProbability[iLep]   = p * f; 
                promtFakeWeight *= f/(1-f);
            }
            promptProbability[iLep] /= (p - f);
            fakeProbability[iLep]   /= (p - f);
        }
        float PF = promptProbability[0] * fakeProbability  [1];
        float FP = fakeProbability  [0] * promptProbability[1];
        float FF = fakeProbability  [0] * fakeProbability  [1];

        if (nTight == 1) 
            FF *= -1.;
        else
        {
            PF *= -1.;
            FP *= -1.;
                
            if( nTight == totLep-2 )
                promtFakeWeight *= -1;
            if( nTight == totLep || nTight < totLep-2 )
                promtFakeWeight = 0;
        }
        
        if( latinoFakes ) 
            fakeWeight = PF + FP + FF;
        else
            fakeWeight = promtFakeWeight;
        
//      cout << "Central weight: " << eventContainer.weight() << endl;
//      cout << "New weight: " << PF + FP + FF << endl;
        float newWeight = eventContainer.weight();
        if( sampleType == SampleType::MCFAKELEPTON )
            newWeight *= fakeWeight;
        else
            newWeight = fakeWeight;
        eventContainer.setWeight(newWeight);
    }
    
    // DY   
    if( applyDYWeight && (sampleName.find("DY") != string::npos || sampleName.find("TTTo2L2Nu") != string::npos || sampleName.find("ST_tW") != string::npos  || sampleName.find("GluGluWWTo2L2Nu") != string::npos|| sampleName.find("WWTo2L2Nu.root") != string::npos) /*&& sampleName.find("M-10") == string::npos*/ )
    {    
        float weight = eventContainer.weight();
        
//         unsigned int nJets = eventContainer.genjetnhardprocess();
//         weight *= DYweights[nJets];
        
//         if( sampleName.find("DYJetsToLL_M-50_") == string::npos )
//             weight *= hDYshapeWeight->GetBinContent(hDYshapeWeight->FindBin(min(eventContainer.detajj(), maxVal)));
//         if( eventContainer.channel() == 1 ) 
//         {
//             weight *= hDYshapeWeight->GetBinContent(hDYshapeWeight->FindBin( eventContainer.leptonabseta(0), eventContainer.leptonabseta(1))); 
//             eventContainer.setWeight(weight); 
        float chargeMisIdFactor = 0;
        for( auto iLep : eventContainer.goodLeptons )
        {      
//      cout << "Lept pt: " << eventContainer.looseleptonpt(iLep) << ", abs(eta): " << eventContainer.looseleptonabseta(iLep) << endl;
            if( eventContainer.leptons[iLep].isElectron() )
            { 
                float pt = eventContainer.leptons[iLep].pt();
                if( pt > maxPtChargeHist ) 
                   pt = maxPtChargeHist;
                chargeMisIdFactor += ( hDYshapeWeight->GetBinContent(hDYshapeWeight->FindBin( pt, abs(eventContainer.leptons[iLep].eta()) )) );
            }
        }
        if( chargeMisIdFactor > 0 )
            eventContainer.setWeight(weight*chargeMisIdFactor); 
    }
}
