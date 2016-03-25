
#include "HistogramContainer.hpp"

HistogramContainer::HistogramContainer(string name) :
containerName(name) { }

HistogramContainer::HistogramContainer(string name, unsigned int nTotal) : containerName(name) 
{ 
    histograms.reserve(nTotal);
    reducedNames.reserve(nTotal);
    color.reserve(nTotal);
    sampleType.reserve(nTotal);
}

HistogramContainer::~HistogramContainer() { }

void HistogramContainer::add(TH1* h, string histName, int color_, SampleType sampleType_)
{
    histograms.push_back(h);
    reducedNames.push_back(histName);
    color.push_back(color_);
    sampleType.push_back(sampleType_);
}

void HistogramContainer::add(const HistogramContainer& histContainer, int index)
{
    histograms.push_back(histContainer.histograms[index]);
    reducedNames.push_back(histContainer.reducedNames[index]);
    color.push_back(histContainer.color[index]);
    sampleType.push_back(histContainer.sampleType[index]);
}

void HistogramContainer::pop_back()
{
    histograms.pop_back();
    reducedNames.pop_back();
    color.pop_back();
    sampleType.pop_back();    
}

bool HistogramContainer::check() const
{
    unsigned int size = histograms.size();
    if( reducedNames.size() == size && color.size() == size && sampleType.size() == size )
        return true;
    else 
    {
        cerr << "Inconsistency found in histogramContainer \"" <<  containerName << "\", vectors don't have the same lenght." << endl;
        return false;
    }
}

void HistogramContainer::addOverflow()
{
    for( unsigned int iHist = 0; iHist < histograms.size(); ++iHist )
    {
        histograms[iHist]->SetBinContent(histograms[iHist]->GetNbinsX(),histograms[iHist]->GetBinContent(histograms[iHist]->GetNbinsX())+histograms[iHist]->GetBinContent(histograms[iHist]->GetNbinsX()+1)); 
        histograms[iHist]->SetBinError(histograms[iHist]->GetNbinsX(),sqrt(pow(histograms[iHist]->GetBinError(histograms[iHist]->GetNbinsX()),2)+pow(histograms[iHist]->GetBinError(histograms[iHist]->GetNbinsX()+1),2))); 
        histograms[iHist]->SetBinContent(histograms[iHist]->GetNbinsX()+1,0);
        histograms[iHist]->SetBinError(histograms[iHist]->GetNbinsX()+1,0);
        
        // need to do this for every row in 2D histogram
//         if( histograms[iHist]->GetDimension() == 2 )
//         {
//             histograms[iHist]->SetBinContent(histograms[iHist]->GetNbinsY(),histograms[iHist]->GetBinContent(histograms[iHist]->GetNbinsY())+histograms[iHist]->GetBinContent(histograms[iHist]->GetNbinsY()+1)); 
//             histograms[iHist]->SetBinError(histograms[iHist]->GetNbinsY(),sqrt(pow(histograms[iHist]->GetBinError(histograms[iHist]->GetNbinsY()),2)+pow(histograms[iHist]->GetBinError(histograms[iHist]->GetNbinsY()+1),2))); 
//             histograms[iHist]->SetBinContent(histograms[iHist]->GetNbinsY()+1,0);
//             histograms[iHist]->SetBinError(histograms[iHist]->GetNbinsY()+1,0); 
//         }
    }
}

void HistogramContainer::addUnderflow()
{
    for( unsigned int iHist = 0; iHist < histograms.size(); ++iHist )
    {
        histograms[iHist]->SetBinContent(1,histograms[iHist]->GetBinContent(1)+histograms[iHist]->GetBinContent(0)); 
        histograms[iHist]->SetBinError(1,sqrt(pow(histograms[iHist]->GetBinError(1),2)+pow(histograms[iHist]->GetBinError(0),2))); 
        histograms[iHist]->SetBinContent(0,0);
        histograms[iHist]->SetBinError(0,0);
    }
}
