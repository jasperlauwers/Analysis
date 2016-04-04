
#include "EventReader.hpp"
#include "TChain.h"

EventReader::EventReader(EventContainer& eventCont, const ConfigContainer& cfgContainer)
: eventContainer(eventCont), configContainer(cfgContainer), treeReader(nullptr), nLeptons(0), nJets(0), needJets(false), needGenJets(false), needPuppiJets(false), needGenLeptons(false), needElectronId(false), needTrackJets(false), triggerSelection(false), hasNegWeight(false), sampleType(SampleType::DATA), maxEventsWeight(1.) 
{ 
    bool firstGenJet = true, firstPuppiJet = true, firstJet = true, firstgenLepton = true, firstElectronID = true, firstTrackJet = true, firstLooseLepton=true;
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
            else if( iString.find("trackjet") != string::npos && firstTrackJet )
            {
                needTrackJets = true;
                branches.push_back("std_vector_trackjet_eta");
                branches.push_back("std_vector_trackjet_pt");
                branches.push_back("std_vector_trackjet_phi");
                firstTrackJet = false; 
            }
            else if( iString.find("jet") != string::npos && firstJet )
            {
                needJets = true;
                branches.push_back("std_vector_jet_eta");
                branches.push_back("std_vector_jet_pt");
                branches.push_back("std_vector_jet_phi");
                branches.push_back("std_vector_jet_mass");
                branches.push_back("std_vector_jet_cmvav2");
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
                genBranches.push_back("std_vector_leptonGen_isPrompt");
                genBranches.push_back("std_vector_leptonGen_isDirectPromptTauDecayProduct");
                firstgenLepton = false;
            }
            
            // Electrons
            if( iString.find("electron") != string::npos && firstElectronID )
            {
                needElectronId = true;
                branches.push_back("std_vector_electron_dEtaIn");
                branches.push_back("std_vector_electron_dPhiIn");
                branches.push_back("std_vector_electron_effectiveArea");
                branches.push_back("std_vector_electron_expectedMissingInnerHits");
                branches.push_back("std_vector_electron_full5x5_sigmaIetaIeta");
                branches.push_back("std_vector_electron_hOverE");
                branches.push_back("std_vector_electron_ooEmooP");
                branches.push_back("std_vector_electron_passConversionVeto");
                branches.push_back("std_vector_electron_scEta");
//                 "std_vector_lepton_d0","std_vector_lepton_dz", "std_vector_lepton_sumPUPt",
//                       "std_vector_lepton_chargedHadronIso","std_vector_lepton_neutralHadronIso","std_vector_lepton_photonIso", "jetRho",
                firstElectronID = false;
            }
            
            // Loose lepton
            if( iString.find("loose") != string::npos && firstLooseLepton )
            {
                needLooseLeptons = true;
                branches.push_back("std_vector_lepton_isTightLepton");
//                 genBranches.push_back("std_vector_looseLepton_eta");
//                 genBranches.push_back("std_vector_looseLepton_pt");
//                 genBranches.push_back("std_vector_looseLepton_phi");
//                 genBranches.push_back("std_vector_looseLepton_flavour");
                firstLooseLepton = false;
            }
            
            // Trigger
            if( iString.find("trigger") != string::npos )
            {
                triggerSelection = true;
            }
        }
    }
    // Trigger
    if( configContainer.cutContainer.triggerVector.size() > 0 )
    {
        triggerSelection = true;
    }
}

EventReader::~EventReader() {
    if( treeReader ) delete treeReader;    
}

void EventReader::addReadBranches(const vector<string>& readBranches)
{
    branches.insert(end(branches), begin(readBranches), end(readBranches));
}

