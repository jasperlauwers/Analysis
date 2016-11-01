
#include "WeightCalc.hpp"

WeightCalc::WeightCalc(EventContainer& evContainer)
: eventContainer(evContainer), applyDYWeight(false), applyFakeWeight(false), useElectronCorrectedPt(false), useMuonCorrectedPt(false), useTwoMuonFR(false) { }

WeightCalc::~WeightCalc() 
{
    // Need to be cleaned up after detaching histrograms from file
    if( applyFakeWeight ) 
    {
        hFakeElectron->Delete();
        hFakeMuon->Delete();
        if(useTwoMuonFR) 
            hFakeMuon2->Delete();
        hPromptElectron->Delete();
        hPromptMuon->Delete();
    }
}

void WeightCalc::initFakeWeight(FakeContainer* fContainer)
{ 
    // Only one time
    if( applyFakeWeight )
        return;
    
    if( fakeContainer->fakeMuonFile2.size() > 0 ) 
        useTwoMuonFR = true;
    
    fakeContainer = fContainer;
    
    TFile* fFakeElectron = new TFile(fakeContainer->fakeElectronFile.c_str(),"READ");
    hFakeElectron = (TH2F*) fFakeElectron->Get(fakeContainer->fakeElectronHist.c_str());
    hFakeElectron->SetDirectory(0); // "detach" the histogram from the file
    if (fakeContainer->maxPtElectronFake <= 0.)
        fakeContainer->maxPtElectronFake = hFakeElectron->GetXaxis()->GetBinCenter(hFakeElectron->GetNbinsX());
    
    TFile* fFakeMuon = new TFile(fakeContainer->fakeMuonFile.c_str(),"READ");
    hFakeMuon = (TH2F*) fFakeMuon->Get(fakeContainer->fakeMuonHist.c_str());
    hFakeMuon->SetDirectory(0); // "detach" the histogram from the file
    if (fakeContainer->maxPtMuonFake <= 0.)
        fakeContainer->maxPtMuonFake = hFakeMuon->GetXaxis()->GetBinCenter(hFakeMuon->GetNbinsX());
    
    if( useTwoMuonFR )
    {
        TFile* fFakeMuon2 = new TFile(fakeContainer->fakeMuonFile2.c_str(),"READ");
        hFakeMuon2 = (TH2F*) fFakeMuon2->Get(fakeContainer->fakeMuonHist2.c_str());
        hFakeMuon2->SetDirectory(0); // "detach" the histogram from the file
        if (fakeContainer->maxPtMuonFake2 <= 0.)
            fakeContainer->maxPtMuonFake2 = hFakeMuon2->GetXaxis()->GetBinCenter(hFakeMuon2->GetNbinsX());
    }
    
    TFile* fPromptElectron = new TFile(fakeContainer->promptElectronFile.c_str(),"READ");
    hPromptElectron = (TH2F*) fPromptElectron->Get(fakeContainer->promptElectronHist.c_str());
    hPromptElectron->SetDirectory(0); // "detach" the histogram from the file
    if (fakeContainer->maxPtElectronPrompt <= 0.)
        fakeContainer->maxPtElectronPrompt = hPromptElectron->GetXaxis()->GetBinCenter(hPromptElectron->GetNbinsX());
    
    TFile* fPromptMuon = new TFile(fakeContainer->promptMuonFile.c_str(),"READ");
    hPromptMuon = (TH2F*) fPromptMuon->Get(fakeContainer->promptMuonHist.c_str());
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
//     hDYshapeWeight = (TH1F*) f->Get("DY_detajj_weight"); //contains shape + NLO correction
//     maxVal = 3.9;
    
    // lepton eta weight
    TFile* f = new TFile("DYWeight/DY_SSoverOS_2Dweight_2jet_tripleCharge.root","READ");
    hDYshapeWeight = (TH2F*) f->Get("data_leptonabseta2D_weight"); //contains shape + normalization
}

void WeightCalc::setWeight(SampleType sampleType, const string& sampleName) 
{
    // Fake weigth
    if( applyFakeWeight && sampleType == SampleType::FAKELEPTON )
    {
        if ( eventContainer.looseLeptons[1].pt() < 0 ) 
        {
            cerr << "Not enough leptons for fake weight calculation." << endl;
            throw 1;
        }
        
        float p, f/*, pE, fE*/;
        unsigned int nTight = 0;
        float promptProbability[2], fakeProbability[2];
        for( unsigned int iLep=0; iLep < 2; ++iLep ) 
        {        
//      cout << "Lept pt: " << eventContainer.looseleptonpt(iLep) << ", abs(eta): " << eventContainer.looseleptonabseta(iLep) << endl;
            if( eventContainer.looseLeptons[iLep].isElectron() )
            {
                // Fix for electrons with |eta| > 2.4
                if( eventContainer.looseleptonabseta(iLep) > 2.4 ) {
                    eventContainer.setWeight(0);
                    return;
                }
                
                p = hPromptElectron->GetBinContent(hPromptElectron->FindBin(min(eventContainer.looseleptonpt(iLep), fakeContainer->maxPtElectronPrompt), abs(eventContainer.looseleptonabseta(iLep))));
                if( useElectronCorrectedPt )
                    f = hFakeElectron->GetBinContent(hFakeElectron->FindBin(min(eventContainer.looseleptoncorrectedpt(iLep), fakeContainer->maxPtElectronFake), abs(eventContainer.looseleptonabseta(iLep))));
                else
                    f = hFakeElectron->GetBinContent(hFakeElectron->FindBin(min(eventContainer.looseleptonpt(iLep), fakeContainer->maxPtElectronFake), abs(eventContainer.looseleptonabseta(iLep))));
    //             cout << "p:\t" << p << "\tf:\t" << f << endl;
            }
            else
            {
                TH2F* hFakeMuonFinal = hFakeMuon;
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
            
            if( eventContainer.looseLeptons[iLep].passesMedium() )
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
        }
        
//      cout << "Central weight: " << eventContainer.weight() << endl;
//      cout << "New weight: " << PF + FP + FF << endl;
        eventContainer.setWeight(PF + FP + FF);
    }
    
    // DY   
    if( applyDYWeight && sampleName.find("DY") != string::npos /*&& sampleName.find("M-10") == string::npos*/ )
    {    
        float weight = eventContainer.weight();
        
//         unsigned int nJets = eventContainer.genjetnhardprocess();
//         weight *= DYweights[nJets];
        
//         if( sampleName.find("DYJetsToLL_M-50_") == string::npos )
//             weight *= hDYshapeWeight->GetBinContent(hDYshapeWeight->FindBin(min(eventContainer.detajj(), maxVal)));
        if( eventContainer.channel() == 1 ) 
        {
            weight *= hDYshapeWeight->GetBinContent(hDYshapeWeight->FindBin( eventContainer.leptonabseta(0), eventContainer.leptonabseta(1))); 
            eventContainer.setWeight(weight); 
        }
    }
}