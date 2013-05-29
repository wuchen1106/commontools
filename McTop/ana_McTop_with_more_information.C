#include <vector>
#include "TString.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TF1.h"
#include "TChain.h"
#include <iostream>
#include "TCanvas.h"
#include <fstream>

void ana( )
{
  //preset
  std::string tree_name = "t";
  std::string filename = "RPC002_raw.root";
  std::string filepath = "/home/wuchen/workarea/Simulate/CometPhaseI/data/";
  std::string full_filename = filepath + filename;

//histogram
//  TString nameOfViriable1D("dedx/E");
//  TString nameOfViriable2D("E:dedx/E");
//  TString nameOfViriable3D("HitPosition");

//  Int_t bin = 150;
//  Double_t left = 0.;
//  Double_t right = 5e-4;
//
//  Int_t bin1 = 100;
//  Double_t left1 = 4e-2;
//  Double_t right1 = 1.1e-1;
//
//  Int_t bin2 = 100;
//  Double_t left2 = 0;
//  Double_t right2 = 5e-5;

//  Int_t bin3 = 300;
//  Double_t left3 = -75;
//  Double_t right3 = 75;

//  TH1D* h1D1 = new TH1D(nameOfViriable1D,nameOfViriable1D,bin,left,right);
//  TH2D* h2D1 = new TH2D(nameOfViriable2D,nameOfViriable2D,bin1,left1,right1,bin2,left2,right2);
//  TH3D* h3D1 = new TH3D(nameOfViriable3D,nameOfViriable3D,bin1,left1,right1,bin2,left2,right2,bin3,left3,right3);


//file info
  TChain* m_TChain = new TChain(tree_name.c_str());
  m_TChain->Add(full_filename.c_str());
//Constants
  Double_t e_mass = 0.000510999;
//Statistic Info
//Root Info

  int evt_num;

  int ini_ntrack;
  double ini_x[100];
  double ini_y[100];
  double ini_z[100];
  double ini_t[100];
  double ini_px[100];
  double ini_py[100];
  double ini_pz[100];
  double ini_e[100];
  int ini_pid[100];
  int ini_tid[100];
  int ini_ptid[100];
  int ini_prid[100];
  int ini_vid[100];

  int trigger_nhit;
  double trigger_x[100];
  double trigger_y[100];
  double trigger_z[100];
  double trigger_t[100];
  double trigger_px[100];
  double trigger_py[100];
  double trigger_pz[100];
  double trigger_e[100];
  int trigger_tid[100];
  int trigger_vid[100];

  int driftChamber_nhit;
  double driftChamber_x[10000];
  double driftChamber_y[10000];
  double driftChamber_z[10000];
  double driftChamber_t[10000];
  double driftChamber_px[10000];
  double driftChamber_py[10000];
  double driftChamber_pz[10000];
  double driftChamber_e[10000];
  int driftChamber_tid[10000];
  int driftChamber_vid[10000];

//Set Branch Address
//  m_TChain->SetBranchAddress("runNo", &runNo);

  m_TChain->SetBranchAddress("evt_num", &evt_num);

  m_TChain->SetBranchAddress("ini_ntrack", &ini_ntrack);
  m_TChain->SetBranchAddress("ini_x", ini_x);
  m_TChain->SetBranchAddress("ini_y", ini_y);
  m_TChain->SetBranchAddress("ini_z", ini_z);
  m_TChain->SetBranchAddress("ini_t", ini_t);
  m_TChain->SetBranchAddress("ini_px", ini_px);
  m_TChain->SetBranchAddress("ini_py", ini_py);
  m_TChain->SetBranchAddress("ini_pz", ini_pz);
  m_TChain->SetBranchAddress("ini_e", ini_e);
  m_TChain->SetBranchAddress("ini_pid", ini_pid);
  m_TChain->SetBranchAddress("ini_tid", ini_tid);
  m_TChain->SetBranchAddress("ini_ptid", ini_ptid);
  m_TChain->SetBranchAddress("ini_prid", ini_prid);
  m_TChain->SetBranchAddress("ini_vid", ini_vid);

  m_TChain->SetBranchAddress("trigger_nhit", &trigger_nhit);
  m_TChain->SetBranchAddress("trigger_x", trigger_x);
  m_TChain->SetBranchAddress("trigger_y", trigger_y);
  m_TChain->SetBranchAddress("trigger_z", trigger_z);
  m_TChain->SetBranchAddress("trigger_t", trigger_t);
  m_TChain->SetBranchAddress("trigger_px", trigger_px);
  m_TChain->SetBranchAddress("trigger_py", trigger_py);
  m_TChain->SetBranchAddress("trigger_pz", trigger_pz);
  m_TChain->SetBranchAddress("trigger_e", trigger_e);
  m_TChain->SetBranchAddress("trigger_tid", trigger_tid);
  m_TChain->SetBranchAddress("trigger_vid", trigger_vid);

  m_TChain->SetBranchAddress("driftChamber_nhit", &driftChamber_nhit);
  m_TChain->SetBranchAddress("driftChamber_x", driftChamber_x);
  m_TChain->SetBranchAddress("driftChamber_y", driftChamber_y);
  m_TChain->SetBranchAddress("driftChamber_z", driftChamber_z);
  m_TChain->SetBranchAddress("driftChamber_t", driftChamber_t);
  m_TChain->SetBranchAddress("driftChamber_px", driftChamber_px);
  m_TChain->SetBranchAddress("driftChamber_py", driftChamber_py);
  m_TChain->SetBranchAddress("driftChamber_pz", driftChamber_pz);
  m_TChain->SetBranchAddress("driftChamber_e", driftChamber_e);
  m_TChain->SetBranchAddress("driftChamber_tid", driftChamber_tid);
  m_TChain->SetBranchAddress("driftChamber_vid", driftChamber_vid);

//read m_TChain
  Long64_t nEvent = m_TChain->GetEntries();
  for( Long64_t iEvent = 0; iEvent < nEvent; iEvent++ ){
//  for( Long64_t iEvent = 600; iEvent < 610; iEvent++ ){
    m_TChain->GetEntry(iEvent);
    Int_t m_tid[100][100];
    Int_t m_ptid[100][100];
    Int_t m_pid[100][100];
    Int_t m_prid[100][100];
    Int_t n_par[100];
    m_tid[0][0] = 1;
    m_ptid[0][0] = 0;
    m_pid[0][0] = 22;
    m_prid[0][0] = -1;
    n_par[0] = 1;
    Int_t n_level = 0;
    Int_t count = 1;
//    for ( int i = 0; i<ini_ntrack; i++){
//      std::cout<<"pid = "<<ini_pid[i]<<std::endl;
//      std::cout<<"tid = "<<ini_tid[i]<<std::endl;
//      std::cout<<"ptid = "<<ini_ptid[i]<<std::endl;
//      std::cout<<"prid = "<<ini_prid[i]<<std::endl;
//    }
loop:    for ( Int_t i = 0; i < n_par[n_level]; i++ ){
      Int_t ptid = m_tid[n_level][i];
      for ( Int_t j = 0; j < ini_ntrack; j++ ){
        if ( ini_ptid[j] == ptid ){
          Int_t npar = n_par[n_level+1];
          m_tid[n_level+1][npar] = ini_tid[j];
          m_ptid[n_level+1][npar] = ini_ptid[j];
          m_pid[n_level+1][npar] = ini_pid[j];
          m_prid[n_level+1][npar] = ini_prid[j];
          n_par[n_level+1] += 1;
          count++;
        }
      }
    }
    n_level++;
    if ( count < ini_ntrack )
      goto loop;
    n_level++;
    if ( ini_ntrack != 1 ){

      std::cout<<"********************Event "<<iEvent<<"**************************"<<std::endl;
      for (int i=0; i<n_level; i++){
        for (int j=0; j<n_par[i]; j++){
          std::cout<<m_tid[i][j]<<" ";
        }
        std::cout<<std::endl;
      }
      for (int i=0; i<n_level; i++){
        for (int j=0; j<n_par[i]; j++){
          std::cout<<m_ptid[i][j]<<" ";
        }
        std::cout<<std::endl;
      }
      for (int i=0; i<n_level; i++){
        for (int j=0; j<n_par[i]; j++){
          std::cout<<m_pid[i][j]<<" ";
        }
        std::cout<<std::endl;
      }
      for (int i=0; i<n_level; i++){
        for (int j=0; j<n_par[i]; j++){
          std::cout<<m_prid[i][j]<<" ";
        }
        std::cout<<std::endl;
      }
      Int_t i_level = 0;
      Int_t i_par[100];
      Int_t cur_par = 0;
      std::cout<<"|_pid="<<22<<", tid="<<1<<", prid="<<-1<<std::endl;
      i_par[0] += 1;
      Int_t cur_ptid;
      Int_t up_tid;
loop2:  cur_ptid = m_ptid[i_level+1][cur_par];
      up_tid = m_tid[i_level][i_par[i_level]-1];
      std::cout<<"Ah, a new start..."<<std::endl;
      if ( cur_ptid == up_tid ){
        std::cout<<"Ooh, found a daugter..."<<std::endl;
        for ( Int_t i = 0; i <= i_level; i++ ){
          std::cout<<" ";
        }
        std::cout<<"|_pid="<<m_pid[i_level+1][cur_par]<<", tid="<<m_tid[i_level+1][cur_par]<<", prid="<<m_prid[i_level+1][cur_par]<<std::endl;
        i_par[i_level+1] += 1;
        if ( i_level + 1 == n_level - 1 ){
          std::cout<<"En, the bottom..."<<std::endl;
          if ( cur_par == n_par[i_level + 1] - 1 ){
            std::cout<<"OK, right end..."<<std::endl;
            if (i_level == 0)
              goto finish;
            i_level--;
            while ( i_par[i_level+1] == n_par[i_level+1] ){
              if (i_level == 0)
                goto finish;
              i_level--;
            }
            cur_par = i_par[i_level+1];
            goto loop2;
          }
          else{
            std::cout<<"Fine, going right..."<<std::endl;
            cur_par++;
            goto loop2;
          }
        }
        else{
          std::cout<<"Yeah, coming down..."<<std::endl;
          i_level++;
          while ( i_par[i_level+1] == n_par[i_level+1] ){
            if (i_level == 0)
              goto finish;
            i_level--;
          }
          cur_par = i_par[i_level+1];
          goto loop2;
        }
      }
      else{
        std::cout<<"Allright, it not her daughter..."<<std::endl;
        if ( cur_par == n_par[i_level + 1] - 1 ){
          std::cout<<"En, the bottom..."<<std::endl;
          if (i_level == 0)
            goto wrong;
          i_level--;
          cur_par = i_par[i_level+1];
          goto loop2;
        }
        else{
          std::cout<<"Fine, going right..."<<std::endl;
          cur_par++;
          goto loop2;
        }
      }
wrong: std::cout<<"Wrong!!!"<<std::endl;
finish: std::cout<<"Done!!!"<<std::endl;
    }
  }
}
