
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
void ana(){
  std::string filename;
  std::string treename;
  std::string rootfile;
  int module = 1000;

  filename = "temp.txt";
  treename = "t";
  rootfile = "out.root";
  TFile file_output( rootfile.c_str(), "RECREATE" );
  TTree* d_tree = new TTree( treename.c_str(), treename.c_str() );
  double x1;
  d_tree->Branch("x1", &x1, "x1/D");
  double y1;
  d_tree->Branch("y1", &y1, "y1/D");
  double z1;
  d_tree->Branch("z1", &z1, "z1/D");
  double x2;
  d_tree->Branch("x2", &x2, "x2/D");
  double y2;
  d_tree->Branch("y2", &y2, "y2/D");
  double z2;
  d_tree->Branch("z2", &z2, "z2/D");
  FILE* fin = fopen(filename.c_str(), "r");
  if(!fin){
    std::cout<<"Cannot find "<<filename<<std::endl;
  }
  int iline = 0;
  char line[512];
  while(fgets(line,sizeof(line),fin)){
    if(iline++ == 0) continue;
    if(fmod(iline,module) == 0) std::cout<<"line: "<<iline<<std::endl;
    sscanf(line," %lf %lf %lf %lf %lf %lf",&x1,&y1,&z1,&x2,&y2,&z2);
    d_tree->Fill();
  }


  fclose(fin);
  d_tree-> Write();
  file_output.Close();

}
