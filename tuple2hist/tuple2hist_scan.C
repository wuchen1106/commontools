#include <iostream>
#include <fstream>
#include <strstream>
#include <vector>
#include <iomanip>
#include "TFile.h"
#include "TH1.h"
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
#include "TBranch.h"
#include "TLeaf.h"

void tuple2hist_scan(std::string name_file = "/home/chen/MyWorkArea/MyStudies/AntiProton/Simulate/configure/data/spectrum.root"){

  //std::string name_file = "/home/chen/workarea/Analyse/common_tools/tuple2hist/input.root";
  Int_t myBin = 200;

  std::ofstream myFstream( "tuple2hist_card.txt" );
  TFile* myFile = new TFile( name_file.c_str() );
  if ( myFile->IsZombie() ){
    std::cout<<"Cannot create myFile !!! "<<std::endl;
    std::cout<<"Something is wrong with file :"<<name_file<<std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout<<"the file to scan is : "<<std::endl;
  myFile->Print();
  std::cout<<std::endl;
  myFstream<<"Filename: "<<name_file<<std::endl;

  TCollection* col_file = (TCollection*) myFile -> GetListOfKeys();
  TIter iter_col_file( col_file );
  TObject* obj_file_as_a_key(0);
  while ( obj_file_as_a_key = iter_col_file() ){
    TKey* myKey = dynamic_cast<TKey*> (obj_file_as_a_key);
    if ( myKey == NULL ){
      std::cout<<"Cannot convert obj_file_as_a_key to TKey "<<std::endl;
      exit( EXIT_FAILURE );
    }
    TObject* obj_file(0);
    obj_file = myKey -> ReadObj();
    std::string name_tree = obj_file -> GetName(); 
    if ( obj_file -> InheritsFrom( "TTree" ) ){
      TTree* myTree = dynamic_cast<TTree*> ( obj_file );
      if ( myTree == NULL ){
        std::cout<<"Cannot convert "<<name_tree<<" to TTree "<<std::endl;
        exit( EXIT_FAILURE );
      }

      Long64_t tree_nEntries = myTree -> GetEntries();
      myFstream<<"Tuplename: "<<name_tree<<" , with "<<tree_nEntries<<" entries inside."<<std::endl;

      TCollection* col_tree = dynamic_cast<TCollection*> ( myTree -> GetListOfBranches() );
      if ( col_tree == NULL ){
        std::cout<<"Cannot convert col of "<<name_tree<<" to TCollection "<<std::endl;
        exit( EXIT_FAILURE );
      }
      TIter iter_col_tree( col_tree );
      TObject* obj_tree(0);
      while ( obj_tree = iter_col_tree() ){
        std::string name_branch = obj_tree -> GetName();
        if ( obj_tree -> InheritsFrom( "TBranch" ) ){
          TBranch* myBranch = dynamic_cast<TBranch*> ( obj_tree );
          if ( myBranch == NULL ){
            std::cout<<"Cannot convert "<<name_branch<<" to TBranch "<<std::endl;
            exit( EXIT_FAILURE );
          }
          TCollection* col_branch = dynamic_cast<TCollection*> ( myBranch -> GetListOfLeaves() );
          if ( col_branch == NULL ){
            std::cout<<"Cannot convert col of "<<name_branch<<" to TCollection "<<std::endl;
            exit( EXIT_FAILURE );
          }
          TIter iter_col_branch( col_branch );
          TObject* obj_branch(0);
          Int_t branch_n_elem = 0;
          bool is_there_array = false;
          Double_t branch_min = 0;
          Double_t branch_max = 0;
          while ( obj_branch = iter_col_branch() ){
            branch_n_elem++;
            std::string name_leaf = obj_branch -> GetName();
            if (obj_branch -> InheritsFrom ("TLeaf") ){
              TLeaf* myLeaf = dynamic_cast<TLeaf*> ( obj_branch );
              if ( myLeaf == NULL ){
                std::cout<<"Cannot convert "<<name_leaf<<" to TLeaf "<<std::endl;
                exit( EXIT_FAILURE );
              }
//              Int_t leaf_min = myLeaf -> GetMinimum();
//              Int_t leaf_max = myLeaf -> GetMaximum();
//              std::string name_leaf_type = myLeaf -> GetTypeName();
              Int_t leaf_counter = -2;
              TLeaf* leaf_counter_leaf = myLeaf -> GetLeafCounter( leaf_counter );
              bool is_counter_null = ( leaf_counter_leaf == NULL );
//              bool is_leaf_range = myLeaf -> IsRange();
//              Int_t leaf_getLen = myLeaf -> GetLen();
//              Int_t leaf_getLenStatic = myLeaf -> GetLenStatic();
//              Int_t leaf_getLenType = myLeaf -> GetLenType();
//              Int_t leaf_getNdata = myLeaf -> GetNdata();
//              std::cout<<"Leaf in "<<name_branch<<":"<<std::endl;
//              std::cout<<"     Name: "<<name_leaf<<std::endl;
//              std::cout<<"     min: "<<leaf_min<<std::endl;
//              std::cout<<"     max: "<<leaf_max<<std::endl;
//              std::cout<<"     type: "<<leaf_type<<std::endl;
//              std::cout<<"     counter: "<<leaf_counter<<std::endl;
//              std::cout<<"     is counter leaf NULL? "<<is_counter_null<<std::endl;
//              std::cout<<"     is leaf range? "<<is_leaf_range<<std::endl;
//              std::cout<<"     getLen: "<<leaf_getLen<<std::endl;
//              std::cout<<"     getLenStatic: "<<leaf_getLenStatic<<std::endl;
//              std::cout<<"     getLenType: "<<leaf_getLenType<<std::endl;
//              std::cout<<"     getNdata: "<<leaf_getNdata<<std::endl;
              if ( leaf_counter != 1 || ! is_counter_null ){
                std::cout<<"  "<<name_leaf<<" contains arrays inside!!!"<<std::endl;
                std::cout<<"  Sorry, this program cannot deal with arrays yet!"<<std::endl;
                std::cout<<std::endl;
                is_there_array = true;
              }
            }
            else{
              std::cout<<"  "<<name_leaf<<" in "<<name_branch<<" is not a Tleaf!!!"<<std::endl;
              std::cout<<std::endl;
            }
          }
          if ( branch_n_elem > 1 ){
            std::cout<<"There are more than one element in "<<name_branch<<std::endl;
            std::cout<<"Sorry, this program cannot deal with this kind of TBranch yet!"<<std::endl;
            std::cout<<std::endl;
          }
          else if ( is_there_array ){
            std::cout<<"There are arrays in "<<name_branch<<std::endl;
            std::cout<<"Sorry, this program cannot deal with this kind of TBranch yet!"<<std::endl;
            std::cout<<std::endl;
          }
          else{
            TCollection* col_branch_for_loop = dynamic_cast<TCollection*> ( myBranch -> GetListOfLeaves() );
            TIter iter_col_branch_for_loop( col_branch_for_loop );
            TLeaf* myLeaf_for_loop = dynamic_cast<TLeaf*> ( iter_col_branch_for_loop() );
            std::string name_leaf_type_for_loop = myLeaf_for_loop -> GetTypeName();
            std::string name_leaf_hist_type_for_loop;
            if ( name_leaf_type_for_loop == "Double_t" ){
              name_leaf_hist_type_for_loop = "TH1D";
            }
            else if ( name_leaf_type_for_loop == "Float_t" ){
              name_leaf_hist_type_for_loop = "TH1F";
            }
            else if ( name_leaf_type_for_loop == "Int_t" ){
              name_leaf_hist_type_for_loop = "TH1I";
            }
            else{
              name_leaf_hist_type_for_loop = "TH1";
              std::cout<<"Cannot deal with type \""<<name_leaf_type_for_loop<<"\" yet!!!"<<std::endl;
              std::cout<<"Please finish hist_type for TBranch "<<name_branch<<" in tuple2hist_card.txt!!!"<<std::endl;
            }
            for ( Int_t iEvent = 0; iEvent < tree_nEntries; iEvent++ ){
              myBranch -> GetEntry( iEvent );
              Double_t branch_value = myLeaf_for_loop -> GetValue();
              if ( iEvent == 0 ){
                branch_min = branch_value;
                branch_max = branch_value;
              }
              if ( branch_value < branch_min ){
                branch_min = branch_value;
              }
              if ( branch_value > branch_max ){
                branch_max = branch_value;
              }
            }
          myFstream<<setiosflags(ios::left)<<setw(7)<<"TH1:"<<setiosflags(ios::left)<<setw(20)<<name_branch<<setiosflags(ios::left)<<setw(5)<<" "<<name_leaf_hist_type_for_loop<<" "<<setiosflags(ios::left)<<setw(10)<<name_leaf_type_for_loop<<" "<<setiosflags(ios::left)<<setw(5)<<myBin<<" "<<setiosflags(ios::left)<<setw(12)<<branch_min<<" "<<setiosflags(ios::left)<<setw(12)<<branch_max<<std::endl;
          }
        }
        else{
          std::cout<<name_branch<<" in "<<name_tree<<" is not a TBranch!!!"<<std::endl;
          std::cout<<std::endl;
        }
      }
    }
    else{
      std::cout<<name_tree<<" in "<<name_file<<" is not a TTree!!!"<<std::endl;
      std::cout<<std::endl;
    }
  }
  myFstream.close();
}
