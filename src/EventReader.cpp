
#include "EventReader.hpp"
#include "TChain.h"

EventReader::EventReader(EventContainer& eventCont, const ConfigContainer& cfgContainer)
: eventContainer(eventCont), configContainer(cfgContainer), treeReader(nullptr), nLeptons(0), nJets(0), isData(false), needJets(false), needGenJets(false), needPuppiJets(false), needGenLeptons(false), needElectronId(false), maxEventsWeight(1.) 
{ 
    bool firstGenJet = true, firstPuppiJet = true, firstJet = true, firstgenLepton = true, firstElectronID = true;
    vector<const vector<string>*> variableNames = {&configContainer.variableContainer.variableNames, &configContainer.cutContainer.variableNames};
    
    for( const auto* varNames : variableNames ) 
    {
        for( const string& iString : *varNames )
        {
            // Jets
            if( iString.find("genjet") != string::npos && firstGenJet )
            {
                needGenJets = true;
                needGenLeptons = true; // for cleaning jets
                genBranches.push_back("std_vector_jetGen_eta");
                genBranches.push_back("std_vector_jetGen_pt");
                genBranches.push_back("std_vector_jetGen_phi");
                firstGenJet = false;
            }
            else if( iString.find("puppijet") != string::npos && firstPuppiJet )
            {
                needPuppiJets = true;
                branches.push_back("std_vector_puppijet_eta");
                branches.push_back("std_vector_puppijet_pt");
                branches.push_back("std_vector_puppijet_phi");
                firstPuppiJet = false; 
            }
            else if( iString.find("jet") != string::npos && firstJet )
            {
                needJets = true;
                branches.push_back("std_vector_jet_eta");
                branches.push_back("std_vector_jet_pt");
                branches.push_back("std_vector_jet_phi");
                branches.push_back("std_vector_jet_mass");
                branches.push_back("std_vector_jet_csvv2ivf");
                firstJet = false; 
            } 
            
            // Gen leptons
            if( iString.find("genlepton") != string::npos && firstgenLepton )
            {
                needGenLeptons = true;
                genBranches.push_back("std_vector_leptonGen_eta");
                genBranches.push_back("std_vector_leptonGen_pt");
                genBranches.push_back("std_vector_leptonGen_phi");
                genBranches.push_back("std_vector_leptonGen_status");
                genBranches.push_back("std_vector_leptonGen_pid");
                firstgenLepton = false;
            }
            
            // Electrons
            if( iString.find("electron") != string::npos && firstElectronID )
            {
                needElectronId = true;
                branches.push_back("std_vector_electron_d0");
                branches.push_back("std_vector_electron_dEtaIn");
                branches.push_back("std_vector_electron_dPhiIn");
                branches.push_back("std_vector_electron_dz");
                branches.push_back("std_vector_electron_effectiveArea");
                branches.push_back("std_vector_electron_expectedMissingInnerHits");
                branches.push_back("std_vector_electron_full5x5_sigmaIetaIeta");
                branches.push_back("std_vector_electron_hOverE");
                branches.push_back("std_vector_electron_ooEmooP");
                branches.push_back("std_vector_electron_passConversionVeto");
                branches.push_back("std_vector_electron_scEta");
                firstElectronID = false;
            }
        }
    }
}

EventReader::~EventReader() {
    if( treeReader ) delete treeReader;    
}