bool EventReader::setSample(unsigned int iSample, unsigned int iSubSample)
{
    cout << endl << "Reading sample: " << configContainer.sampleContainer.sampleNames[iSample][iSubSample] << endl;
    
    // Check arguments
    if( iSample >= configContainer.sampleContainer.reducedNames.size() || iSubSample >= configContainer.sampleContainer.sampleNames[iSample].size() )
    {
        cerr << "Indices for sample or subsample out of range in the Plotter::fill function." << endl;
        throw 1;
    }
    
    // Check if data should be blinded
    if( configContainer.sampleContainer.sampleType[iSample] == SampleType::DATA && ! configContainer.unblind )
    {
        cout << "Skipping sample, for unblinding set Unblind = true" << endl;
        return false;
    }
        
    // Delete treeReader for previous sample
    if( treeReader ) delete treeReader; 
//     extraBranches.clear();
    

    
    // Set TTree or TChain
    TTree *t;
    if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("*") != string::npos )
    {
        TChain* tChain = new TChain(configContainer.treeName.c_str());
        DIR *dpdf;
        struct dirent *epdf;
        unsigned int nFiles = 0;
        
        string inDir = configContainer.sampleDir;
        if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("/") != string::npos )
        {
            inDir+=configContainer.sampleContainer.sampleNames[iSample][iSubSample].substr(0, configContainer.sampleContainer.sampleNames[iSample][iSubSample].find_last_of('/')+1); // +1: add last slash
        }
        dpdf = opendir(inDir.c_str());
        if (dpdf != NULL)
        {
            while ((epdf = readdir(dpdf)))
            {
                string fname = epdf->d_name;
                string reString = "("+configContainer.sampleContainer.sampleNames[iSample][iSubSample].substr(configContainer.sampleContainer.sampleNames[iSample][iSubSample].find_last_of('/')+1) + ")";
                reString.replace(reString.find("*"),1,")(.*)(");
                regex regexp(reString);
                
                if ( regex_match( fname, regexp ) ) {
                    cout << "File matches Regex: " << fname << endl;
                    tChain->Add((inDir+fname).c_str());
                    nFiles++;
                }
            }
            if( nFiles < 1 )
            {
                cerr << "Regex did not match any file: " << configContainer.sampleContainer.sampleNames[iSample][iSubSample].substr(configContainer.sampleContainer.sampleNames[iSample][iSubSample].find_last_of('/')+1) << endl;
                throw 1;
            }
        }
        else
        {
            cerr << "Directory not found: " << inDir << endl;
            throw 1;
        }
        t = tChain;
    }
    else
    {
        TFile* f = new TFile((configContainer.sampleDir + configContainer.sampleContainer.sampleNames[iSample][iSubSample] ).c_str(),"READ");
        t = (TTree*) f->Get(configContainer.treeName.c_str());                                                                                              
    }  
    
    // Make new treeReader
    treeReader = new TreeReader(t);
    
    // Set Branch statusses
    vector<string> sampleBranches= {"std_vector_lepton_eta","std_vector_lepton_pt","std_vector_lepton_phi","std_vector_lepton_flavour", "metPfType1","metPfType1Phi","nvtx"};
    sampleBranches.insert(sampleBranches.end(), branches.begin(), branches.end());
    
    // Set data/MC weight branches       
    sampleType = configContainer.sampleContainer.sampleType[iSample];
    if( sampleType == SampleType::DATA || sampleType == SampleType::FAKELEPTON )
    {
        sampleBranches.push_back("trigger"); 
        
        if( sampleType == SampleType::FAKELEPTON )
        {
            sampleBranches.push_back("fakeW2l2j"); 
            sampleBranches.push_back("fakeW2l2jstatUp");
            sampleBranches.push_back("fakeW2l2jstatDown");
        }
        
        if( triggerSelection )
            sampleBranches.push_back("std_vector_trigger");
    }
    else
    {
        sampleBranches.push_back("baseW");        
        sampleBranches.push_back("puW");
        hasNegWeight = false;
        
        if(  treeReader->fChain->FindBranch("GEN_weight_SM") )
        {
            sampleBranches.push_back("GEN_weight_SM");
            hasNegWeight = true;
        }
        
        sampleBranches.insert(sampleBranches.end(), genBranches.begin(), genBranches.end());
    }

    cout << "Reading out branches: \n";
    for( const string& branch : sampleBranches )
        cout << branch << endl;
    
    treeReader->setBranchStatus(sampleBranches);
    
    // Reset eventContainer
    treeReader->GetEntry(0);
    if( treeReader->fChain->GetBranchStatus("std_vector_jet_pt") )
        nJets = treeReader->std_vector_jet_pt->size();
    else 
        nJets = 0;
    if( treeReader->fChain->GetBranchStatus("std_vector_lepton_pt") )
        nLeptons = treeReader->std_vector_lepton_pt->size();
    else 
        nLeptons = 0;
    eventContainer.reset();
    eventContainer.init(nLeptons, nJets);
    
    // Set maxEventsWeight
