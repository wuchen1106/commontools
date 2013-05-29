//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A class for automatically detect objects in root files and automatically match them between reference files and
//new sample root files,finally perform comparison by using Ks Test or Chi2-test.
//
//input : package name
//        reference histogram file
//        new sample histogram file
//
//output:Pv distribution
//       comparison plots
//
//Created by Xingtao Huang <huangxt@sdu.edu.cn>
//Date: 2007-07-27
//Modified By Xingtao Huang
//Date: 2007-08-20
//Re-Design: combination of root macros inside this class in order to  support different root files.
//Data: 2008-07-30
//Re-Design: recursive searching
//Date: 2011-11-11
//Modified By: Wu Chen <wuchen@ihep.ac.cn>
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <strstream>
#include <vector>
#include <iomanip>
#include "TFile.h"
#include "TH1F.h"
#include "TH2.h"
#include "TProfile.h"
#include "TText.h"
#include "TList.h"
#include "TDirectory.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TKey.h"
#include "TFolder.h"
#include "TObject.h"
#include "TCollection.h"
#include "TTree.h"
#include "TStyle.h"

class AutoHistoCompare {

  public:

    AutoHistoCompare( TH1F* h1 , const char* oldfn,  const char* newfn, int method_AHC = 2 );
    ~AutoHistoCompare();

    void Execute();

  private:

    void CompareHist_1D( TH1* hist_old_CH, TH1* hist_new_CH );

    void CompareHist_2D( TH2* hist_old_CH, TH2* hist_new_CH );

    void SearchAndCompare( TObject* obj_old_DSAC, TObject* obj_new_DSAC );

    TFile*  file_old;
    TFile*  file_new;

    Double_t mypv;

    TText * myte;
    TH1F*   PvHisto; 

    TText* te;
    TLegend* leg; 

    int method;
    int levelNo;

};

AutoHistoCompare:: AutoHistoCompare( TH1F* h1 , const char* oldfn,  const char* newfn, int method_AHC ):
  PvHisto(h1),levelNo(0),method(method_AHC){

    file_old = new TFile(oldfn);
    if ( file_old->IsZombie() ){
      std::cout<<"Cannot create file_old !!! "<<std::endl;
      std::cout<<"Something is wrong with file :"<<oldfn<<std::endl;
      exit(EXIT_FAILURE);
    }
    file_new = new TFile(newfn);
    if ( file_new->IsZombie() ){
      std::cout<<"Cannot create file_new !!! "<<std::endl;
      std::cout<<"Something is wrong with file :"<<newfn<<std::endl;
      exit(EXIT_FAILURE);
    }

    te = new TText();
    te->SetTextSize(0.06);

    leg = new TLegend(0.6, 0.92, 0.75, 0.99);

    std::cout<<std::endl;
    std::cout<<"Using method "<<method<<std::endl;
    std::cout<<"old file is: "<<std::endl;
    file_old->Print();
    std::cout<<"new file is: "<<std::endl;
    file_new->Print();

  } 

AutoHistoCompare::~AutoHistoCompare(){
  delete file_old;
  delete file_new;
  delete te;
  delete leg;
}

void AutoHistoCompare::Execute(){
  TObject* obj_old = (TObject*) file_old;
  TObject* obj_new = (TObject*) file_new;
  SearchAndCompare( obj_old, obj_new );
}