void EventReader::setSample(unsigned int iSample, unsigned int iSubSample)
{
    // Delete treeReader for previous sample
    if( treeReader ) delete treeReader; 
//     extraBranches.clear();
    
    // Check arguments
    if( iSample >= configContainer.sampleContainer.reducedNames.size() || iSubSample >= configContainer.sampleContainer.sampleNames[iSample].size() )
    {
        cerr << "Indices for sample or subsample out of range in the Plotter::fill function." << endl;
        throw 1;
    }
    
    // Make new treeReader
    TFile* f = new TFile((configContainer.sampleDir + configContainer.sampleContainer.sampleNames[iSample][iSubSample] + string(".root") ).c_str(),"READ");
    TTree *t = (TTree*) f->Get(configContainer.treeName.c_str());                                                                                              
    treeReader = new TreeReader(t);
    
    
    // Set Branch statusses
    vector<string> sampleBranches= {"std_vector_lepton_eta","std_vector_lepton_pt","std_vector_lepton_phi","std_vector_lepton_flavour",
                      "std_vector_lepton_eleIdTight","std_vector_lepton_isMediumMuon","std_vector_lepton_BestTrackdxy","std_vector_lepton_BestTrackdz", "std_vector_lepton_sumPUPt",
                      "std_vector_lepton_chargedHadronIso","std_vector_lepton_neutralHadronIso","std_vector_lepton_photonIso", "trkMet","nvtx"};
    sampleBranches.insert(sampleBranches.end(), branches.begin(), branches.end());
    
    // Set data/MC weight branches       
    if( configContainer.sampleContainer.isData[iSample] )
    {
        isData = true;
        sampleBranches.push_back("trigger");        
    }
    else
    {
        isData = false;
        sampleBranches.push_back("baseW");        
        sampleBranches.push_back("GEN_weight_SM");
        sampleBranches.insert(sampleBranches.end(), genBranches.begin(), genBranches.end());
    }
        

    // FIXME
    // check cuts and variables for keywords and add to branches
    // bind functions to call them
//    fChain->SetBranchStatus("std_vector_jet_puid",1);
//    fChain->SetBranchStatus("std_vector_jet_QGlikelihood",1);
   
//    fChain->SetBranchStatus("std_vector_puppijet_eta",1);
//    fChain->SetBranchStatus("std_vector_puppijet_pt",1);
//    fChain->SetBranchStatus("std_vector_puppijet_phi",1);

    cout << "Reading out branches: \n";
    for( const string& branch : sampleBranches )
        cout << branch << endl;
    
    treeReader->setBranchStatus(sampleBranches);
    
    // Reset eventContainer
    treeReader->GetEntry(0);
    nJets = treeReader->std_vector_jet_pt->size();
    nLeptons = treeReader->std_vector_lepton_pt->size();
    eventContainer.reset();
    eventContainer.init(nLeptons, nJets);
    
    // Set maxEventsWeight
    if( !isData && configContainer.maxEvents > 0 && configContainer.maxEvents < treeReader->fChain->GetEntriesFast() )
    {
        maxEventsWeight = treeReader->fChain->GetEntriesFast() / configContainer.maxEvents;
    }
    else 
        maxEventsWeight = 1.;
    
    cout << "Reading sample: " << configContainer.sampleContainer.sampleNames[iSample][iSubSample] << endl;
}

