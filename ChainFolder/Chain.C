#include <iostream>
#include <sstream>

#include <cstdlib>

#include "TChain.h"
#include "TTree.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "Math/DistFunc.h"

int main(int argc, char* argv[]){
	if ( argc < 3 ){
		std::cout<<"Please indicate at least: Name number"<<std::endl;
		return 0;
	}
	std::stringstream buff;

	int nFiles;
	buff.str("");
	buff.clear();
	buff<<argv[2];
	buff>>nFiles;

	std::string FileName;
	buff.str("");
	buff.clear();
	buff<<argv[1];
	FileName = buff.str();

	std::string TreeName;
	if ( argc >= 4 ){
		buff.str("");
		buff.clear();
		buff<<argv[3];
		TreeName = buff.str();
	}
	else TreeName = "T";

	TFile *file = new TFile("output.root","RECREATE");
	TChain *ch = new TChain(TreeName.c_str());

	for (int i = 1; i <= nFiles; i ++){
		buff.str("");
		buff.clear();
		if ( i < 10 ){
			buff<<FileName<<"_0"<<i<<".root";
		}
		else{
			buff<<FileName<<"_"<<i<<".root";
		}
		ch->Add(buff.str().c_str());
		std::cout<<buff.str()<<std::endl;
	}
	std::cout<<__LINE__<<std::endl;
	TTree *m_tree = ch->CloneTree();
	std::cout<<__LINE__<<std::endl;
	file->Write();
	std::cout<<__LINE__<<std::endl;
	//file->Close();
	delete file;
	std::cout<<__LINE__<<std::endl;
	return 0;
}
