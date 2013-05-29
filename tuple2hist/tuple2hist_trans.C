#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "TFile.h"
#include "TCollection.h"
#include "TBranch.h"
#include "TKey.h"
#include "TObject.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include "TFolder.h"
#include "TVector3.h"
#include "TLorentzVector.h"

void ana(){

//**************************************Preset**************************************************  

  vector<string> vec_tree;
  vector< pair<string,string> > vec_branch_tree;

  TObjArray* list_hist = new TObjArray(0);

//Ignore me! I am just a separator!

  string fileTuple_name = "/home/chen/workarea/Analyse/CometPhaseI/GammaStudy_103_2/ana_tools/Merge/output.root";
  string fileHist_name = "hist.root";

  vec_tree.push_back( "t" );
  pair<string,string> pair_t_x( "t", "x" );
  vec_branch_tree.push_back( pair_t_x );
  pair<string,string> pair_t_y( "t", "y" );
  vec_branch_tree.push_back( pair_t_y );
  pair<string,string> pair_t_z( "t", "z" );
  vec_branch_tree.push_back( pair_t_z );
  pair<string,string> pair_t_kpx( "t", "kpx" );
  vec_branch_tree.push_back( pair_t_kpx );
  pair<string,string> pair_t_kpy( "t", "kpy" );
  vec_branch_tree.push_back( pair_t_kpy );
  pair<string,string> pair_t_kpz( "t", "kpz" );
  vec_branch_tree.push_back( pair_t_kpz );

//Ignore me! I am just a separator!

  TFile* myFile = new TFile ( fileTuple_name.c_str() );
  if ( myFile -> IsZombie() ){
    std::cout<<"Cannot create myFile!!! "<<std::endl;
    std::cout<<"Something is wrong with file :"<<std::endl;
    exit(EXIT_FAILURE);
  }

//**************************************Precheck************************************************

  TCollection* myCol = (TCollection*) myFile -> GetListOfKeys();
  //First of all, make sure every TTree in the card can be found in myFile.
  bool isEveryOneOK = true;
  for ( int i = 0; i < vec_branch_tree.size(); i++ ){
    pair<string,string> myPair = vec_branch_tree[i];
    string name_tree = myPair.first;
    string name_branch = myPair.second;
    TObject* obj_from_key_find = myCol -> FindObject( name_tree.c_str() );
    if ( obj_from_key_find == 0 ){
      std::cout<<"TTree   "<<setiosflags(ios::left)<<setw(20)<<name_tree<<" can not be found in myFile !!!"<<std::endl;
      isEveryOneOK = false;
    }
    else{
      TKey* key_find = dynamic_cast<TKey*> ( obj_from_key_find );
      if ( key_find == NULL ){
        std::cout<<"Cannot convert obj_from_key_find to key_find "<<std::endl;
        exit(EXIT_FAILURE);
      }
      TObject* obj_find = key_find -> ReadObj();
      if ( obj_find -> InheritsFrom("TTree") ){
        TTree* tree_find = dynamic_cast<TTree*> ( obj_find );
        if ( tree_find == NULL ){
          std::cout<<"Cannot convert "<<name_tree<<" to TTree "<<std::endl;
          exit(EXIT_FAILURE);
        }
        TCollection* col_tree_find = dynamic_cast<TCollection*> ( tree_find -> GetListOfBranches() );
        if ( col_tree_find == NULL ){
          std::cout<<"Cannot convert col of "<<name_tree<<" to TCollection "<<std::endl;
          exit(EXIT_FAILURE);
        }
        TObject* it_branch_find = col_tree_find -> FindObject( name_branch.c_str() );
        if ( it_branch_find == 0 ){
          std::cout<<"TBranch   "<<setiosflags(ios::left)<<setw(20)<<name_branch<<" can not be found in "<<name_tree<<" !!!"<<std::endl;
          isEveryOneOK = false;
        }
        else{
          if ( ! it_branch_find -> InheritsFrom("TBranch") ){
            std::cout<<setiosflags(ios::left)<<setw(20)<<name_branch<<" in   "<<setiosflags(ios::left)<<setw(20)<<name_tree<<" is not a TBranch!!!"<<std::endl;
            isEveryOneOK = false;
          }
        }

      }
      else{
        std::cout<<setiosflags(ios::left)<<setw(20)<<name_tree<<" in myFile is not a TTree!!!"<<std::endl;
        obj_find -> Print();
        isEveryOneOK = false;
      }
    }
  }
  if ( !isEveryOneOK ){
    exit(EXIT_FAILURE);
  }

  //Secondly, Check objs within myFile. Output info.
  //Plus, if there is TFolder inside, keep them in new hist_file
  TIter iter_tree( myCol );
  TKey* key_tree;
  while ( key_tree = (TKey*) iter_tree() ){
    TObject* obj_tree = key_tree -> ReadObj();
    string name_obj_tree = obj_tree -> GetName();
    if ( obj_tree -> InheritsFrom("TTree") ){
      vector<string>::iterator result = std::find( vec_tree.begin(), vec_tree.end(), name_obj_tree );
      if ( result == vec_tree.end() ){
        std::cout<<"TTree   "<<setiosflags(ios::left)<<setw(20)<<name_obj_tree<<" in myFile is not included in the card:"<<std::endl;
      }
      else{
        TTree* myTree = dynamic_cast<TTree*> ( obj_tree );
        if ( myTree == NULL ){
          std::cout<<"Cannot convert obj_tree to TTree "<<std::endl;
          exit(EXIT_FAILURE);
        }
        TCollection* col_tree = dynamic_cast<TCollection*> ( myTree -> GetListOfBranches() );
        if ( col_tree == NULL ){
          std::cout<<"Cannot convert list of obj_tree to TCollection "<<std::endl;
          exit(EXIT_FAILURE);
        }
        TIter iter_branch( col_tree );
        TObject* obj_branch;
        while ( obj_branch = iter_branch() ){
          string name_obj_branch = obj_branch -> GetName();
          if ( obj_branch -> InheritsFrom("TBranch") ){
            pair<string,string> myPair( name_obj_tree, name_obj_branch );
            vector< pair<string,string> >::iterator result = std::find( vec_branch_tree.begin(), vec_branch_tree.end(), myPair );
            if ( result == vec_branch_tree.end() ){
              std::cout<<"TBranch   "<<setiosflags(ios::left)<<setw(20)<<name_obj_branch<<" in "<<setiosflags(ios::left)<<setw(20)<<name_obj_tree<<" is not included in the card:"<<std::endl;
            }
          }
          else{
            std::cout<<setiosflags(ios::left)<<setw(20)<<name_obj_branch<<" in "<<setiosflags(ios::left)<<setw(20)<<name_obj_tree<<"is not a TBranch!!!"<<std::endl;
          }
        }
      }
    }
    else if ( obj_tree -> InheritsFrom("TFolder") ){
      std::cout<<setiosflags(ios::left)<<setw(20)<<name_obj_tree<<" in myFile is a TFolder!!!"<<std::endl;
      TFolder* myFolder = dynamic_cast<TFolder*> ( obj_tree );
      list_hist -> Add( myFolder );
    }
    else{
      std::cout<<setiosflags(ios::left)<<setw(20)<<name_obj_tree<<" in myFile does not inherit from a TTree!!!"<<std::endl;
    }
  }
  myFile -> Close();

//*********************************Set hist and branch************************************************

//Ignore me! I am just a separator!

  TChain* chain_t = new TChain( "t" );

  chain_t -> Add( fileTuple_name.c_str() );

  TFolder* folder_t = new TFolder( "t", "t" );

  list_hist -> Add( folder_t );

  Double_t t_x;
  Double_t t_y;
  Double_t t_z;
  Double_t t_kpx;
  Double_t t_kpy;
  Double_t t_kpz;

  chain_t -> SetBranchAddress( "x", &t_x );
  chain_t -> SetBranchAddress( "y", &t_y );
  chain_t -> SetBranchAddress( "z", &t_z );
  chain_t -> SetBranchAddress( "kpx", &t_kpx );
  chain_t -> SetBranchAddress( "kpy", &t_kpy );
  chain_t -> SetBranchAddress( "kpz", &t_kpz );

  TH1D* hist_t_x = new TH1D( "t_x", "x", 200, -10, 10 );
  TH1D* hist_t_y = new TH1D( "t_y", "y", 200, -10, 10 );
  TH1D* hist_t_z = new TH1D( "t_z", "z", 33, -41.25, 41.25 );
  TH1D* hist_t_r = new TH1D( "t_r", "r", 200, 0, 10 );
  TH1D* hist_t_kpx = new TH1D( "t_kpx", "kpx", 200, -1.01, 1.01 );
  TH1D* hist_t_kpy = new TH1D( "t_kpy", "kpy", 200, -1.01, 1.01 );
  TH1D* hist_t_kpz = new TH1D( "t_kpz", "kpz", 200, -1.01, 1.01 );
  TH1D* hist_t_kpa = new TH1D( "t_kpa", "kpa", 200, 0.9, 1.01 );
  TH1D* hist_t_kpt = new TH1D( "t_kpt", "kpt", 200, 0, 1.01 );
  TH1D* hist_t_pos_phi = new TH1D( "t_pos_phi", "pos_phi", 200, -3.15, 3.15 );
  TH1D* hist_t_mom_phi = new TH1D( "t_mom_phi", "mom_phi", 200, -3.15, 3.15 );
  TH1D* hist_t_mom_theta = new TH1D( "t_mom_theta", "mom_theta", 200, 0, 3.15 );

  folder_t -> Add( hist_t_x );
  folder_t -> Add( hist_t_y );
  folder_t -> Add( hist_t_z );
  folder_t -> Add( hist_t_r );
  folder_t -> Add( hist_t_kpx );
  folder_t -> Add( hist_t_kpy );
  folder_t -> Add( hist_t_kpz );
  folder_t -> Add( hist_t_kpa );
  folder_t -> Add( hist_t_kpt );
  folder_t -> Add( hist_t_pos_phi );
  folder_t -> Add( hist_t_mom_phi );
  folder_t -> Add( hist_t_mom_theta );

  Long64_t nEvent_t = chain_t -> GetEntries();
  for ( Long64_t iEvent = 0; iEvent < nEvent_t; iEvent++ ){

    chain_t -> GetEntry( iEvent );

    Double_t t_r;
    TVector3 v3_pos( t_x, t_y, 0 );
    t_r = v3_pos.Mag();
    Double_t t_kpa;
    TVector3 v3_pa( t_kpx, t_kpy, t_kpz );
    t_kpa = v3_pa.Mag();
    Double_t t_kpt;
    t_kpt = v3_pa.Perp();
    Double_t t_pos_phi;
    t_pos_phi = v3_pos.Phi();
    Double_t t_mom_phi;
    t_mom_phi = v3_pa.Phi();
    Double_t t_mom_theta;
    t_mom_theta = v3_pa.Theta();

    hist_t_x -> Fill( t_x );
    hist_t_y -> Fill( t_y );
    hist_t_z -> Fill( t_z );
    hist_t_r -> Fill( t_r );
    hist_t_kpx -> Fill( t_kpx );
    hist_t_kpy -> Fill( t_kpy );
    hist_t_kpz -> Fill( t_kpz );
    hist_t_kpa -> Fill( t_kpa );
    hist_t_kpt -> Fill( t_kpt );
    hist_t_pos_phi -> Fill( t_pos_phi );
    hist_t_mom_phi -> Fill( t_mom_phi );
    hist_t_mom_theta -> Fill( t_mom_theta );

  }
//Ignore me! I am just a separator!

	hist_t_kpa->Scale(1./hist_t_kpa->Integral()*5.195E-4*5/6);
  TFile file_hist( fileHist_name.c_str(), "RECREATE" );
  list_hist -> Write();
  file_hist.Close();

}
