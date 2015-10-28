#include "LatinoStyle2.h"

void LatinoStyle2()
{
  TStyle* GloStyle;
  GloStyle = gStyle;
  (void)GloStyle;

  TStyle* LatinosStyle = new TStyle("LatinosStyle", "LatinosStyle");
  gStyle = LatinosStyle;


  //----------------------------------------------------------------------------
  // Canvas
  //----------------------------------------------------------------------------
  LatinosStyle->SetCanvasBorderMode(  0);
  LatinosStyle->SetCanvasBorderSize( 10);
  LatinosStyle->SetCanvasColor     (  0);
  LatinosStyle->SetCanvasDefH      (794);
  LatinosStyle->SetCanvasDefW      (550);
  LatinosStyle->SetCanvasDefX      ( 10);
  LatinosStyle->SetCanvasDefY      ( 10);


  //----------------------------------------------------------------------------
  // Pad
  //----------------------------------------------------------------------------
  LatinosStyle->SetPadBorderMode  (   0);
  LatinosStyle->SetPadBorderSize  (  10);
  LatinosStyle->SetPadColor       (   0);
  LatinosStyle->SetPadBottomMargin(0.20);
  LatinosStyle->SetPadTopMargin   (0.08);
  LatinosStyle->SetPadLeftMargin  (0.18);
  LatinosStyle->SetPadRightMargin (0.05);


  //----------------------------------------------------------------------------
  // Frame
  //----------------------------------------------------------------------------
  LatinosStyle->SetFrameFillStyle ( 0);
  LatinosStyle->SetFrameFillColor ( 0);
  LatinosStyle->SetFrameLineColor ( 1);
  LatinosStyle->SetFrameLineStyle ( 0);
  LatinosStyle->SetFrameLineWidth ( 2);
  LatinosStyle->SetFrameBorderMode( 0);
  LatinosStyle->SetFrameBorderSize(10);


  //----------------------------------------------------------------------------
  // Hist
  //----------------------------------------------------------------------------
  LatinosStyle->SetHistFillColor(0);
  LatinosStyle->SetHistFillStyle(1);
  LatinosStyle->SetHistLineColor(1);
  LatinosStyle->SetHistLineStyle(0);
  LatinosStyle->SetHistLineWidth(1);
  
  //added
  LatinosStyle->SetHatchesLineWidth(2);

  //----------------------------------------------------------------------------
  // Axis
  //----------------------------------------------------------------------------
  LatinosStyle->SetLabelFont  (   42, "xyz");
  LatinosStyle->SetLabelOffset(0.015, "xyz");
  LatinosStyle->SetLabelSize  (0.050, "xyz");
  LatinosStyle->SetNdivisions (  505, "xyz");
  LatinosStyle->SetTitleFont  (   42, "xyz");
  LatinosStyle->SetTitleSize  (0.050, "xyz");

  //  LatinosStyle->SetNdivisions ( -503, "y");

//   LatinosStyle->SetTitleOffset(  1.4,   "x");
//   LatinosStyle->SetTitleOffset(  1.2,   "y");
  
  // changed
  LatinosStyle->SetTitleOffset(1.25, "x");
  
  LatinosStyle->SetPadTickX   (           1);  // Tick marks on the opposite side of the frame
  LatinosStyle->SetPadTickY   (           1);  // Tick marks on the opposite side of the frame

  // Values from UATAnaDisplay overwrites
//   LatinosStyle->SetTitleOffset(1.1, "x");
  LatinosStyle->SetTitleOffset(1.5, "y");
//   LatinosStyle->SetTitleSize(0.05, "xy");
//   LatinosStyle->SetLabelSize(0.05, "xy");
//   LatinosStyle->SetLabelOffset(0.01, "xy");
  
  //----------------------------------------------------------------------------
  // Title
  //----------------------------------------------------------------------------
  LatinosStyle->SetTitleBorderSize(    0);
  LatinosStyle->SetTitleFillColor (   10);
  LatinosStyle->SetTitleAlign     (   12);
  LatinosStyle->SetTitleFontSize  (0.045);
  LatinosStyle->SetTitleX         (0.590);
  LatinosStyle->SetTitleY         (0.830);

  LatinosStyle->SetTitleFont(42, "");
  
  //added
  LatinosStyle->SetOptTitle(0); 


  //----------------------------------------------------------------------------
  // Stat
  //----------------------------------------------------------------------------
//   LatinosStyle->SetOptStat       (1110);
  //changed
  LatinosStyle->SetOptStat       (   0);
  
  LatinosStyle->SetStatBorderSize(   0);
  LatinosStyle->SetStatColor     (  10);
  LatinosStyle->SetStatFont      (  42);
  LatinosStyle->SetStatX         (0.94);
  LatinosStyle->SetStatY         (0.91);

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  LatinosStyle->SetNumberContours(NCont);

//  return();
}