void AutoHistoCompare::SearchAndCompare( TObject* obj_old_DSAC, TObject* obj_new_DSAC ){
  std::cout<<std::endl;
  std::cout<<"******************************************************************************"<<std::endl;
  std::cout<<setw( levelNo * 5 + 2 )<<"|_";
  std::cout<<"This is level "<<levelNo<<std::endl;
  std::cout<<setw( levelNo * 5 + 2 )<<"|_";
  obj_old_DSAC->Print();

  if ( obj_old_DSAC->InheritsFrom("TH1") ){
    std::cout<<setw( levelNo * 5 + 2 )<<"|_";
    std::cout<<"This obj inherits from TH1"<<std::endl;
    TH1* hist_old = dynamic_cast< TH1* > ( obj_old_DSAC );
    if ( hist_old == NULL ){
      cout<<"Cannot convert obj_old_DSAC to TH1* "<<std::endl;
      exit(EXIT_FAILURE);
    }
    TH1* hist_new = dynamic_cast< TH1* > ( obj_new_DSAC );
    if ( hist_new == NULL ){
      cout<<"Cannot convert obj_new_DSAC to TH1* "<<std::endl;
      exit(EXIT_FAILURE);
    }
    Double_t scale_old = 1/hist_old->Integral();
    Double_t scale_new = 1/hist_new->Integral();
    hist_old->Scale(scale_old);
    hist_new->Scale(scale_new);
    hist_old->SetMinimum(0);
    hist_new->SetMinimum(0);
    CompareHist_1D( hist_old, hist_new );
  }
  else if ( obj_old_DSAC->InheritsFrom("TH2") ){
    std::cout<<setw( levelNo * 5 + 2 )<<"|_";
    std::cout<<"This obj inherits from TH2"<<std::endl;
    TH2* hist_old = dynamic_cast< TH2* > ( obj_old_DSAC );
    if ( hist_old == NULL ){
      cout<<"Cannot convert obj_old_DSAC to TH2* "<<std::endl;
      exit(EXIT_FAILURE);
    }
    TH2* hist_new = dynamic_cast< TH2* > ( obj_new_DSAC );
    if ( hist_new == NULL ){
      cout<<"Cannot convert obj_new_DSAC to TH2* "<<std::endl;
      exit(EXIT_FAILURE);
    }
    Double_t scale_old = 1/hist_old->Integral();
    Double_t scale_new = 1/hist_new->Integral();
    hist_old->Scale(scale_old);
    hist_new->Scale(scale_new);
    CompareHist_2D( hist_old, hist_new );
  }
  else if ( obj_old_DSAC->InheritsFrom("TFile") ){
    std::cout<<setw( levelNo * 5 + 2 )<<"|_";
    std::cout<<"This obj inherits from TFile"<<std::endl;
    TCollection* col_old = (TCollection*) ( ( TFile* ) obj_old_DSAC ) -> GetListOfKeys();
    TCollection* col_new = (TCollection*) ( ( TFile* ) obj_new_DSAC ) -> GetListOfKeys();
    TIter iter_col_old( col_old );
    TIter iter_col_new( col_new );
    TKey* key_old;
    TKey* key_new;
    levelNo++;
    for(;;){
      bool isTheEnd = !( key_old = (TKey*) iter_col_old() );
      if ( isTheEnd ){
        levelNo--;
        break;
      }
      else{
        key_new = (TKey*) iter_col_new();
        TObject* obj_old(0);
        TObject* obj_new(0);
        obj_old = key_old -> ReadObj();
        obj_new = key_new -> ReadObj();
        SearchAndCompare( obj_old, obj_new );
      }
    }
  }
  else if ( obj_old_DSAC->InheritsFrom("TFolder") ){
    std::cout<<setw( levelNo * 5 + 2 )<<"|_";
    std::cout<<"This obj inherits from TFolder"<<std::endl;
    TCollection* col_old = ( ( TFolder* ) obj_old_DSAC ) -> GetListOfFolders();
    TCollection* col_new = ( ( TFolder* ) obj_new_DSAC ) -> GetListOfFolders();
    std::cout<<"The size of col_old is "<<col_old->GetSize()<<std::endl;
    TIter iter_col_old( col_old );
    TIter iter_col_new( col_new );
    levelNo++;
    for(;;){
      TObject* obj_old(0);
      TObject* obj_new(0);
      bool isTheEnd = !( obj_old = iter_col_old() );
      if ( isTheEnd ){
        levelNo--;
        break;
      }
      else{
        obj_new = iter_col_new();
        SearchAndCompare( obj_old , obj_new );
      }
    }
  }
  else{
    std::cout<<std::endl;
    std::cout<<"Cannot deal with this kind of TObject yet!!!"<<std::endl;
    std::cout<<"More information about this obj :"<<std::endl;
    obj_old_DSAC->Print();
    std::cout<<std::endl;
  }
}

void AutoHistoCompare::CompareHist_1D( TH1* hist_old_CH, TH1* hist_new_CH ){

  gStyle->SetPadTopMargin(0.15);

  char title[50];
  TCanvas c1;

  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);

  hist_old_CH->SetLineColor(2);
  hist_new_CH->SetLineColor(4);
  hist_old_CH->SetLineStyle(3);
  hist_new_CH->SetLineStyle(5);
  hist_new_CH->Draw();
  hist_old_CH->Draw("Same");
  leg->Clear();
  leg->AddEntry( hist_old_CH, "E_{#gamma}=100MeV", "l");
  leg->AddEntry( hist_new_CH, "E_{#gamma}=103MeV", "l");
  leg->Draw();

  Double_t *res; 
  mypv= 0.000001;
  if (method == 1) {
    mypv = hist_old_CH->Chi2Test(hist_new_CH,"UU" ,res);
  }
  else {
    mypv = hist_old_CH->KolmogorovTest(hist_new_CH, "UO");
  }
  if (mypv< 0.01){
    sprintf(title,"%s%s%s","LT_", hist_old_CH->GetName(),".pdf");
  }
  else {
    sprintf(title,"%s%s", hist_old_CH ->GetName(),".pdf");
  }

  std::strstream buf;
  std::string value;
  buf<<"PV="<<mypv<<std::endl;
  buf>>value;
  //te->DrawTextNDC(0.4,0.85, value.c_str());

  c1.Print(title);
  PvHisto->Fill(mypv);

  std::cout<<setw( levelNo * 5 + 2 )<<"|_";
  std::cout << "[OVAL] " << hist_old_CH->GetName() << " PV = " << mypv << std::endl;

  return;
  
}

void AutoHistoCompare::CompareHist_2D( TH2* hist_old_CH, TH2* hist_new_CH ){

  char title[50];
  TCanvas c1("c","c",600,800);
  c1.Divide(1,2);

  gStyle->SetPadTopMargin(0.15);

  //hist_old_CH->SetMarkerColor(2);
  //hist_new_CH->SetMarkerColor(4);
  hist_old_CH->SetLineColor(2);
  hist_new_CH->SetLineColor(4);
  c1.cd(1);
  hist_old_CH->Draw();
  c1.cd(2);
  hist_new_CH->Draw();

  Double_t *res; 
  mypv= 0.000001;
  if (method == 1) {
    mypv = hist_old_CH->Chi2Test(hist_new_CH,"OU");
  }
  else {
    mypv = hist_old_CH->KolmogorovTest(hist_new_CH, "UO");
  }
  if (mypv< 0.01){
    sprintf(title,"%s%s%s","LT_", hist_old_CH->GetName(),".pdf");
  }
  else {
    sprintf(title,"%s%s", hist_old_CH ->GetName(),".pdf");
  }
  
  std::strstream buf;
  std::string value;
  buf<<"PV="<<mypv<<std::endl;
  buf>>value;
  c1.cd(1);
  te->DrawTextNDC(0.4,0.85, value.c_str());

  c1.Print(title);
  PvHisto->Fill(mypv);

  std::cout<<setw( levelNo * 5 + 2 )<<"|_";
  std::cout << "[OVAL] " << hist_old_CH->GetName() << " PV = " << mypv << std::endl;

  return;
  
}
