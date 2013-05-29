
#include <iostream>
#include <fstream>
#include <sstream>

#include <TChain.h>

#include "McTopAnalysis.h"

#include <sstream>
#include <cstdio>
#include <string>
#include "stdint.h"

using namespace std;

void testmctop()
{
  McTopAnalysis::readPntFile( "pnt" );


  std::string tree_name = "t";
  std::string file_name = "RPC002_raw.root";
  std::string file_dir = "/home/wuchen/workarea/Simulate/CometPhaseI/data/";
  std::string full_file_name = file_dir + file_name;

  TChain* chain = new TChain(tree_name.c_str());
  chain->Add(full_file_name.c_str());

  int dataType;

  int psippDataNum;
  int psippMcTopData[128];
  uint32_t psippMd5[4];
  int dpDataNum;
  int dpMcTopData[128];
  uint32_t dpMd5[4];
  int dmDataNum;
  int dmMcTopData[128];
  uint32_t dmMd5[4];

  Int_t nMcParticle = 0;
  Int_t vMcParticleId[100];
  Int_t vMcMoParticleId[100];
  Int_t vMcMoMoParticleId[100];
  Double_t vMcParticleInitialR[100];
  Double_t vMcParticleInitialPhi[100];
  Double_t vMcParticleInitialTheta[100];
  Double_t vMcParticleInitialT[100];
  Double_t vMcParticleFinalR[100];
  Double_t vMcParticleFinalPhi[100];
  Double_t vMcParticleFinalTheta[100];
  Double_t vMcParticleFinalT[100];
  Double_t vMcParticleP[100];
  Double_t vMcParticleE[100];
  Double_t vMcParticlePhi[100];
  Double_t vMcParticleTheta[100];
  Double_t vMcParticleM[100];

  Int_t nNeutral = 0;
  Int_t vNeutralTrackId[100];
  Int_t vNeutralModule[100];
  Int_t vNeutralStatus[100];
  Double_t vNeutralDE[100];
  Double_t vNeutralE[100];
  Double_t vNeutralPhi[100];
  Double_t vNeutralTheta[100];
  Double_t vNeutralX[100];
  Double_t vNeutralY[100];
  Double_t vNeutralZ[100];
  Double_t vNeutralTime[100];

  Int_t nNeutralMatch = 0;
  Double_t mNeutralPMatch[10000];

  int evtNo;
  int runNo;

  chain->SetBranchAddress("dataType", &dataType);

  chain->SetBranchAddress("psippDataNum", &psippDataNum);
  chain->SetBranchAddress("psippMcTopData", psippMcTopData);
  chain->SetBranchAddress("psippMd5", psippMd5);
  chain->SetBranchAddress("dpDataNum", &dpDataNum);
  chain->SetBranchAddress("dpMcTopData", dpMcTopData);
  chain->SetBranchAddress("dpMd5", dpMd5);
  chain->SetBranchAddress("dmDataNum", &dmDataNum);
  chain->SetBranchAddress("dmMcTopData", dmMcTopData);
  chain->SetBranchAddress("dmMd5", dmMd5);

  chain->SetBranchAddress("nMcParticle",&nMcParticle);
  chain->SetBranchAddress("vMcParticleId",vMcParticleId);
  chain->SetBranchAddress("vMcMoParticleId",vMcMoParticleId);
  chain->SetBranchAddress("vMcMoMoParticleId",vMcMoMoParticleId);
  chain->SetBranchAddress("vMcParticleInitialR",vMcParticleInitialR);
  chain->SetBranchAddress("vMcParticleInitialPhi",vMcParticleInitialPhi);
  chain->SetBranchAddress("vMcParticleInitialTheta",vMcParticleInitialTheta);
  chain->SetBranchAddress("vMcParticleInitialT",vMcParticleInitialT);
  chain->SetBranchAddress("vMcParticleFinalR",vMcParticleFinalR);
  chain->SetBranchAddress("vMcParticleFinalPhi",vMcParticleFinalPhi);
  chain->SetBranchAddress("vMcParticleFinalTheta",vMcParticleFinalTheta);
  chain->SetBranchAddress("vMcParticleFinalT",vMcParticleFinalT);
  chain->SetBranchAddress("vMcParticleP",vMcParticleP);
  chain->SetBranchAddress("vMcParticleE",vMcParticleE);
  chain->SetBranchAddress("vMcParticlePhi",vMcParticlePhi);
  chain->SetBranchAddress("vMcParticleTheta",vMcParticleTheta);
  chain->SetBranchAddress("vMcParticleM",vMcParticleM);
  
  chain->SetBranchAddress("nNeutral",&nNeutral);
  chain->SetBranchAddress("vNeutralTrackId",vNeutralTrackId);
  chain->SetBranchAddress("vNeutralModule",vNeutralModule);
  chain->SetBranchAddress("vNeutralStatus",vNeutralStatus);
  chain->SetBranchAddress("vNeutralDE",vNeutralDE);
  chain->SetBranchAddress("vNeutralE",vNeutralE);
  chain->SetBranchAddress("vNeutralPhi",vNeutralPhi);
  chain->SetBranchAddress("vNeutralTheta",vNeutralTheta);
  chain->SetBranchAddress("vNeutralX",vNeutralX);
  chain->SetBranchAddress("vNeutralY",vNeutralY);
  chain->SetBranchAddress("vNeutralZ",vNeutralZ);
  chain->SetBranchAddress("vNeutralTime",vNeutralTime);

  chain->SetBranchAddress("nNeutralMatch",&nNeutralMatch);
  chain->SetBranchAddress("mNeutralPMatch",mNeutralPMatch);

  chain->SetBranchAddress("evtNo", &evtNo);
  chain->SetBranchAddress("runNo", &runNo);

  McTopAnalysis mctopd0, mctopd0bar, mctopdp, mctopdm, mctoppsipp;

  Long64_t nEvent = chain->GetEntries();
  std::cout << "Entries: " << nEvent << std::endl << std::endl;;
  for( Long64_t i=0; i<nEvent; i++ )
  {

    if ( i % 1000 == 0 ){
      cout<<" i = "<<i<<endl;
    }

    chain->GetEntry(i);

    for ( int i = 0; i < nMcParticle; i++ ){
      if ( vMcParticleE[i] < 0.029791 || vMcParticleE[i] > 0.029792 ){
        continue;
      }

      mctoppsipp.add( psippDataNum, psippMcTopData, (const int*)psippMd5 );
      switch( dataType )
      {
        case 1:
          mctopdp.add( dpDataNum, dpMcTopData, (const int*)dpMd5 );
          mctopdm.add( dmDataNum, dmMcTopData, (const int*)dmMd5 );
          break;
        case 2:
          mctopd0.add   ( dpDataNum, dpMcTopData, (const int*)dpMd5 );
          mctopd0bar.add( dmDataNum, dmMcTopData, (const int*)dmMd5 );
          break;
        case 3:
          break;
        default:
          break;
      }

    }
  }

  // sort all the decay
  mctopdp.sort();
  mctopdm.sort();
  mctopd0.sort();
  mctopd0bar.sort();
  mctoppsipp.sort();

  std::ofstream fall( "output/generic_all" );
  fall << "psi(3770) decay lists: " << std::endl;
  mctoppsipp.print( fall );
  fall.close();

  std::ofstream fd0( "output/generic_d0" );
  fd0 << "D0 decay lists: " << std::endl;
  mctopd0.print( fd0 );
  fd0.close();

  std::ofstream fd0bar( "output/generic_d0bar" );
  fd0bar << "D0bar decay lists: " << std::endl;
  mctopd0bar.print( fd0bar );
  fd0bar.close();

  std::ofstream fdp( "output/generic_dp" );
  fdp << "D+ decay lists: " << std::endl;
  mctopdp.print( fdp );
  fdp.close();

  std::ofstream fdm( "output/generic_dm" );
  fdm << "D- decay lists: " << std::endl;
  mctopdm.print( fdm );
  fdm.close();

  mctopd0.clearFinalParticle();
  mctopd0.addFinalParticle( "gamma" );
  mctopd0.addFinalParticle( "gamma" );
  mctopd0.addFinalParticle( "gamma" );
  mctopd0.addFinalParticle( "gamma" );
  mctopd0.searchFinalStates();
  std::ofstream fd02pi0( "output/generic_d02pi0" );
  mctopd0.printFinalStates( fd02pi0 );
  fd02pi0.close();

  mctopd0bar.clearFinalParticle();
  mctopd0bar.addFinalParticle( "gamma" );
  mctopd0bar.addFinalParticle( "gamma" );
  mctopd0bar.addFinalParticle( "gamma" );
  mctopd0bar.addFinalParticle( "gamma" );
  mctopd0bar.searchFinalStates();
  std::ofstream fd0bar2pi0( "output/generic_d0bar2pi0" );
  mctopd0bar.printFinalStates( fd0bar2pi0 );
  fd0bar2pi0.close();

//  ofstream haha( "output/haha.tex" );
//  mctopd0.latex( haha );
//  haha.close();

#if 0
  ofstream fkpipi( "output/generic_kpipi" );
  mctopdp.clearFinalParticle();
  mctopdp.addFinalParticle( "K-" );
  mctopdp.addFinalParticle( "pi+" );
  mctopdp.addFinalParticle( "pi+" );
  mctopdp.searchFinalStates();
  mctopdp.printFinalStates( fkpipi );
  mctopdm.clearFinalParticle();
  mctopdm.addFinalParticle( "K+" );
  mctopdm.addFinalParticle( "pi-" );
  mctopdm.addFinalParticle( "pi-" );
  mctopdm.searchFinalStates();
  mctopdm.printFinalStates( fkpipi );
  fkpipi.close();

  ofstream fkpipipi0( "output/generic_kpipipi0" );
  mctopdp.clearFinalParticle();
  mctopdp.addFinalParticle( "K-" );
  mctopdp.addFinalParticle( "pi+" );
  mctopdp.addFinalParticle( "pi+" );
  mctopdp.addFinalParticle( "gamma" );
  mctopdp.addFinalParticle( "gamma" );
  mctopdp.searchFinalStates();
  mctopdp.printFinalStates( fkpipipi0 );
  mctopdm.clearFinalParticle();
  mctopdm.addFinalParticle( "K+" );
  mctopdm.addFinalParticle( "pi-" );
  mctopdm.addFinalParticle( "pi-" );
  mctopdm.addFinalParticle( "gamma" );
  mctopdm.addFinalParticle( "gamma" );
  mctopdm.searchFinalStates();
  mctopdm.printFinalStates( fkpipipi0 );
  fkpipipi0.close();

  ofstream fkspi( "output/generic_kspi" );
  mctopdp.clearFinalParticle();
  mctopdp.addFinalParticle( "pi+" );
  mctopdp.addFinalParticle( "pi+" );
  mctopdp.addFinalParticle( "pi-" );
  mctopdp.searchFinalStates();
  mctopdp.printFinalStates( fkspi );
  mctopdm.clearFinalParticle();
  mctopdm.addFinalParticle( "pi+" );
  mctopdm.addFinalParticle( "pi-" );
  mctopdm.addFinalParticle( "pi-" );
  mctopdm.searchFinalStates();
  mctopdm.printFinalStates( fkspi );
  fkspi.close();

  ofstream fkspipi0( "output/generic_kspipi0" );
  mctopdp.clearFinalParticle();
  mctopdp.addFinalParticle( "pi+" );
  mctopdp.addFinalParticle( "pi+" );
  mctopdp.addFinalParticle( "pi-" );
  mctopdp.addFinalParticle( "gamma" );
  mctopdp.addFinalParticle( "gamma" );
  mctopdp.searchFinalStates();
  mctopdp.printFinalStates( fkspipi0 );
  mctopdm.clearFinalParticle();
  mctopdm.addFinalParticle( "pi+" );
  mctopdm.addFinalParticle( "pi-" );
  mctopdm.addFinalParticle( "pi-" );
  mctopdm.addFinalParticle( "gamma" );
  mctopdm.addFinalParticle( "gamma" );
  mctopdm.searchFinalStates();
  mctopdm.printFinalStates( fkspipi0 );
  fkspipi0.close();

  ofstream fkspipipi( "output/generic_kspipipi" );
  mctopdp.clearFinalParticle();
  mctopdp.addFinalParticle( "pi+" );
  mctopdp.addFinalParticle( "pi+" );
  mctopdp.addFinalParticle( "pi+" );
  mctopdp.addFinalParticle( "pi-" );
  mctopdp.addFinalParticle( "pi-" );
  mctopdp.searchFinalStates();
  mctopdp.printFinalStates( fkspipipi );
  mctopdm.clearFinalParticle();
  mctopdm.addFinalParticle( "pi+" );
  mctopdm.addFinalParticle( "pi+" );
  mctopdm.addFinalParticle( "pi-" );
  mctopdm.addFinalParticle( "pi-" );
  mctopdm.addFinalParticle( "pi-" );
  mctopdm.searchFinalStates();
  mctopdm.printFinalStates( fkspipipi );
  fkspipipi.close();

  ofstream fkkpi( "output/generic_kkpi" );
  mctopdp.clearFinalParticle();
  mctopdp.addFinalParticle( "K-" );
  mctopdp.addFinalParticle( "K+" );
  mctopdp.addFinalParticle( "pi+" );
  mctopdp.searchFinalStates();
  mctopdp.printFinalStates( fkkpi );
  mctopdm.clearFinalParticle();
  mctopdm.addFinalParticle( "K+" );
  mctopdm.addFinalParticle( "K-" );
  mctopdm.addFinalParticle( "pi-" );
  mctopdm.searchFinalStates();
  mctopdm.printFinalStates( fkkpi );
  fkkpi.close();
#endif
}

int main()
{
  testmctop();
  return 0;
}