bool EventReader::fillNextEvent()
{
    // Clear good object vectors
    eventContainer.goodLeptons.clear();
    eventContainer.goodJets.clear();
    eventContainer.goodGenLeptons.clear();
    eventContainer.goodGenJets.clear();
    eventContainer.goodPuppiJets.clear();
    
    // Read event (trigger == 1 for data)
    do
    {
        if( (!isData && eventContainer.iEvent == configContainer.maxEvents) || ! treeReader->GetEntry(eventContainer.iEvent) )
            return false;
        eventContainer.iEvent++;
    }
    while( isData && treeReader->trigger == 0); // Cut on trigger for data
    
    // Fill gen jets
    if( needGenJets )
    {
        for( unsigned int iJet=0; iJet < nJets; ++iJet ) {
            if( (*treeReader->std_vector_jetGen_pt)[iJet] > 0 )
            {
                eventContainer.genJets[iJet].set((*treeReader->std_vector_jetGen_pt)[iJet],(*treeReader->std_vector_jetGen_eta)[iJet],(*treeReader->std_vector_jetGen_phi)[iJet], 0);
                eventContainer.goodGenJets.push_back(iJet);
            }
            else
                break; // don't need to clean contents because it won't be used
        }
    }
    
    // Fill puppi jets
    if( needPuppiJets )
    {
        for( unsigned int iJet=0; iJet < nJets; ++iJet ) {
            if( (*treeReader->std_vector_puppijet_pt)[iJet] >= configContainer.minJetPt )
            {
                eventContainer.puppiJets[iJet].set((*treeReader->std_vector_puppijet_pt)[iJet],(*treeReader->std_vector_puppijet_eta)[iJet],(*treeReader->std_vector_puppijet_phi)[iJet],0);
//                 eventContainer.jets[iJet].setCsv((*treeReader->std_vector_jet_csvv2ivf)[iJet]);
                eventContainer.goodPuppiJets.push_back(iJet);
            }
            else
                break; // don't need to clean contents because it won't be used
        }
    }
    
    // Fill jets
    if( needJets )
    {
        for( unsigned int iJet=0; iJet < nJets; ++iJet ) {
            if( (*treeReader->std_vector_jet_pt)[iJet] >= configContainer.minJetPt )
            {
                eventContainer.jets[iJet].set((*treeReader->std_vector_jet_pt)[iJet],(*treeReader->std_vector_jet_eta)[iJet],(*treeReader->std_vector_jet_phi)[iJet],(*treeReader->std_vector_jet_mass)[iJet]);
                eventContainer.jets[iJet].setCsv((*treeReader->std_vector_jet_csvv2ivf)[iJet]);
                eventContainer.goodJets.push_back(iJet);
            }
            else
                break; // don't need to clean contents because it won't be used
        }
    }
    
    // Fill gen leptons
    if( needGenLeptons && !isData )
    {
        for( unsigned int iLepton=0; iLepton < nLeptons; ++iLepton ) {
            if( (*treeReader->std_vector_leptonGen_status)[iLepton] == 1 && (*treeReader->std_vector_leptonGen_pt)[iLepton] > 0)
            {                
                eventContainer.goodGenLeptons.push_back(iLepton);
                eventContainer.genLeptons[iLepton].set((*treeReader->std_vector_leptonGen_pt)[iLepton],(*treeReader->std_vector_leptonGen_eta)[iLepton],(*treeReader->std_vector_leptonGen_phi)[iLepton],(*treeReader->std_vector_leptonGen_pid)[iLepton] );                
            }
        }
    }
    
    // Fill leptons
    for( unsigned int iLepton=0; iLepton < nLeptons; ++iLepton ) {
        if( (*treeReader->std_vector_lepton_pt)[iLepton] >= configContainer.minLeptonPt )
        {
            eventContainer.goodLeptons.push_back(iLepton);
            
            if( abs((*treeReader->std_vector_lepton_flavour)[iLepton]) == 11 )
            {
                eventContainer.leptons[iLepton].set((*treeReader->std_vector_lepton_pt)[iLepton],(*treeReader->std_vector_lepton_eta)[iLepton],(*treeReader->std_vector_lepton_phi)[iLepton],(*treeReader->std_vector_lepton_flavour)[iLepton], (*treeReader->std_vector_lepton_eleIdTight)[iLepton] == 1 );
                if( needElectronId )
                {
                    eventContainer.leptons[iLepton].setd0((*treeReader->std_vector_electron_d0)[iLepton]);
                    eventContainer.leptons[iLepton].setdEtaIn((*treeReader->std_vector_electron_dEtaIn)[iLepton]);
                    eventContainer.leptons[iLepton].setdPhiIn((*treeReader->std_vector_electron_dPhiIn)[iLepton]);
                    eventContainer.leptons[iLepton].setdz((*treeReader->std_vector_electron_dz)[iLepton]);
                    eventContainer.leptons[iLepton].seteffectiveArea((*treeReader->std_vector_electron_effectiveArea)[iLepton]);
                    eventContainer.leptons[iLepton].setmissingHits((*treeReader->std_vector_electron_expectedMissingInnerHits)[iLepton]);
                    eventContainer.leptons[iLepton].setsigmaIetaIeta((*treeReader->std_vector_electron_full5x5_sigmaIetaIeta)[iLepton]);
                    eventContainer.leptons[iLepton].sethOverE((*treeReader->std_vector_electron_full5x5_sigmaIetaIeta)[iLepton]);
                    eventContainer.leptons[iLepton].setooEmoop((*treeReader->std_vector_electron_full5x5_sigmaIetaIeta)[iLepton]);
                    eventContainer.leptons[iLepton].setconversionVeto((*treeReader->std_vector_electron_passConversionVeto)[iLepton]);
                    eventContainer.leptons[iLepton].setscEta((*treeReader->std_vector_electron_scEta)[iLepton]);
                    eventContainer.leptons[iLepton].setIsolation( ((*treeReader->std_vector_lepton_chargedHadronIso)[iLepton] + (*treeReader->std_vector_lepton_neutralHadronIso)[iLepton] + (*treeReader->std_vector_lepton_photonIso)[iLepton] ) / (*treeReader->std_vector_lepton_pt)[iLepton] );
                }
            }
            else if( abs((*treeReader->std_vector_lepton_flavour)[iLepton]) == 13 )
            {
                eventContainer.leptons[iLepton].set(
                    (*treeReader->std_vector_lepton_pt)[iLepton],
                    (*treeReader->std_vector_lepton_eta)[iLepton],
                    (*treeReader->std_vector_lepton_phi)[iLepton],
                    (*treeReader->std_vector_lepton_flavour)[iLepton],
                    ((*treeReader->std_vector_lepton_isMediumMuon)[iLepton] == 1) && ( (*treeReader->std_vector_lepton_chargedHadronIso)[iLepton] + TMath::Max((*treeReader->std_vector_lepton_neutralHadronIso)[iLepton] + (*treeReader->std_vector_lepton_photonIso)[iLepton] - 0.5 * (*treeReader->std_vector_lepton_sumPUPt)[iLepton], 0.)) / (*treeReader->std_vector_lepton_pt)[iLepton] < 0.15 && (*treeReader->std_vector_lepton_BestTrackdxy)[iLepton] < 0.02 && (*treeReader->std_vector_lepton_BestTrackdz)[iLepton] < 0.1 );
            }
            else
            {
                cerr << "Unexpected lepton id: " << (*treeReader->std_vector_lepton_flavour)[iLepton] << endl;
                throw 1;
            }
        }
        else
            break;
    }
    
    // Fill MET 
    eventContainer.met.set( treeReader->trkMet, 0);
    
    // Fill number of vertices
    eventContainer.setNvertices( treeReader->nvtx );
    
    // Fill weight
    eventContainer.setWeight( treeReader->baseW * (treeReader->GEN_weight_SM/abs(treeReader->GEN_weight_SM)) * maxEventsWeight);
        
    // Fill extra variables
//     for( const auto& iPair : extraBranches )
//     {
//         iPair.first = iPair.second;        
//     }


    return true;
}