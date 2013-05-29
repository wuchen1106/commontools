#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

#include <iostream>
#include <fstream>
#include <string>

void ana(){
	std::string histo_for_mom_filename = "BEStudy";
	std::string full_infile_name = "BEStudy.txt";
  std::string full_outfile_name = "out.root";
  int histo_for_mom_bin = 10;
  int histo_for_mom_left = 65;
  int histo_for_mom_right = 130;
  int histo_for_mom_2_bin = 10;
  int histo_for_mom_2_left = 0;
  int histo_for_mom_2_right = 3.1416;
	TFile* ft = new TFile(full_outfile_name.c_str(),"RECREATE");
  FILE* fp = fopen(full_infile_name.c_str(),"r");
  if (fp==NULL) {
    std::cout<<"ERROR: Can not find file: "<<full_infile_name<<"!!!"<<std::endl;
    return;
  }
  TH1D* histo_for_mom = new TH1D(histo_for_mom_filename.c_str(), histo_for_mom_filename.c_str(), histo_for_mom_bin, histo_for_mom_left, histo_for_mom_right );
  TH2D* histo_for_mom_2 = new TH2D(histo_for_mom_filename.c_str(), histo_for_mom_filename.c_str(), histo_for_mom_bin, histo_for_mom_left, histo_for_mom_right, histo_for_mom_2_bin, histo_for_mom_2_left, histo_for_mom_2_right  );

  char line[128];
  double mom,mom2,cont;
  while (fgets(line,sizeof(line),fp)) {
    sscanf(line,"%lf %lf %lf",&mom,&mom2,&cont);
    histo_for_mom_2->Fill(mom,mom2,cont);
		std::cout<<"Fill("<<mom<<", "<<mom2<<", "<<cont<<")"<<std::endl;
  }
  fclose(fp);
  histo_for_mom_2->Write();
}
