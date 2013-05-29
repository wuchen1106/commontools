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

void tuple2hist_trans(){

//**************************************Preset**************************************************  

  vector<string> vec_tree;
  vector< pair<string,string> > vec_branch_tree;

  TObjArray* list_hist = new TObjArray(0);

//Ignore me! I am just a separator!

  string fileTuple_name = "/home/chen/MyWorkArea/MyStudies/AntiProton/Simulate/configure/data/spectrum.root";
  string fileHist_name = "hist.root";

  vec_tree.push_back( "t" );
  pair<string,string> pair_t_pa( "t", "pa" );
  vec_branch_tree.push_back( pair_t_pa );

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
      vector<string>::iterator result1 = std::find( vec_tree.begin(), vec_tree.end(), name_obj_tree );
      if ( result1 == vec_tree.end() ){
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
            vector< pair<string,string> >::iterator result2 = std::find( vec_branch_tree.begin(), vec_branch_tree.end(), myPair );
            if ( result2 == vec_branch_tree.end() ){
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

  Double_t t_pa;

  chain_t -> SetBranchAddress( "pa", &t_pa );

  TH1D* hist_t_pa = new TH1D( "t_pa", "pa", 100, 0.5, 100.5 );

  folder_t -> Add( hist_t_pa );

  Long64_t nEvent_t = chain_t -> GetEntries();
  for ( Long64_t iEvent = 0; iEvent < nEvent_t; iEvent++ ){

    chain_t -> GetEntry( iEvent );

    hist_t_pa -> Fill( t_pa );

  }
//Ignore me! I am just a separator!

//	hist_t_pa->Scale(1./hist_t_kpa->Integral()*5.195E-4*5/6);
  TFile file_hist( fileHist_name.c_str(), "RECREATE" );
  list_hist -> Write();
  file_hist.Close();

}
