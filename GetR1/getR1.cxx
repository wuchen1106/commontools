#include <iostream>
#include <vector>
#include "TChain.h"

int main(int argc, char** argv){
	TChain * chain = new TChain("tree","tree");
	double R1;
	chain->SetBranchAddress("R1",&R1);
	chain->Add(argv[1]);
	std::vector<double> vr1;
	std::vector<int> nr1;
	Long64_t nEntries = chain->GetEntries();
	bool found;
	for (Long64_t iEntry= 0; iEntry<nEntries; iEntry++){
		chain->GetEntry(iEntry);
		found = false;
		for (Long64_t i = 0; i<vr1.size(); i++){
			if (vr1[i]==R1){
				nr1[i]++;
				found=true;
				break;
			}
		}
		if (!found){
			vr1.push_back(R1);
			nr1.push_back(1);
		}
	}
	for (Long64_t i = 0; i<vr1.size(); i++){
		if (nr1[i]>1){
			std::cout<<vr1[i]<<" "<<nr1[i]<<std::endl;
		}
	}
	return 0;
}
