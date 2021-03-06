
#include "EventReader.hpp"
#include "TChain.h"

EventReader::EventReader(EventContainer& eventCont, const ConfigContainer& cfgContainer)
: eventContainer(eventCont), configContainer(cfgContainer), treeReader(nullptr), nLeptons(0), nJets(0), needJets(false), needGenJets(false), needPuppiJets(false), needGenLeptons(false), needLHELeptons(false), needElectronId(false), needTrackJets(false), needSoftMuons(false), needTaus(false), triggerSelection(false), hasNegWeight(false), isDY(false), applybPogSF(false), genMatching(false), sampleType(SampleType::DATA), maxEventsWeight(1.) 
{ 
    bool firstGenJet = true, firstPuppiJet = true, firstJet = true, firstgenLepton = true, firstLHELepton = true, firstElectronID = true, firstTrackJet = true, firstLooseLepton=true, firstSoftMuon = true, firstTau = true;
    vector<const vector<string>*> variableNames = {&configContainer.variableContainer.variableNames, &configContainer.cutContainer.variableNames};
    
    for( const auto* varNames : variableNames ) 
    {
        for( const string& iString : *varNames )
        {
            // Jets
            if( iString.find("genjet") != string::npos && firstGenJet )
            {
                needGenJets = true;
//                 needGenLeptons = true; // for cleaning jets
//                 genBranches.push_back("std_vector_jetGen_eta");
//                 genBranches.push_back("std_vector_jetGen_pt");
//                 genBranches.push_back("std_vector_jetGen_phi");
                genBranches.push_back("std_vector_LHEparton_pt");
                genBranches.push_back("std_vector_LHEparton_eta");
                genBranches.push_back("std_vector_LHEparton_phi");
//                 genBranches.push_back("std_vector_partonGen_isHardProcess");
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
                branches.push_back("std_vector_jet_csvv2ivf");
                branches.push_back("std_vector_jet_softMuPt");
                branches.push_back("std_vector_jet_softMuEta");
                branches.push_back("std_vector_jet_softMuPhi");
                branches.push_back("std_vector_jet_QGlikelihood");
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
            
            // Gen leptons
            if( iString.find("lhelepton") != string::npos && firstLHELepton )
            {
                needLHELeptons = true;
                genBranches.push_back("std_vector_LHElepton_eta");
                genBranches.push_back("std_vector_LHElepton_pt");
                genBranches.push_back("std_vector_LHElepton_phi");
                genBranches.push_back("std_vector_LHElepton_id");
                firstLHELepton = false;
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
                        
            else if( iString.find("softmu") != string::npos && firstSoftMuon )
            {
                needSoftMuons = true;
                branches.push_back("std_vector_softMuPt");
                branches.push_back("std_vector_softMuEta");
                branches.push_back("std_vector_softMuPhi");
                firstSoftMuon = false; 
            }
            
            else if( iString.find("tau") != string::npos && firstTau )
            {
                needTaus = true;
                branches.push_back("std_vector_tau_pt");
                branches.push_back("std_vector_tau_eta");
                branches.push_back("std_vector_tau_phi");
                branches.push_back("std_vector_tau_looseIso_dbeta");
                firstTau = false; 
            }
            
            // Loose lepton
            if( /*iString.find("loose") != string::npos &&*/ firstLooseLepton )
            {
                needLooseLeptons = true;
                branches.push_back("std_vector_electron_isTightLepton_cut_Tight80x_SS");
                branches.push_back("std_vector_muon_isTightLepton_cut_Tight80x");
//                 branches.push_back("std_vector_lepton_closejet_pt");
//                 branches.push_back("std_vector_lepton_closejet_PartonFlavour");
//                 branches.push_back("std_vector_lepton_closejet_drlj");
//                 branches.push_back("std_vector_lepton_trackIso");
                branches.push_back("std_vector_lepton_sumPUPt");
                branches.push_back("std_vector_lepton_chargedHadronIso");
                branches.push_back("std_vector_lepton_neutralHadronIso");
                branches.push_back("std_vector_lepton_photonIso");
                branches.push_back("jetRho");
                branches.push_back("std_vector_electron_effectiveArea");
                branches.push_back("std_vector_electron_tripleChargeAgreement");
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
    
    // b SF
    for( const string& iString : configContainer.cutContainer.variableNames )
    {
        if( iString.find("bjetveto") != string::npos || iString.find("bveto") != string::npos || iString.find("btag") != string::npos || iString.find("bjettag") != string::npos )
            applybPogSF = true;
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
        cerr << "Indices for sample or subsample out of range in the EventReader::setSample function." << endl;
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
//     findTrees(iSample, iSubSample, t);
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
                    cout << "File matches Regex: " << inDir+fname << endl;
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
    vector<string> sampleBranches= {"std_vector_lepton_eta","std_vector_lepton_pt","std_vector_lepton_phi","std_vector_lepton_flavour", "metPfType1","metPfType1Phi","nvtx","dmZll_vetoLep","dmZllReco" /*,"std_vector_lepton_idisoW","effTrigW"*/,"veto_EMTFBug","std_vector_trigger_special"};
    sampleBranches.insert(sampleBranches.end(), branches.begin(), branches.end());
    
    // Set data/MC weight branches       
    sampleType = configContainer.sampleContainer.sampleType[iSample];
    if( sampleType == SampleType::DATA || sampleType == SampleType::FAKELEPTON )
    {
//         sampleBranches.push_back("trigger"); 
        genMatching = false;
        
        if( sampleType == SampleType::FAKELEPTON )
        {
            sampleBranches.push_back("fakeW2l2j"); 
            sampleBranches.push_back("fakeW2l2jstatMuUp");
            sampleBranches.push_back("fakeW2l2jstatMuDown");
        }
        
//         if( triggerSelection )
        sampleBranches.push_back("std_vector_trigger");
        sampleBranches.push_back("trig_DbleEle");
        sampleBranches.push_back("trig_EleMu");
        sampleBranches.push_back("trig_SnglMu");
        sampleBranches.push_back("trig_DbleMu");
        sampleBranches.push_back("trig_SnglEle");
        
        // Debug eventNo info
        sampleBranches.push_back("event"); 
        
        // Data type  iString.find("genjet") != string::npo
        if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("MuonEG") != string::npos )
            dataType = DataType::MuonEG;
        else if (configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("DoubleMuon") != string::npos )
            dataType = DataType::DoubleMuon;
        else if (configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("SingleMuon") != string::npos )
            dataType = DataType::SingleMuon;
        else if (configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("DoubleEG") != string::npos )
            dataType = DataType::DoubleEG;
        else if (configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("SingleElectron") != string::npos )
            dataType = DataType::SingleElectron;
    }
    else
    {
        sampleBranches.push_back("baseW");        
        sampleBranches.push_back("puW");
        sampleBranches.push_back("std_vector_lepton_idisoWcut_WP_Tight80X"); 
        sampleBranches.push_back("std_vector_lepton_recoW");
        sampleBranches.push_back("std_vector_lepton_promptgenmatched");
        hasNegWeight = false;
        
        if(  treeReader->fChain->FindBranch("GEN_weight_SM") )
        {
            sampleBranches.push_back("GEN_weight_SM");
            hasNegWeight = true;
        }
        
        if( !triggerSelection ) 
        {
            sampleBranches.push_back("effTrigW");
//             sampleBranches.push_back("effTrigW_DbleEle");
//             sampleBranches.push_back("effTrigW_DbleMu");
//             sampleBranches.push_back("effTrigW_EleMu");
//             sampleBranches.push_back("bPogSF");
        }
        
        if( applybPogSF )
            sampleBranches.push_back("bPogSF_CSVM");
        
        if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("WW_DoubleScattering") == string::npos )
            sampleBranches.insert(sampleBranches.end(), genBranches.begin(), genBranches.end());
        
        if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("WGJJ") == string::npos && configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("Zg") == string::npos )
            genMatching = true;
        else
            genMatching = false;
    }
    
//     if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("DY") != string::npos && configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("M-10") == string::npos )
//     {
//         isDY = true;
//         sampleBranches.push_back("std_vector_LHEparton_pt");
//         sampleBranches.push_back("std_vector_LHEparton_eta");
//         sampleBranches.push_back("std_vector_LHEparton_phi");
//     }
//     else
//         isDY = false;

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
    eventContainer.goodLHELeptons.clear();
    eventContainer.goodGenJets.clear();
    eventContainer.goodPuppiJets.clear();
    eventContainer.goodTrackJets.clear();
    eventContainer.goodSoftMuons.clear();
    eventContainer.goodTaus.clear();
    
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
            if( triggerSelection ) 
            {
                skipEvent = true;
                unsigned int nTriggers = configContainer.cutContainer.triggerVector.size();
                for( unsigned int iTrig=0; iTrig < nTriggers; ++iTrig )
                {
                    bool passTrigger = ( (*treeReader->std_vector_trigger)[configContainer.cutContainer.triggerVector[iTrig]] == 1 && (*treeReader->std_vector_lepton_pt)[0] > configContainer.cutContainer.triggerPtVector[iTrig] );
                    if( passTrigger && ( iTrig+1 == nTriggers || (*treeReader->std_vector_lepton_pt)[0] < configContainer.cutContainer.triggerPtVector[iTrig+1] ) )
                    {
                        eventContainer.setWeight(1000. / configContainer.cutContainer.triggerLumiVector[iTrig]);
                        skipEvent = false;
                    }
                }
            }
            else
            {
//                skipEvent = ( treeReader->trigger != 1 );
//                 skipEvent = ( /*treeReader->trigger != 1 ||*/ ((*treeReader->std_vector_trigger)[5] != 1 &&  (*treeReader->std_vector_trigger)[10] != 1 && (*treeReader->std_vector_trigger)[12] != 1 && (*treeReader->std_vector_trigger)[7] != 1 && (*treeReader->std_vector_trigger)[9] != 1) );
//                 bool passMuonEG = (*treeReader->std_vector_trigger)[6] == 1 || (*treeReader->std_vector_trigger)[8] == 1 || 
//                             (*treeReader->std_vector_trigger)[41] == 1 || (*treeReader->std_vector_trigger)[57] == 1 || (*treeReader->std_vector_trigger)[58] == 1/* || (*treeReader->std_vector_trigger)[96] == 1 || (*treeReader->std_vector_trigger)[97] == 1 || (*treeReader->std_vector_trigger)[98] == 1*/;
//                 bool passDoubleMuon = (*treeReader->std_vector_trigger)[11] == 1 || (*treeReader->std_vector_trigger)[13] == 1 /*|| (*treeReader->std_vector_trigger)[100] == 1*/;
//                 bool passSingleMuon = (*treeReader->std_vector_trigger)[42] == 1 || (*treeReader->std_vector_trigger)[43] == 1 
//                                        || (*treeReader->std_vector_trigger)[44] == 1 ||         (*treeReader->std_vector_trigger)[45] == 1;
//                 bool passDoubleEG = (*treeReader->std_vector_trigger)[46] == 1;
//                 bool passSingleElectron = (*treeReader->std_vector_trigger)[93] == 1;
                bool passMuonEG = treeReader->trig_EleMu;
                bool passDoubleMuon = treeReader->trig_DbleMu;
                bool passSingleMuon = treeReader->trig_SnglMu;
                bool passDoubleEG = treeReader->trig_DbleEle;
                bool passSingleElectron = treeReader->trig_SnglEle;
                
                bool keepEvent = false;
                if( dataType == DataType::MuonEG ) {
                    if( passMuonEG) keepEvent = true;
                }
                else if( dataType == DataType::DoubleMuon ) {
                    if( passDoubleMuon && !passMuonEG ) keepEvent = true;
                }
                else if( dataType == DataType::DoubleEG) {
                    if( passDoubleEG && !passMuonEG && !passDoubleMuon ) keepEvent = true;   
                }
                else if( dataType == DataType::SingleMuon ) {
                    if( passSingleMuon && !passMuonEG && !passDoubleMuon && !passDoubleEG ) keepEvent = true;
                }
                else if( dataType == DataType::SingleElectron ) {
                    if( passSingleElectron && !passMuonEG && !passDoubleMuon && !passDoubleEG && !passSingleMuon ) keepEvent = true;
                }

                skipEvent = !keepEvent;
            }
            
            if( (*treeReader->std_vector_trigger_special)[0]*(*treeReader->std_vector_trigger_special)[1]*(*treeReader->std_vector_trigger_special)[2]*(*treeReader->std_vector_trigger_special)[3]*(*treeReader->std_vector_trigger_special)[5]        
                * ((*treeReader->std_vector_trigger_special)[4]*!(*treeReader->std_vector_trigger_special)[6]*!(*treeReader->std_vector_trigger_special)[7]*(*treeReader->std_vector_trigger_special)[8]*(*treeReader->std_vector_trigger_special)[9]) == 0 )
                skipEvent = true;
            
        }
        else 
        {
            if( genMatching && !triggerSelection && ((*treeReader->std_vector_lepton_promptgenmatched)[0] != 1 || (*treeReader->std_vector_lepton_promptgenmatched)[1] != 1) )
                skipEvent = true;
            else
                skipEvent = false;
            
            if( (*treeReader->std_vector_trigger_special)[0]*(*treeReader->std_vector_trigger_special)[1]*(*treeReader->std_vector_trigger_special)[2]*(*treeReader->std_vector_trigger_special)[3]*(*treeReader->std_vector_trigger_special)[5]        
                * (((*treeReader->std_vector_trigger_special)[8]==-2. * ((*treeReader->std_vector_trigger_special)[6]*(*treeReader->std_vector_trigger_special)[7])) || (! ((*treeReader->std_vector_trigger_special)[8]==-2.)*((*treeReader->std_vector_trigger_special)[8]*(*treeReader->std_vector_trigger_special)[9])) ) == 0 )
                skipEvent = true;
                
        }
    }
    while( skipEvent ); // Cut on trigger for data and non-genmatched leptons
    
    // Fill gen jets
    if( (needGenJets || isDY ) && sampleType != SampleType::DATA && sampleType != SampleType::FAKELEPTON )
    {
        for( unsigned int iJet=0; iJet < nJets; ++iJet ) {
            if( (*treeReader->std_vector_LHEparton_pt)[iJet] > 0 )
            {
                eventContainer.genJets[iJet].set((*treeReader->std_vector_LHEparton_pt)[iJet],(*treeReader->std_vector_LHEparton_eta)[iJet],(*treeReader->std_vector_LHEparton_phi)[iJet], 0);
                eventContainer.genJets[iJet].setIsHardProcess(1);
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
                if( (*treeReader->std_vector_jet_softMuPt)[iJet] > 0 )
                    eventContainer.jets[iJet].setSoftMu((*treeReader->std_vector_jet_softMuPt)[iJet], (*treeReader->std_vector_jet_softMuEta)[iJet], (*treeReader->std_vector_jet_softMuPhi)[iJet]);
                else
                    eventContainer.jets[iJet].setSoftMu(0,0,0);
                eventContainer.jets[iJet].setQgLikelihood((*treeReader->std_vector_jet_QGlikelihood)[iJet]);
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
                eventContainer.trackJets[iJet].set((*treeReader->std_vector_trackjet_pt)[iJet],(*treeReader->std_vector_trackjet_eta)[iJet],(*treeReader->std_vector_trackjet_phi)[iJet],0);
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
    
        // Fill gen leptons
    if( needLHELeptons && (sampleType != SampleType::DATA && sampleType != SampleType::FAKELEPTON) )
    {
        for( unsigned int iLepton=0; iLepton < nLeptons; ++iLepton ) {
            if( (*treeReader->std_vector_LHElepton_pt)[iLepton] > 0)
            {                
                eventContainer.goodLHELeptons.push_back(iLepton);
                eventContainer.lheLeptons[iLepton].set((*treeReader->std_vector_LHElepton_pt)[iLepton],(*treeReader->std_vector_LHElepton_eta)[iLepton],(*treeReader->std_vector_LHElepton_phi)[iLepton],(*treeReader->std_vector_LHElepton_id)[iLepton] );
            }
        }
    }
    
    if( needTaus )
    {
        for( unsigned int iLepton=0; iLepton < nLeptons; ++iLepton ) {
            if( (*treeReader->std_vector_tau_pt)[iLepton] > 0 && (*treeReader->std_vector_tau_looseIso_dbeta)[iLepton] )
            {                
                eventContainer.goodTaus.push_back(iLepton);
                eventContainer.taus[iLepton].set((*treeReader->std_vector_tau_pt)[iLepton],(*treeReader->std_vector_tau_eta)[iLepton],(*treeReader->std_vector_tau_phi)[iLepton], 15 );
            }
        }
    }
    
    if( needSoftMuons )
    {
        for( unsigned int iLepton=0; iLepton < nLeptons; ++iLepton ) {
            if( (*treeReader->std_vector_softMuPt)[iLepton] > 0)
            {                
                eventContainer.goodSoftMuons.push_back(iLepton);
                eventContainer.softMuons[iLepton].set((*treeReader->std_vector_softMuPt)[iLepton],(*treeReader->std_vector_softMuEta)[iLepton],(*treeReader->std_vector_softMuPhi)[iLepton], 13 );
            }
        }
    }
    
    // Fill loose leptons
    if( needLooseLeptons )
    {
        for( unsigned int iLepton=0; iLepton < nLeptons; ++iLepton ) {
            if( (*treeReader->std_vector_lepton_pt)[iLepton] > 0. )
            {
                eventContainer.looseLeptons[iLepton].set((*treeReader->std_vector_lepton_pt)[iLepton],(*treeReader->std_vector_lepton_eta)[iLepton],(*treeReader->std_vector_lepton_phi)[iLepton],(*treeReader->std_vector_lepton_flavour)[iLepton], ((*treeReader->std_vector_muon_isTightLepton_cut_Tight80x)[iLepton] || (*treeReader->std_vector_electron_isTightLepton_cut_Tight80x_SS)[iLepton]) );
//                 eventContainer.looseLeptons[iLepton].setClosestJetPt((*treeReader->std_vector_lepton_closejet_pt)[iLepton]);
//                 eventContainer.looseLeptons[iLepton].setClosestJetDr((*treeReader->std_vector_lepton_closejet_drlj)[iLepton]);
//                 eventContainer.looseLeptons[iLepton].setClosestJetPartonFlavour((*treeReader->std_vector_lepton_closejet_PartonFlavour)[iLepton]);
//                 eventContainer.looseLeptons[iLepton].setTrackIso((*treeReader->std_vector_lepton_trackIso)[iLepton]/(*treeReader->std_vector_lepton_pt)[iLepton]);
                eventContainer.looseLeptons[iLepton].setTripleChargeAgreement((*treeReader->std_vector_electron_tripleChargeAgreement)[iLepton]);
                
                eventContainer.goodLeptons.push_back(iLepton);
                eventContainer.leptons[iLepton].set((*treeReader->std_vector_lepton_pt)[iLepton],(*treeReader->std_vector_lepton_eta)[iLepton],(*treeReader->std_vector_lepton_phi)[iLepton],(*treeReader->std_vector_lepton_flavour)[iLepton], ((*treeReader->std_vector_muon_isTightLepton_cut_Tight80x)[iLepton] || (*treeReader->std_vector_electron_isTightLepton_cut_Tight80x_SS)[iLepton]) );
//                 eventContainer.leptons[iLepton].setClosestJetPt((*treeReader->std_vector_lepton_closejet_pt)[iLepton]);
//                 eventContainer.leptons[iLepton].setClosestJetDr((*treeReader->std_vector_lepton_closejet_drlj)[iLepton]);
//                 eventContainer.leptons[iLepton].setClosestJetPartonFlavour((*treeReader->std_vector_lepton_closejet_PartonFlavour)[iLepton]);
                eventContainer.leptons[iLepton].setTripleChargeAgreement((*treeReader->std_vector_electron_tripleChargeAgreement)[iLepton]);
//                 
                if( abs((*treeReader->std_vector_lepton_flavour)[iLepton]) == 11 )
                {
                    float iso = ((*treeReader->std_vector_lepton_chargedHadronIso)[iLepton] + TMath::Max((*treeReader->std_vector_lepton_neutralHadronIso)[iLepton] + (*treeReader->std_vector_lepton_photonIso)[iLepton] - (treeReader->jetRho * (*treeReader->std_vector_electron_effectiveArea)[iLepton]), (float) 0.) ) / (*treeReader->std_vector_lepton_pt)[iLepton];
                    eventContainer.leptons[iLepton].setIsolation(iso);
                    eventContainer.looseLeptons[iLepton].setIsolation(iso);
                }
                else if( abs((*treeReader->std_vector_lepton_flavour)[iLepton]) == 13 )
                {
                    float iso = ((*treeReader->std_vector_lepton_chargedHadronIso)[iLepton] + TMath::Max((*treeReader->std_vector_lepton_neutralHadronIso)[iLepton] + (*treeReader->std_vector_lepton_photonIso)[iLepton] - 0.5 * (*treeReader->std_vector_lepton_sumPUPt)[iLepton], 0.)) / (*treeReader->std_vector_lepton_pt)[iLepton];
                    eventContainer.leptons[iLepton].setIsolation(iso);
                    eventContainer.looseLeptons[iLepton].setIsolation(iso);
                }
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
                        eventContainer.leptons[iLepton].sethOverE((*treeReader->std_vector_electron_hOverE)[iLepton]);
                        eventContainer.leptons[iLepton].setooEmoop((*treeReader->std_vector_electron_ooEmooP)[iLepton]);
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
    
    // Fill new Veto's
//     eventContainer.setTauVeto( (*treeReader->std_vector_tau_looseIso_dbeta)[0]>0. );
//     eventContainer.setLooseMuonVeto((*treeReader->std_vector_softMuPt)[0]>0. );
    eventContainer.setZveto(treeReader->dmZllReco < 15.);
    eventContainer.setZvetoMuon(treeReader->dmZll_vetoLep < 15.);
    eventContainer.setPassEMTF(treeReader->veto_EMTFBug);
    
    // Fill weight
    if( sampleType == SampleType::FAKELEPTON )
    {
        eventContainer.setWeight( treeReader->fakeW2l2j );
        eventContainer.setFakeWeights( treeReader->fakeW2l2jstatMuUp, treeReader->fakeW2l2jstatMuDown );
        
        // Debug eventNo info
        eventContainer.setEventNo( treeReader->event );
    }
    
    // For pu reweighting plot
//     if( sampleType == SampleType::SIGNAL )
//     {
//         eventContainer.setWeight( treeReader->baseW * (treeReader->GEN_weight_SM/abs(treeReader->GEN_weight_SM)) * maxEventsWeight);
//     }
    else if( sampleType != SampleType::DATA )
    {
        float weight = treeReader->puW * maxEventsWeight;
//         if( ! isDY ) 
        weight *= treeReader->baseW;
        if( !triggerSelection ) 
            weight *= ( treeReader->effTrigW
                        * (*treeReader->std_vector_lepton_idisoWcut_WP_Tight80X)[0] * (*treeReader->std_vector_lepton_idisoWcut_WP_Tight80X)[1]
                        * (*treeReader->std_vector_lepton_recoW)[0] * (*treeReader->std_vector_lepton_recoW)[1]);
        else
            weight *= (*treeReader->std_vector_lepton_idisoWcut_WP_Tight80X)[0] * (*treeReader->std_vector_lepton_recoW)[0];
        if( applybPogSF )
            weight *= treeReader->bPogSF_CSVM; 
        if( hasNegWeight ) 
            weight *= (treeReader->GEN_weight_SM/abs(treeReader->GEN_weight_SM));
        
        eventContainer.setWeight( weight );
    }
    else
    {
        // Debug eventNo info
        eventContainer.setEventNo( treeReader->event );
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

void EventReader::getDYWeights(vector<float>& DYweights) const
{
    DYweights.clear();
    DYweights.resize(5, 0.);
    float NLOcor = 1.21623; // NLO/LO corr factor 
    cout << "Recalculating DY baseW" << endl; 
    
    for( unsigned int iSample = 0; iSample < configContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        for( unsigned int iSubSample = 0; iSubSample < configContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {
            if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("DY") != string::npos )
            {
                TTree *t;
                cout << "iSample: " << iSample << ", iSubSample: " << iSubSample << endl;
                
//                 findTrees(iSample, iSubSample, t);
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
                                cout << "File matches Regex: " << inDir+fname << endl;
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
                
                TreeReader *tReader = new TreeReader(t);
                tReader->GetEntry(1);
                float invBaseW = pow(tReader->baseW,-1);
                delete tReader;
//                 delete t; // already deleted if this is from a tree (not for tchain)
                
                if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("DY1JetsToLL") != string::npos )
                    DYweights[1] += invBaseW / NLOcor;
                else if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("DY2JetsToLL") != string::npos )
                    DYweights[2] += invBaseW / NLOcor;
                else if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("DY3JetsToLL") != string::npos )
                    DYweights[3] += invBaseW / NLOcor;
                else if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("DY4JetsToLL") != string::npos )
                    DYweights[4] += invBaseW / NLOcor;
                else if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("DYJetsToLL_M-50") != string::npos )
                {
                    if( configContainer.sampleContainer.sampleNames[iSample][iSubSample].find("-LO") != string::npos )
                        invBaseW /= NLOcor;
                    DYweights[0] += invBaseW;
                    DYweights[1] += invBaseW;
                    DYweights[2] += invBaseW;
                    DYweights[3] += invBaseW;
                    DYweights[4] += invBaseW;
                }
            }
        }
    }
    
    // Convert to real baseW 
    for( unsigned int iWeight = 0; iWeight < 5; ++iWeight )
    {
        DYweights[iWeight] = pow( DYweights[iWeight], -1);
    }    
}

void EventReader::findTrees(unsigned int iSample, unsigned int iSubSample, TTree *t) const
{
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
    t->SetDirectory(0);
    t->ls();
}