//     TThread::Lock();
    if( (sampleType != SampleType::DATA && sampleType != SampleType::FAKELEPTON) && configContainer.maxEvents > 0 && configContainer.maxEvents < treeReader->fChain->GetEntries() )
    {
        maxEventsWeight = treeReader->fChain->GetEntries() / configContainer.maxEvents;
    }
    else 
        maxEventsWeight = 1.;
//     TThread::UnLock();
    return true;
}

bool EventReader::fillNextEvent()
{
    // Clear good object vectors
    eventContainer.goodLeptons.clear();
    eventContainer.goodJets.clear();
    eventContainer.goodGenLeptons.clear();
    eventContainer.goodGenJets.clear();
    eventContainer.goodPuppiJets.clear();
    eventContainer.goodTrackJets.clear();
    
    // Read event (trigger == 1 for data)
    bool skipEvent = false;
    do
    {
//         TThread::Lock();
        if( ((sampleType != SampleType::DATA && sampleType != SampleType::FAKELEPTON) && eventContainer.iEvent == configContainer.maxEvents) || ! treeReader->GetEntry(eventContainer.iEvent) ) {
//             TThread::UnLock();
            return false;
        }
//         TThread::UnLock();
        eventContainer.iEvent++;
        
        if( sampleType == SampleType::DATA || sampleType == SampleType::FAKELEPTON)
        {
            skipEvent = ( treeReader->trigger == 0 );
            if( triggerSelection ) 
            {
                skipEvent = true;
                unsigned int nTriggers = configContainer.cutContainer.triggerVector.size();
                for( unsigned int iTrig=0; iTrig < nTriggers; ++iTrig )
                {
                    bool passTrigger = ( (*treeReader->std_vector_trigger)[configContainer.cutContainer.triggerVector[iTrig]] && (*treeReader->std_vector_lepton_pt)[0] > configContainer.cutContainer.triggerPtVector[iTrig] );
                    if( passTrigger && ( iTrig+1 == nTriggers || (*treeReader->std_vector_lepton_pt)[0] < configContainer.cutContainer.triggerPtVector[iTrig+1] ) )
                    {
                        eventContainer.setWeight(1000. / configContainer.cutContainer.triggerLumiVector[iTrig]);
                        skipEvent = false;
                    }
                }
            }
        }
    }
    while( skipEvent ); // Cut on trigger for data
    
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
                eventContainer.jets[iJet].setCsv((*treeReader->std_vector_jet_cmvav2)[iJet]);
                eventContainer.goodJets.push_back(iJet);
            }
            else
                break; // don't need to clean contents because it won't be used
        }
    }
    
    // Fill track jets
    if( needTrackJets )
    {
        for( unsigned int iJet=0; iJet < 50; ++iJet ) {
            if( (*treeReader->std_vector_trackjet_pt)[iJet] >= 0. )
            {
                eventContainer.trackJets[iJet].set((*treeReader->std_vector_jet_pt)[iJet],(*treeReader->std_vector_jet_eta)[iJet],(*treeReader->std_vector_jet_phi)[iJet],0);
                eventContainer.goodTrackJets.push_back(iJet);
            }
        }
    }
    
    // Fill gen leptons
    if( needGenLeptons && (sampleType != SampleType::DATA && sampleType != SampleType::FAKELEPTON) )
    {
        for( unsigned int iLepton=0; iLepton < nLeptons; ++iLepton ) {
            if( /*(*treeReader->std_vector_leptonGen_status)[iLepton] == 1 &&*/ (*treeReader->std_vector_leptonGen_pt)[iLepton] > 0)
            {                
                eventContainer.goodGenLeptons.push_back(iLepton);
                eventContainer.genLeptons[iLepton].set((*treeReader->std_vector_leptonGen_pt)[iLepton],(*treeReader->std_vector_leptonGen_eta)[iLepton],(*treeReader->std_vector_leptonGen_phi)[iLepton],(*treeReader->std_vector_leptonGen_pid)[iLepton] );
//                 eventContainer.genLeptons[iLepton].setGenFlags( (*treeReader->std_vector_leptonGen_isPrompt)[iLepton]>0., (*treeReader->std_vector_leptonGen_isDirectPromptTauDecayProduct)[iLepton]>0. );
            }
        }
    }
    
    
    // Fill loose leptons
    if( needLooseLeptons )
    {
        for( unsigned int iLepton=0; iLepton < nLeptons; ++iLepton ) {
            if( (*treeReader->std_vector_lepton_pt)[iLepton] > 0. )
            {
                eventContainer.looseLeptons[iLepton].set((*treeReader->std_vector_lepton_pt)[iLepton],(*treeReader->std_vector_lepton_eta)[iLepton],(*treeReader->std_vector_lepton_phi)[iLepton],(*treeReader->std_vector_lepton_flavour)[iLepton]);
                
                eventContainer.goodLeptons.push_back(iLepton);
                eventContainer.leptons[iLepton].set((*treeReader->std_vector_lepton_pt)[iLepton],(*treeReader->std_vector_lepton_eta)[iLepton],(*treeReader->std_vector_lepton_phi)[iLepton],(*treeReader->std_vector_lepton_flavour)[iLepton], (*treeReader->std_vector_lepton_isTightLepton)[iLepton] == 1 );
            }
            else
                eventContainer.looseLeptons[iLepton].set(0,0,0,0);
        }
    } 
    else
    {
    // Fill leptons
        for( unsigned int iLepton=0; iLepton < nLeptons; ++iLepton ) {
            if( (*treeReader->std_vector_lepton_pt)[iLepton] >= configContainer.minLeptonPt )
            {
                eventContainer.goodLeptons.push_back(iLepton);
                
                if( abs((*treeReader->std_vector_lepton_flavour)[iLepton]) == 11 )
                {
                    eventContainer.leptons[iLepton].set((*treeReader->std_vector_lepton_pt)[iLepton],(*treeReader->std_vector_lepton_eta)[iLepton],(*treeReader->std_vector_lepton_phi)[iLepton],(*treeReader->std_vector_lepton_flavour)[iLepton]/*, (*treeReader->std_vector_lepton_eleIdTight)[iLepton] == 1*/ );
                    if( needElectronId )
                    {
                        eventContainer.leptons[iLepton].setd0((*treeReader->std_vector_lepton_d0)[iLepton]);
                        eventContainer.leptons[iLepton].setdEtaIn((*treeReader->std_vector_electron_dEtaIn)[iLepton]);
                        eventContainer.leptons[iLepton].setdPhiIn((*treeReader->std_vector_electron_dPhiIn)[iLepton]);
                        eventContainer.leptons[iLepton].setdz((*treeReader->std_vector_lepton_dz)[iLepton]);
                        eventContainer.leptons[iLepton].seteffectiveArea((*treeReader->std_vector_electron_effectiveArea)[iLepton]);
                        eventContainer.leptons[iLepton].setmissingHits((*treeReader->std_vector_electron_expectedMissingInnerHits)[iLepton]);
                        eventContainer.leptons[iLepton].setsigmaIetaIeta((*treeReader->std_vector_electron_full5x5_sigmaIetaIeta)[iLepton]);
                        eventContainer.leptons[iLepton].sethOverE((*treeReader->std_vector_electron_full5x5_sigmaIetaIeta)[iLepton]);
                        eventContainer.leptons[iLepton].setooEmoop((*treeReader->std_vector_electron_full5x5_sigmaIetaIeta)[iLepton]);
                        eventContainer.leptons[iLepton].setconversionVeto((*treeReader->std_vector_electron_passConversionVeto)[iLepton]);
                        eventContainer.leptons[iLepton].setscEta((*treeReader->std_vector_electron_scEta)[iLepton]);
    //                     eventContainer.leptons[iLepton].setIsolation( ((*treeReader->std_vector_lepton_chargedHadronIso)[iLepton] + TMath::Max((*treeReader->std_vector_lepton_neutralHadronIso)[iLepton] + (*treeReader->std_vector_lepton_photonIso)[iLepton] - (*treeReader->jetRho)*(*treeReader->std_vector_electron_effectiveArea)[iLepton])) / (*treeReader->std_vector_lepton_pt)[iLepton] );
                    }
                }
                else if( abs((*treeReader->std_vector_lepton_flavour)[iLepton]) == 13 )
                {
                    eventContainer.leptons[iLepton].set(
                        (*treeReader->std_vector_lepton_pt)[iLepton],
                        (*treeReader->std_vector_lepton_eta)[iLepton],
                        (*treeReader->std_vector_lepton_phi)[iLepton],
                        (*treeReader->std_vector_lepton_flavour)[iLepton]/*,
                        ((*treeReader->std_vector_lepton_isMediumMuon)[iLepton] == 1)*/ );
    //                 eventContainer.leptons[iLepton].setd0((*treeReader->std_vector_lepton_d0)[iLepton]);
    //                 eventContainer.leptons[iLepton].setdz((*treeReader->std_vector_lepton_dz)[iLepton]);
//                     eventContainer.leptons[iLepton].setIsolation( ((*treeReader->std_vector_lepton_chargedHadronIso)[iLepton] + TMath::Max((*treeReader->std_vector_lepton_neutralHadronIso)[iLepton] + (*treeReader->std_vector_lepton_photonIso)[iLepton] - 0.5 * (*treeReader->std_vector_lepton_sumPUPt)[iLepton], 0.)) / (*treeReader->std_vector_lepton_pt)[iLepton] );
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
    }
    
    // Fill MET 
    eventContainer.met.set( treeReader->metPfType1, treeReader->metPfType1Phi);
    
    // Fill number of vertices
    eventContainer.setNvertices( treeReader->nvtx );
    
    // Fill weight
    if( sampleType == SampleType::FAKELEPTON )
    {
        eventContainer.setWeight( treeReader->fakeW2l2j );
        eventContainer.setFakeWeights( treeReader->fakeW2l2jstatUp, treeReader->fakeW2l2jstatDown );
    }
    // For pu reweighting plot
//     if( sampleType == SampleType::SIGNAL )
//     {
//         eventContainer.setWeight( treeReader->baseW * (treeReader->GEN_weight_SM/abs(treeReader->GEN_weight_SM)) * maxEventsWeight);
//     }
    else if( sampleType != SampleType::DATA )
    {
        if( hasNegWeight )
            eventContainer.setWeight( treeReader->baseW * (treeReader->GEN_weight_SM/abs(treeReader->GEN_weight_SM)) * treeReader->puW * maxEventsWeight);
        else
            eventContainer.setWeight( treeReader->baseW * treeReader->puW * maxEventsWeight);
    }
    
    return true;
}

void EventReader::reweigh(unsigned int index)
{
    if( index < treeReader->std_vector_LHE_weight->size() )
    {
        eventContainer.setReweight( (*treeReader->std_vector_LHE_weight)[index] / (*treeReader->std_vector_LHE_weight)[0] );
    }
    else
    {
        cerr << "Reweigh index out of bounds: " << index << " while maximum index is " <<  treeReader->std_vector_LHE_weight->size()-1 << endl;
        throw 1;
    }
}