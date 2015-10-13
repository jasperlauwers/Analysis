#include "TreeReader.hpp"
#include <iostream>

TreeReader::TreeReader(TTree *tree)
:latino(tree) 
{
   if (tree == 0) {
        std::cerr << "Tree does not exist!" << std::endl; 
        throw 1;
   }
}

TreeReader::~TreeReader() { } // done in base class

void TreeReader::setBranchStatus( const vector<string>& branchNames )
{
   // Disable all branches
   fChain->SetBranchStatus("*",0);
   
   // Enable required branches
   for( const string& iBranch : branchNames )
   {
       fChain->SetBranchStatus(iBranch.c_str(), 1);
   }
}
