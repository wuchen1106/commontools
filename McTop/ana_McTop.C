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

void ana( Long64_t iEvent = 0 )
{
  //preset
  std::string tree_name = "tree";
//  std::string filename = "MuonStudy_Tungsten.root";
//  std::string filename = "MuonStudy_Aluminium.root";
  std::string filename = "raw_CometPhaseI.root";
  std::string filepath = "/home/chen/CometPhaseI/CometPhaseI-build/output";
  std::string full_filename = filepath + "/" + filename;

  //unit
  int scale = 1000; //GeV
//  int scale = 1;    //MeV

	//output file
	std::string fFileName = "McTop_result.txt";
  std::ofstream fout;
  fout.open(fFileName.c_str());
	if(!fout){
		std::cout<<"Cannot find "<<fFileName<<std::endl;
		return;
	}

//histogram
  TString nameOfViriable1D("no Nucleus mom (MeV/c)");
  TString nameOfViriable2D("with Nucleus mom (MeV/c)");
//  TString nameOfViriable3D("HitPosition");

//  Int_t bin = 150;
//  Double_t left = 0.;
//  Double_t right = 5e-4;

  Int_t bin1 = 200;
  Double_t left1 = 0;
  Double_t right1 = 100;

  Int_t bin2 = 10000;
  Double_t left2 = 0;
  Double_t right2 = 100;

//  Int_t bin3 = 300;
//  Double_t left3 = -75;
//  Double_t right3 = 75;

  TH1D* h1D1 = new TH1D(nameOfViriable1D,nameOfViriable1D,bin1,left1,right1);
  TH1D* h1D2 = new TH1D(nameOfViriable2D,nameOfViriable2D,bin2,left2,right2);
//  TH2D* h2D1 = new TH2D(nameOfViriable2D,nameOfViriable2D,bin1,left1,right1,bin2,left2,right2);
//  TH3D* h3D1 = new TH3D(nameOfViriable3D,nameOfViriable3D,bin1,left1,right1,bin2,left2,right2,bin3,left3,right3);


//file info
  TChain* m_TChain = new TChain(tree_name.c_str());
  m_TChain->Add(full_filename.c_str());
//Constants
  Double_t e_mass = 0.000510999;
//Statistic Info
	//std::string ProcessWanted = "muMinusCaptureAtRest";
	//std::string ProcessWanted = "CHIPSCaptureAtRest";
	std::string ProcessWanted = "Decay";
	std::string ParticleWanted = "mu-";
	//std::string ParticleWanted = "e-";
	int NucleusThre = 1e6;
  int N1 = 0;
  int N2 = 0;
//Root Info

  int evt_num;
  int run_num;

  int McTruth_nTracks;
  vector<double>* McTruth_x = 0;
  vector<double>* McTruth_y = 0;
  vector<double>* McTruth_z = 0;
  vector<double>* McTruth_time = 0;
  vector<double>* McTruth_px = 0;
  vector<double>* McTruth_py = 0;
  vector<double>* McTruth_pz = 0;
  vector<double>* McTruth_e = 0;
  vector<int>* McTruth_charge = 0;
  vector<int>* McTruth_pid = 0;
  vector<int>* McTruth_tid = 0;
  vector<int>* McTruth_ptid = 0;
  vector<string>* McTruth_process = 0;
  vector<string>* McTruth_particleName = 0;
  vector<string>* McTruth_volume = 0;

  TBranch* bMcTruth_x = 0;
  TBranch* bMcTruth_y = 0;
  TBranch* bMcTruth_z = 0;
  TBranch* bMcTruth_time = 0;
  TBranch* bMcTruth_px = 0;
  TBranch* bMcTruth_py = 0;
  TBranch* bMcTruth_pz = 0;
  TBranch* bMcTruth_e = 0;
  TBranch* bMcTruth_charge = 0;
  TBranch* bMcTruth_pid = 0;
  TBranch* bMcTruth_tid = 0;
  TBranch* bMcTruth_ptid = 0;
  TBranch* bMcTruth_process = 0;
  TBranch* bMcTruth_particleName = 0;
  TBranch* bMcTruth_volume = 0;

//Set Branch Address
//  m_TChain->SetBranchAddress("runNo", &runNo);
  m_TChain->SetBranchAddress("evt_num", &evt_num);
  m_TChain->SetBranchAddress("run_num", &run_num);

  m_TChain->SetBranchAddress("McTruth_nTracks", &McTruth_nTracks);
  m_TChain->SetBranchAddress("McTruth_x", &McTruth_x, &bMcTruth_x);
  m_TChain->SetBranchAddress("McTruth_y", &McTruth_y, &bMcTruth_y);
  m_TChain->SetBranchAddress("McTruth_z", &McTruth_z, &bMcTruth_z);
  m_TChain->SetBranchAddress("McTruth_time", &McTruth_time, &bMcTruth_time);
  m_TChain->SetBranchAddress("McTruth_px", &McTruth_px, &bMcTruth_px);
  m_TChain->SetBranchAddress("McTruth_py", &McTruth_py, &bMcTruth_py);
  m_TChain->SetBranchAddress("McTruth_pz", &McTruth_pz, &bMcTruth_pz);
  m_TChain->SetBranchAddress("McTruth_e", &McTruth_e, &bMcTruth_e);
  m_TChain->SetBranchAddress("McTruth_charge", &McTruth_charge, &bMcTruth_charge);
  m_TChain->SetBranchAddress("McTruth_pid", &McTruth_pid, &bMcTruth_pid);
  m_TChain->SetBranchAddress("McTruth_tid", &McTruth_tid, &bMcTruth_tid);
  m_TChain->SetBranchAddress("McTruth_ptid", &McTruth_ptid, &bMcTruth_ptid);
  m_TChain->SetBranchAddress("McTruth_process", &McTruth_process, &bMcTruth_process);
  m_TChain->SetBranchAddress("McTruth_particleName", &McTruth_particleName, &bMcTruth_particleName);
  m_TChain->SetBranchAddress("McTruth_volume", &McTruth_volume, &bMcTruth_volume);

//read m_TChain
  Long64_t nEvent = m_TChain->GetEntries();
  fout<<"nEvent = "<<nEvent<<std::endl;
  for( Long64_t iEvent = 0; iEvent < nEvent; iEvent++ ){
		Long64_t tentry = m_TChain->LoadTree(iEvent);
		//for stat
		bool isProcessWanted = false;
		bool isParticleWanted = false;
		bool hasNucleus = false;
		std::vector<double> v_parMom;
		v_parMom.clear();

    bMcTruth_x->GetEntry(tentry);
    bMcTruth_y->GetEntry(tentry);
    bMcTruth_z->GetEntry(tentry);
    bMcTruth_time->GetEntry(tentry);
    bMcTruth_px->GetEntry(tentry);
    bMcTruth_py->GetEntry(tentry);
    bMcTruth_pz->GetEntry(tentry);
    bMcTruth_e->GetEntry(tentry);
    bMcTruth_charge->GetEntry(tentry);
    bMcTruth_pid->GetEntry(tentry);
    bMcTruth_tid->GetEntry(tentry);
    bMcTruth_ptid->GetEntry(tentry);
    bMcTruth_process->GetEntry(tentry);
    bMcTruth_particleName->GetEntry(tentry);
    bMcTruth_volume->GetEntry(tentry);

    m_TChain->GetEntry(iEvent);

    Int_t m_tid[400][400];
    Int_t m_ptid[400][400];
		std::string m_pid[400][400];
		std::string m_prid[400][400];
    Double_t m_e[400][400];
    Double_t m_p[400][400];
    Int_t n_par[400];
    for ( int i = 0; i < 400; i ++ ){
      n_par[i] = 0;
    }
    m_tid[0][0] = 1;
    m_ptid[0][0] = 0;
    m_pid[0][0] = (*McTruth_particleName)[0];
    m_prid[0][0] = "NOTHING";
    m_e[0][0] = (*McTruth_e)[0];
		double px= (*McTruth_px)[0];
		double py= (*McTruth_py)[0];
		double pz= (*McTruth_pz)[0];
		m_p[0][0] = sqrt(px*px+py*py+pz*pz);
    n_par[0] = 1;
    Int_t n_level = 0;
    Int_t count = 1;
    fout<<"McTruth_nTracks = "<<McTruth_nTracks<<std::endl;
loop: 
		for ( Int_t i = 0; i < n_par[n_level]; i++ ){
      Int_t ptid = m_tid[n_level][i];
      for ( Int_t j = 0; j < McTruth_nTracks; j++ ){
        if ( (*McTruth_ptid)[j] == ptid ){
          Int_t npar = n_par[n_level+1];
          m_tid[n_level+1][npar] = (*McTruth_tid)[j];
          m_ptid[n_level+1][npar] = (*McTruth_ptid)[j];
          m_pid[n_level+1][npar] = (*McTruth_particleName)[j];
          m_prid[n_level+1][npar] = (*McTruth_process)[j];
          m_e[n_level+1][npar] = (*McTruth_e)[j];
					double px= (*McTruth_px)[j];
					double py= (*McTruth_py)[j];
					double pz= (*McTruth_pz)[j];
					double p = sqrt(px*px+py*py+pz*pz);
					m_p[n_level+1][npar] = p;
          n_par[n_level+1] += 1;
          count++;
					if ((*McTruth_process)[j] == ProcessWanted ){
						isProcessWanted = true;
						if ( (*McTruth_pid)[j] > NucleusThre){
							hasNucleus = true;
						}
						if ( (*McTruth_particleName)[j] == ParticleWanted ){
							v_parMom.push_back(p);
						}
					}
        }
      }
    }
    n_level++;
    if ( count < McTruth_nTracks )
      goto loop;
    n_level++;
    if ( McTruth_nTracks != 1 ){

      fout<<"********************Event "<<iEvent<<"**************************"<<std::endl;
      Int_t i_level = 0;
      Int_t i_par[400];
      for ( int i = 0; i < 400; i ++ ){
        i_par[i] = 0;
      }
      Int_t cur_par = 0;
      double px= (*McTruth_px)[0];
      double py= (*McTruth_py)[0];
      double pz= (*McTruth_pz)[0];
      double p = sqrt(px*px+py*py+pz*pz);
      fout<<"|_pid="<<m_pid[0][0]<<", tid="<<1<<", prid="<<-1<<", momentum="<<m_p[0][0]*scale<<"MeV, energy="<<m_e[0][0]*scale<<"MeV"<<std::endl;
      i_par[0] += 1;
      Int_t cur_ptid;
      Int_t up_tid;
loop2:  cur_ptid = m_ptid[i_level+1][cur_par];
      up_tid = m_tid[i_level][i_par[i_level]-1];
      if ( cur_ptid == up_tid ){
        for ( Int_t i = 0; i <= i_level; i++ ){
          fout<<"   |";
        }
        fout<<"_pid="<<m_pid[i_level+1][cur_par]<<", tid="<<m_tid[i_level+1][cur_par]<<", prid="<<m_prid[i_level+1][cur_par]<<", momentum="<<m_p[i_level+1][cur_par]*scale<<"MeV, energy="<<m_e[i_level+1][cur_par]*scale<<"MeV"<<std::endl;
        i_par[i_level+1] += 1;
        if ( i_level + 1 == n_level - 1 ){
          if ( cur_par == n_par[i_level + 1] - 1 ){
            if (i_level == 0)
              goto ftracksh;
            i_level--;
            while ( i_par[i_level+1] == n_par[i_level+1] ){
              if (i_level == 0)
                goto ftracksh;
              i_level--;
            }
            cur_par = i_par[i_level+1];
            goto loop2;
          }
          else{
            cur_par++;
            goto loop2;
          }
        }
        else{
          i_level++;
          while ( i_par[i_level+1] == n_par[i_level+1] ){
            if (i_level == 0)
              goto ftracksh;
            i_level--;
          }
          cur_par = i_par[i_level+1];
          goto loop2;
        }
      }
      else{
        if ( cur_par == n_par[i_level + 1] - 1 ){
          if (i_level == 0)
            goto wrong;
          i_level--;
          cur_par = i_par[i_level+1];
          goto loop2;
        }
        else{
          cur_par++;
          goto loop2;
        }
      }
wrong: fout<<"Wrong!!!"<<std::endl;
ftracksh: fout<<"Done!!!"<<std::endl;
    }
    if (isProcessWanted){
    	N1++;
			if (!hasNucleus){
				N2++;
				for ( int i = 0; i < v_parMom.size(); i++ ){
					h1D1->Fill(v_parMom[i]*scale);
				}
			}
			else{
				for ( int i = 0; i < v_parMom.size(); i++ ){
					h1D2->Fill(v_parMom[i]*scale);
				}
			}
		}
  }
	fout<<"Totally "<<N1<<" events have a vertex of type \""<<ProcessWanted<<"\""<<std::endl;
	fout<<"  Among them, "<<N2<<" events don't have a Nucleus"<<std::endl;
  TCanvas*c1 = new TCanvas("c1","c1");
  h1D1->Draw();
  TCanvas*c2 = new TCanvas("c2","c2");
  h1D2->Draw();
}
