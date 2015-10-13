//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Sep 14 15:48:37 2015 by ROOT version 5.34/18
// from TTree probe_tree/probe_tree
// found on file: testTree.root
//////////////////////////////////////////////////////////

#ifndef TreeReader_hpp
#define TreeReader_hpp

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <Math/GenVector/LorentzVector.h>
#include <Math/GenVector/PxPyPzE4D.h>

#include "latino.h"

using namespace std;

// Fixed size dimensions of array or collections stored in the TTree if any.

class TreeReader : public latino {
public :
   TreeReader(TTree *tree=0);
   virtual ~TreeReader();
   void setBranchStatus( const vector<string>& branchNames );
};

#endif
