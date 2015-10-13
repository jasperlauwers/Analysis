
#include "EventReader.hpp"
#include "TChain.h"

EventReader::EventReader(EventContainer& eventCont, const ConfigContainer& cfgContainer)
: eventContainer(eventCont), configContainer(cfgContainer), treeReader(nullptr), nLeptons(0), nJets(0), isData(false) { }

EventReader::~EventReader() {
    if( treeReader ) delete treeReader;    
}

void EventReader::setSample(unsigned int iSample, unsigned int iSubSample)
{
    // Delete treeReader for previous sample
    if( treeReader ) delete treeReader; 
    extraBranches.clear();
    
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
    vector<string> branches= {"std_vector_jet_eta","std_vector_jet_pt","std_vector_jet_phi","std_vector_jet_mass",
                      "std_vector_lepton_eta","std_vector_lepton_pt","std_vector_lepton_phi","std_vector_lepton_flavour",
                      "std_vector_lepton_eleIdMedium","std_vector_lepton_isMediumMuon",
                      "std_vector_lepton_chargedHadronIso","std_vector_lepton_neutralHadronIso","std_vector_lepton_photonIso"};
                      
    if( configContainer.sampleContainer.isData[iSample] )
    {
        isData = true;
        branches.push_back("trigger");        
    }
    else
    {
        isData = false;
        branches.push_back("baseW");
//         pair<float&,float&> temp = {eventContainer.baseW, treeReader->baseW};
//         extraBranches.push_back( temp );
        
        branches.push_back("GEN_weight_SM");
    }

    // FIXME
    // check cuts and variables for keywords and add to branches
    // bind functions to call them
//    fChain->SetBranchStatus("std_vector_jet_puid",1);
//    fChain->SetBranchStatus("std_vector_jet_QGlikelihood",1);
   
//    fChain->SetBranchStatus("std_vector_puppijet_eta",1);
//    fChain->SetBranchStatus("std_vector_puppijet_pt",1);
//    fChain->SetBranchStatus("std_vector_puppijet_phi",1);
    treeReader->setBranchStatus(branches);
    
    // Reset eventContainer
    treeReader->GetEntry(0);
    nJets = treeReader->std_vector_jet_pt->size();
    nLeptons = treeReader->std_vector_lepton_pt->size();
    eventContainer.reset();
    eventContainer.init(nLeptons, nJets);
    
    cout << "Reading sample: " << configContainer.sampleContainer.sampleNames[iSample][iSubSample] << endl;
}

bool EventReader::fillNextEvent()
{
    do
    {
        if( ! treeReader->GetEntry(eventContainer.iEvent) )
            return false;
        eventContainer.iEvent++;
    }
    while( isData && treeReader->trigger == 0); // Cut on trigger for data
    
    // Fill jets
    for( unsigned int iJet=0; iJet < nJets; ++iJet ) {
        if( (*treeReader->std_vector_jet_pt)[iJet] > 0 )
            eventContainer.jets[iJet].set((*treeReader->std_vector_jet_pt)[iJet],(*treeReader->std_vector_jet_eta)[iJet],(*treeReader->std_vector_jet_phi)[iJet],(*treeReader->std_vector_jet_mass)[iJet]);   
        else
            eventContainer.jets[iJet].set(0.,0.,0.,0.);
    }
    
    // Fill leptons
    for( unsigned int iLepton=0; iLepton < nLeptons; ++iLepton ) {
        if( abs((*treeReader->std_vector_lepton_flavour)[iLepton]) == 11 )
        {
            eventContainer.leptons[iLepton].set((*treeReader->std_vector_lepton_pt)[iLepton],(*treeReader->std_vector_lepton_eta)[iLepton],(*treeReader->std_vector_lepton_phi)[iLepton],(*treeReader->std_vector_lepton_flavour)[iLepton], (*treeReader->std_vector_lepton_eleIdMedium)[iLepton] == 1 );
        }
        else if( abs((*treeReader->std_vector_lepton_flavour)[iLepton]) == 13 )
        {
            eventContainer.leptons[iLepton].set(
                (*treeReader->std_vector_lepton_pt)[iLepton],
                (*treeReader->std_vector_lepton_eta)[iLepton],
                (*treeReader->std_vector_lepton_phi)[iLepton],
                (*treeReader->std_vector_lepton_flavour)[iLepton],
                ((*treeReader->std_vector_lepton_isMediumMuon)[iLepton] == 1) && ( (*treeReader->std_vector_lepton_chargedHadronIso)[iLepton] + (*treeReader->std_vector_lepton_neutralHadronIso)[iLepton] + (*treeReader->std_vector_lepton_photonIso)[iLepton] ) / (*treeReader->std_vector_lepton_pt)[iLepton] < 0.20 );
        }
        else
        {
            eventContainer.leptons[iLepton].set(0.,0.,0.,0.,false);            
        }
    }
    
    // Fill weight
    eventContainer.setWeight( treeReader->baseW * (treeReader->GEN_weight_SM/abs(treeReader->GEN_weight_SM)) );
        
    // Fill extra variables
    for( const auto& iPair : extraBranches )
    {
        iPair.first = iPair.second;        
    }
    
    // Clear good object vectors
    eventContainer.goodLeptons.clear();
    eventContainer.goodJets.clear();

    return true;
}