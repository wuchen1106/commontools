#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm> // for max_element

#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TLegend.h"

#include "MyRootInterface.hxx"

MyRootInterface::MyRootInterface(int verbose,bool backup)
:m_verbose(verbose),d_tree(0),m_TChain(0),m_backup(backup)
{
	Verbose_GeneralInfo = 5; //有哪些hist,什么时候输出了，参数如何
	prefix_GeneralInfo= "  [General] ";
	Verbose_HistInfo = 10; //有哪些hist,什么时候输出了，参数如何
	prefix_HistInfo= "  [Histograms] ";
	Verbose_FileInfo = 10; //有哪些FileList,都有多少file
	prefix_FileInfo="  [FileInfo] ";
	Verbose_InputInfo = 10; //有哪些Input
	prefix_InputInfo="  [InputInfo] ";
	Verbose_InitInfo = 10; //有哪些Init
	prefix_InitInfo="  [InitInfo] ";
	Verbose_BranchInfo = 20; //有哪些Branch
	prefix_BranchInfo="  [BranchInfo] ";
	// by default output files would be put into current directory
	OutputDir = ".";
	OutputName = "output";
	TreeName = "tree";
	oTreeName = "t";
}

MyRootInterface::~MyRootInterface(){
}

int MyRootInterface::read(std::string file){
	if (m_verbose >= Verbose_GeneralInfo) std::cout<<prefix_GeneralInfo<<"Initializing from \""<<file<<"\""<<std::endl;
	std::stringstream buff;
	vec_TBranch.clear();
	vec_TBranchName.clear();
	vec_TBranchType.clear();
	vec_TBranchIsVec.clear();
	vec_oTBranch.clear();
	vec_oTBranchName.clear();
	vec_oTBranchType.clear();
	vec_oTBranchIsVec.clear();
	vec_double.clear();
	vec_int.clear();
	vec_string.clear();
	DirNames.clear(); 
	RunNames.clear(); 
	NCPU.clear();
	beginCPU.clear();
	NJob.clear();
	ovec_double.clear();
	ovec_int.clear();
	ovec_string.clear();
	ovec_vecdouble.clear();
	ovec_vecint.clear();
	ovec_vecstring.clear();

	//=>Read file
	std::ifstream fin_card(file.c_str());
	if(!fin_card){
		std::cout<<"Cannot find "<<file<<std::endl;
		return -1;
	}
	std::string s_card;
	// read file
	while(getline(fin_card,s_card)){
		if ( ISEMPTY(s_card) ) continue;
		//if (m_verbose >= Verbose_InputInfo) std::cout<<prefix_InputInfo<<": \""<<s_card<<"\""<<std::endl;
		std::vector<std::string> segments;
		seperate_string(s_card,segments,'|');
		int iterator = 1;
		if ( segments[0] == "TH1D" ){
			if(iterator<segments.size()) nameForH1D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) titleForH1D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) xNameForH1D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) yNameForH1D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) bin1ForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) left1ForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) right1ForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) minxForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) minyForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) colorForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) compareForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) xlogForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) ylogForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) markerForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) normForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) drawOptForH1D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) useLegendForH1D.push_back(string2double(segments[iterator++])); else {useLegendForH1D.push_back(0);}
			if(iterator<segments.size()) legendNameForH1D.push_back(segments[iterator++]); else {legendNameForH1D.push_back("");}
			if(iterator<segments.size()) legendx1ForH1D.push_back(string2double(segments[iterator++])); else {legendx1ForH1D.push_back(0);}
			if(iterator<segments.size()) legendy1ForH1D.push_back(string2double(segments[iterator++])); else {legendy1ForH1D.push_back(0);}
			if(iterator<segments.size()) legendx2ForH1D.push_back(string2double(segments[iterator++])); else {legendx2ForH1D.push_back(0);}
			if(iterator<segments.size()) legendy2ForH1D.push_back(string2double(segments[iterator++])); else {legendy2ForH1D.push_back(0);}
			if(iterator<segments.size()) npadxForH1D.push_back(string2double(segments[iterator++])); else {npadxForH1D.push_back(1);}
			if(iterator<segments.size()) npadyForH1D.push_back(string2double(segments[iterator++])); else {npadyForH1D.push_back(1);}
			if(iterator<segments.size()) sepForH1D.push_back(string2double(segments[iterator++])); else {sepForH1D.push_back(0);}
			int i = nameForH1D.size() - 1;
			if (m_verbose >= Verbose_InputInfo)
				std::cout<<prefix_InputInfo
				         <<"Input vecH1D["<<i<<"]: "<<nameForH1D[i]
				         <<", "<<titleForH1D[i]
				         <<", "<<xNameForH1D[i]
				         <<", "<<yNameForH1D[i]
				         <<", "<<bin1ForH1D[i]
				         <<", "<<left1ForH1D[i]
				         <<", "<<right1ForH1D[i]
				         <<", minx="<<minxForH1D[i]
				         <<", miny="<<minyForH1D[i]
				         <<", Color="<<colorForH1D[i]
				         <<", xlogSyle="<<xlogForH1D[i]
				         <<", ylogSyle="<<ylogForH1D[i]
				         <<", nCompare="<<compareForH1D[i]
				         <<", markerStyle="<<markerForH1D[i]
				         <<", normalize ="<<normForH1D[i]
				         <<", drawOpt=\""<<drawOptForH1D[i]
				         <<", useLegend=\""<<useLegendForH1D[i]
				         <<", legendName=\""<<legendNameForH1D[i]
				         <<", legendx1=\""<<legendx1ForH1D[i]
				         <<", legendy1=\""<<legendy1ForH1D[i]
				         <<", legendx2=\""<<legendx2ForH1D[i]
				         <<", legendy2=\""<<legendy2ForH1D[i]
				         <<", npadx=\""<<npadxForH1D[i]
				         <<", npady=\""<<npadyForH1D[i]
				         <<", sep=\""<<sepForH1D[i]
				         <<"\""<<std::endl;
		}
		else if ( segments[0] == "TH2D" ){
			if(iterator<segments.size()) nameForH2D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) titleForH2D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) xNameForH2D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) yNameForH2D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) bin1ForH2D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) left1ForH2D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) right1ForH2D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) bin2ForH2D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) left2ForH2D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) right2ForH2D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			int i = nameForH2D.size() - 1;
			if (m_verbose >= Verbose_InputInfo) std::cout<<prefix_InputInfo<<"Input vecH2D["<<i<<"]: "<<nameForH2D[i]<<", "<<titleForH2D[i]<<", "<<xNameForH2D[i]<<", "<<yNameForH2D[i]<<", "<<bin1ForH2D[i]<<", "<<left1ForH2D[i]<<", "<<right1ForH2D[i]<<", "<<bin2ForH2D[i]<<", "<<left2ForH2D[i]<<", "<<right2ForH2D[i]<<std::endl;
		}
		else if ( segments[0] == "FILE" ){
			if(iterator<segments.size()) DirNames.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			std::string runname;
			if(iterator<segments.size()) runname = "_"+segments[iterator++]; else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if (runname=="_"){
				RunNames.push_back("");
			}
			else{
				RunNames.push_back(runname);
			}
			if(iterator<segments.size()) {NCPU.push_back(string2double(segments[iterator++]));beginCPU.push_back(0);} else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) NJob.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			int i = DirNames.size() - 1;
			if ( m_verbose >= Verbose_InputInfo) std::cout<<prefix_InputInfo<<"Input FILE["<<i<<"]: "<<DirNames[i]<<"\" with runname = \""<<RunNames[i]<<"\" has "<<NJob[i]<<" jobs on "<<NCPU[i]<<" CPUs starting from "<<beginCPU[i]<<std::endl;
		}
		else if ( segments[0] == "TGraph" ){
			if(iterator<segments.size()) nameForGraph.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) titleForGraph.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) xNameForGraph.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) yNameForGraph.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			std::vector<double> avec;
			xForGraph.push_back(avec);
			std::vector<double> bvec;
			yForGraph.push_back(bvec);
			if(iterator<segments.size()) colorForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) compareForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) minxForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) maxxForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) minyForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) xlogForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) ylogForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) markerForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) drawOptForGraph.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			int i = nameForGraph.size() - 1;
			if (m_verbose >= Verbose_InputInfo) std::cout<<prefix_InputInfo<<"Input vecGraph["<<i<<"]: "<<nameForGraph[i]<<", "<<titleForGraph[i]<<", "<<xNameForGraph[i]<<", "<<yNameForGraph[i]<<", Color="<<colorForGraph[i]<<", xlogSyle="<<xlogForGraph[i]<<", ylogSyle="<<ylogForGraph[i]<<", nCompare="<<compareForGraph[i]<<", markerStyle="<<markerForGraph[i]<<", drawOpt=\""<<drawOptForGraph[i]<<"\""<<std::endl;
		}
		else if (segments[0] == "refTH1D"){
			if(iterator<segments.size()) refFileName.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) refHistName.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			int i = refFileName.size() - 1;
			if (m_verbose >= Verbose_InputInfo) std::cout<<prefix_InputInfo<<"Input refTH1D["<<i<<"]: "<<refFileName[i]<<", "<<refHistName[i]<<std::endl;
		}
		else if (segments[0] == "oFILE"){
			if(iterator<segments.size()) oFileName.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			int i = oFileName.size() - 1;
			if ( m_verbose >= Verbose_InputInfo) std::cout<<prefix_InputInfo<<"Input oFILE["<<i<<"]: "<<oFileName[i]<<std::endl;
		}
		else if (segments[0] == "TreeName"){
			if(iterator<segments.size()) TreeName = (segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if ( m_verbose >= Verbose_InputInfo) std::cout<<prefix_InputInfo<<"Set TreeName to \""<<TreeName<<"\""<<std::endl;
		}
		else if (segments[0] == "oTreeName"){
			if(iterator<segments.size()) oTreeName = (segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if ( m_verbose >= Verbose_InputInfo) std::cout<<prefix_InputInfo<<"Set oTreeName to \""<<oTreeName<<"\""<<std::endl;
		}
		else if (segments[0] == "TBranch"){
			if(iterator<segments.size()) vec_TBranchName.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) vec_TBranchType.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) vec_TBranchIsVec.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			vec_TBranch.push_back(0);
			vec_double.push_back(0);
			vec_int.push_back(0);
			vec_string.push_back("");
			vec_vecdouble.push_back(0);
			vec_vecint.push_back(0);
			vec_vecstring.push_back(0);
			int i = vec_TBranchName.size() - 1;
			if (m_verbose >= Verbose_InputInfo) std::cout<<prefix_InputInfo<<"Input TBranch["<<i<<"]: "<<vec_TBranchName[i]<<", "<<vec_TBranchType[i]<<", "<<vec_TBranchIsVec[i]<<std::endl;
		}
		else if (segments[0] == "oTBranch"){
			if(iterator<segments.size()) vec_oTBranchName.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) vec_oTBranchType.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) vec_oTBranchIsVec.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			vec_oTBranch.push_back(0);
			ovec_double.push_back(0);
			ovec_int.push_back(0);
			int i = vec_oTBranchName.size() - 1;
			ovec_string.resize(i+1);
			ovec_vecdouble.resize(i+1);
			ovec_vecint.resize(i+1);
			ovec_vecstring.resize(i+1);
			if (m_verbose >= Verbose_InputInfo) std::cout<<prefix_InputInfo<<"Input oTBranch["<<i<<"]: "<<vec_oTBranchName[i]<<", "<<vec_oTBranchType[i]<<", "<<vec_oTBranchIsVec[i]<<std::endl;
		}
		else{
			std::cout<<"Cannot recogonize this line: "<<s_card<<std::endl;
			continue;
		}
	}
}

int MyRootInterface::init(){
	init_hist();
	init_file();
}

int MyRootInterface::init_hist(){
	int index_temp = 0;
	//=> Get histograms in
	for ( int i = 0; i < nameForH2D.size(); i++ ){
		if (m_verbose >= Verbose_InitInfo) std::cout<<prefix_InitInfo<<"Init vecH2D["<<i<<"]: "<<nameForH2D[i]<<", "<<titleForH2D[i]<<", "<<xNameForH2D[i]<<", "<<yNameForH2D[i]<<", "<<bin1ForH2D[i]<<", "<<left1ForH2D[i]<<", "<<right1ForH2D[i]<<", "<<bin2ForH2D[i]<<", "<<left2ForH2D[i]<<", "<<right2ForH2D[i]<<std::endl;
		vecH2D.push_back(new TH2D(nameForH2D[i].c_str(),titleForH2D[i].c_str(),bin1ForH2D[i],left1ForH2D[i],right1ForH2D[i],bin2ForH2D[i],left2ForH2D[i],right2ForH2D[i]) );
	}
	for ( int i = 0; i < nameForH1D.size(); i++ ){
		if (m_verbose >= Verbose_InitInfo)
			std::cout<<prefix_InitInfo
					 <<"Init vecH1D["<<i<<"]: "<<nameForH1D[i]
					 <<", "<<titleForH1D[i]
					 <<", "<<xNameForH1D[i]
					 <<", "<<yNameForH1D[i]
					 <<", "<<bin1ForH1D[i]
					 <<", "<<left1ForH1D[i]
					 <<", "<<right1ForH1D[i]
					 <<", minx="<<minxForH1D[i]
					 <<", miny="<<minyForH1D[i]
					 <<", Color="<<colorForH1D[i]
					 <<", xlogSyle="<<xlogForH1D[i]
					 <<", ylogSyle="<<ylogForH1D[i]
					 <<", nCompare="<<compareForH1D[i]
					 <<", markerStyle="<<markerForH1D[i]
					 <<", normalize ="<<normForH1D[i]
					 <<", drawOpt=\""<<drawOptForH1D[i]
					 <<", useLegend=\""<<useLegendForH1D[i]
					 <<", legendName=\""<<legendNameForH1D[i]
					 <<", legendx1=\""<<legendx1ForH1D[i]
					 <<", legendy1=\""<<legendy1ForH1D[i]
					 <<", legendx2=\""<<legendx2ForH1D[i]
					 <<", legendy2=\""<<legendy2ForH1D[i]
					 <<", npadx=\""<<npadxForH1D[i]
					 <<", npady=\""<<npadyForH1D[i]
					 <<", sep=\""<<sepForH1D[i]
					 <<"\""<<std::endl;
		vecH1D.push_back(new TH1D(nameForH1D[i].c_str(),titleForH1D[i].c_str(),bin1ForH1D[i],left1ForH1D[i],right1ForH1D[i]) );
	}
	for ( int i = 0; i < refFileName.size(); i++ ){
		TFile * fp_ref = new TFile(refFileName[i].c_str());
		if (fp_ref==NULL) {
			std::cout<<"ERROR: Can not find file: "<<refFileName[i]<<"!!!"<<std::endl;
			continue;
		}
		TH1D* h1_ref = (TH1D*)fp_ref->Get(refHistName[i].c_str());
		if(h1_ref==NULL){
			std::cout<<"ERROR: Can not find histogram \""<<refHistName[i]<<"\"in file : "<<refFileName[i]<<"!!!"<<std::endl;
			continue;
		}
		if ( (index_temp = get_TH1D_index(refHistName[i])) != -1 ){
			if (ISEMPTY(titleForH1D[index_temp])) titleForH1D[index_temp] = h1_ref->GetTitle();
			if (ISEMPTY(xNameForH1D[index_temp])) xNameForH1D[index_temp] = h1_ref->GetXaxis()->GetTitle();
			if (ISEMPTY(yNameForH1D[index_temp])) yNameForH1D[index_temp] = h1_ref->GetYaxis()->GetTitle();
			h1_ref->SetTitle(titleForH1D[index_temp].c_str());
			vecH1D[index_temp]=h1_ref;
		}
		else{
			std::cout<<"ERROR: Can not find histogram \""<<refHistName[i]<<"\"in vecH1D!!!"<<std::endl;
			continue;
		}
	}
}

int MyRootInterface::init_file(){
	std::stringstream buff;
	//=> Get Tree in
	m_TChain = new TChain(TreeName.c_str());
	for ( int iFile = 0; iFile < DirNames.size(); iFile++ ){
		if ( m_verbose >= Verbose_FileInfo) std::cout<<prefix_FileInfo<<"Adding FileList \""<<DirNames[iFile]<<"\" with runname = \""<<RunNames[iFile]<<"\" has "<<NJob[iFile]<<" jobs on "<<NCPU[iFile]<<" CPUs starting from "<<beginCPU[iFile]<<std::endl;
		for (int i = beginCPU[iFile]; i < beginCPU[iFile] + NCPU[iFile]; i ++){
			int iStart = 0;
			for (int j = iStart; j < iStart + NJob[iFile]; j ++){
				buff.str("");
				buff.clear();
				buff<<DirNames[iFile]<<"/"<<i<<"_job"<<j<<RunNames[iFile]<<".raw";
				m_TChain->Add(buff.str().c_str());
			}
		}
	}
	for ( int iFile = 0; iFile < oFileName.size(); iFile++ ){
		if ( m_verbose >= Verbose_FileInfo) std::cout<<prefix_FileInfo<<"Adding oFile \""<<oFileName[iFile]<<"\""<<std::endl;
		m_TChain->Add(oFileName[iFile].c_str());
	}

	//=> Set tree for input
	for ( int i_TB = 0; i_TB < vec_TBranchName.size(); i_TB++ ){
		int isvec = vec_TBranchIsVec[i_TB];
		int type = vec_TBranchType[i_TB];
		if (isvec){
			if (type == 0) m_TChain->SetBranchAddress(vec_TBranchName[i_TB].c_str(), &vec_vecdouble[i_TB], &vec_TBranch[i_TB]);
			else if (type == 1) m_TChain->SetBranchAddress(vec_TBranchName[i_TB].c_str(), &vec_vecint[i_TB], &vec_TBranch[i_TB]);
			else if (type == 2) m_TChain->SetBranchAddress(vec_TBranchName[i_TB].c_str(), &vec_vecstring[i_TB], &vec_TBranch[i_TB]);
		}
		else{
			if (type == 0) m_TChain->SetBranchAddress(vec_TBranchName[i_TB].c_str(), &vec_double[i_TB]);
			else if (type == 1) m_TChain->SetBranchAddress(vec_TBranchName[i_TB].c_str(), &vec_int[i_TB]);
			else if (type == 2) m_TChain->SetBranchAddress(vec_TBranchName[i_TB].c_str(), &vec_string[i_TB]);
		}
	}

	//=> Set tree for output
	d_tree = new TTree( oTreeName.c_str(), oTreeName.c_str() );
	for ( int i_TB = 0; i_TB < vec_oTBranchName.size(); i_TB++ ){
		int isvec = vec_oTBranchIsVec[i_TB];
		int type = vec_oTBranchType[i_TB];
		if (isvec){
			if (type == 0) d_tree->Branch(vec_oTBranchName[i_TB].c_str(), &ovec_vecdouble[i_TB]);
			else if (type == 1) d_tree->Branch(vec_oTBranchName[i_TB].c_str(), &ovec_vecint[i_TB]);
			else if (type == 2) d_tree->Branch(vec_oTBranchName[i_TB].c_str(), &ovec_vecstring[i_TB]);
		}
		else{
			if (type == 0) d_tree->Branch(vec_oTBranchName[i_TB].c_str(), &ovec_double[i_TB]);
			else if (type == 1) d_tree->Branch(vec_oTBranchName[i_TB].c_str(), &ovec_int[i_TB]);
			else if (type == 2) d_tree->Branch(vec_oTBranchName[i_TB].c_str(), &ovec_string[i_TB]);
		}
	}
}

int MyRootInterface::GetEntry(Long64_t iEvent){
	if ( m_verbose >= Verbose_BranchInfo)
		std::cout<<prefix_BranchInfo<<" m_TChain @ ("<<(void*)m_TChain<<")"<<std::endl;
	Long64_t tentry = m_TChain->LoadTree(iEvent);
	if ( m_verbose >= Verbose_BranchInfo)
		std::cout<<prefix_BranchInfo<<" tentry = LoadTree("<<iEvent<<") = "<<tentry<<std::endl;
	for ( int i_TB = 0; i_TB < vec_TBranchName.size(); i_TB++ ){
		if ( m_verbose >= Verbose_BranchInfo)
			std::cout<<prefix_BranchInfo<<"Setting Branch["<<i_TB<<"]: "
			                            <<vec_TBranchName[i_TB]<<", @("
			                            <<(void *)&vec_TBranch[i_TB]<<") "
			                            <<std::endl;
		if (vec_TBranch[i_TB]) vec_TBranch[i_TB]->GetEntry(tentry);
	}
	if ( m_verbose >= Verbose_BranchInfo)
		std::cout<<prefix_BranchInfo<<" m_TChain->LoadTree("<<iEvent<<")"<<std::endl;
	m_TChain->GetEntry(iEvent);
	if ( m_verbose >= Verbose_BranchInfo)
		std::cout<<prefix_BranchInfo<<" Finished"<<std::endl;
}

int MyRootInterface::dump(){
	//=======================================================================================================
	//************WRITE AND OUTPUT********************
	std::string outputFileName = OutputDir + "/" + OutputName + ".root";
	if (m_verbose >= Verbose_GeneralInfo) std::cout<<prefix_GeneralInfo<<"Creating output file \""<<outputFileName<<"\""<<std::endl;
	TFile *file = new TFile(outputFileName.c_str(),"RECREATE");
	std::stringstream buff;
	int ww = 1440;
	int wh = 900;

	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	//  gStyle->SetTitleW(0.99);
	//  gStyle->SetTitleH(0.08);
	//Output these histograms
	std::vector<TPad*> padList;
	int ipad = 0;
	TCanvas* c = 0;
	for ( int i = 0; i < vecH1D.size(); i++ ){
		if (m_verbose >= Verbose_HistInfo)
			std::cout<<prefix_HistInfo
					 <<"Output vecH1D["<<i<<"]: "<<nameForH1D[i]
					 <<", "<<titleForH1D[i]
					 <<", "<<xNameForH1D[i]
					 <<", "<<yNameForH1D[i]
					 <<", "<<bin1ForH1D[i]
					 <<", "<<left1ForH1D[i]
					 <<", "<<right1ForH1D[i]
					 <<", Color="<<colorForH1D[i]
					 <<", minx="<<minxForH1D[i]
					 <<", miny="<<minyForH1D[i]
					 <<", xlogSyle="<<xlogForH1D[i]
					 <<", ylogSyle="<<ylogForH1D[i]
					 <<", nCompare="<<compareForH1D[i]
					 <<", markerStyle="<<markerForH1D[i]
					 <<", normalize ="<<normForH1D[i]
					 <<", drawOpt=\""<<drawOptForH1D[i]
					 <<", useLegend=\""<<useLegendForH1D[i]
					 <<", legendName=\""<<legendNameForH1D[i]
					 <<", legendx1=\""<<legendx1ForH1D[i]
					 <<", legendy1=\""<<legendy1ForH1D[i]
					 <<", legendx2=\""<<legendx2ForH1D[i]
					 <<", legendy2=\""<<legendy2ForH1D[i]
					 <<", npadx=\""<<npadxForH1D[i]
					 <<", npady=\""<<npadyForH1D[i]
					 <<", sep=\""<<sepForH1D[i]
					 <<"\""<<std::endl;
		std::string name = vecH1D[i]->GetName();
		if (ipad >= padList.size()){
			if (m_verbose >= Verbose_HistInfo)
				std::cout<<prefix_HistInfo
						 <<"Need to create a new canvas"
						 <<"\""<<std::endl;
			c = new TCanvas(name.c_str(),name.c_str(),ww,wh);
			padList.clear();
			ipad=0;
			for (int ix = 0; ix < npadxForH1D[i]; ix++ ){
				for (int iy = npadyForH1D[i]-1; iy >= 0; iy--){
					buff.str("");
					buff.clear();
					buff<<"pad("<<ix<<":"<<iy<<")"<<std::endl;
					double x1 = ix/((double)npadxForH1D[i]);
					double y1 = iy/((double)npadyForH1D[i]);
					double x2 = (1+ix)/((double)npadxForH1D[i]);
					double y2 = (1+iy)/((double)npadyForH1D[i]);
					TPad *apad = new TPad(buff.str().c_str(),buff.str().c_str(),x1,y1,x2,y2);
					apad->Draw();
					padList.push_back(apad);
				}
			}
			if (m_verbose >= Verbose_HistInfo)
				std::cout<<prefix_HistInfo
						 <<"Added ("
						 <<npadxForH1D[i]*npadyForH1D[i]
						 <<") pads"
						 <<"\""<<std::endl;
		}
		padList[ipad]->cd();
		ipad++;
		if (normForH1D[i]){
			if (normForH1D[i] == 1) vecH1D[i]->Scale(1./vecH1D[i]->Integral());
			else vecH1D[i]->Scale(1./normForH1D[i]);
		}
		vecH1D[i]->SetLineColor(colorForH1D[i]);
		int nCompare = compareForH1D[i];
		if ( nCompare ) if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<nCompare<<" histograms to be compared"<<std::endl;
		double currentMaximum = vecH1D[i]->GetMaximum();
		double currentMinimum = vecH1D[i]->GetMinimum();
		if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    currentMaximum y value is ("<<currentMaximum<<")"<<std::endl;
		if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    currentMinimum y value is ("<<currentMinimum<<")"<<std::endl;
		for ( int j = 1; j <= nCompare; j++ ){
			if (normForH1D[i+j]){
				if (normForH1D[i+j] == 1) vecH1D[i+j]->Scale(1./vecH1D[i+j]->Integral());
				else vecH1D[i+j]->Scale(1./normForH1D[i+j]);
			}
			double maximum = vecH1D[i+j]->GetMaximum();
			if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    Maximum y for "<<nameForH1D[i+j]<<" is ("<<maximum<<")"<<std::endl;
			if ( maximum > currentMaximum ){
				currentMaximum = maximum;
			}
			double minimum = vecH1D[i+j]->GetMinimum();
			if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    Minimum y for "<<nameForH1D[i+j]<<" is ("<<minimum<<")"<<std::endl;
			if ( minimum < currentMinimum ){
				currentMinimum = minimum;
			}
		}
		if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  maximum y value is ("<<currentMaximum<<")"<<std::endl;
		if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  minimum y value is ("<<currentMinimum<<")"<<std::endl;
		if ( xlogForH1D[i] ){
			vecH1D[i]->GetXaxis()->SetRangeUser(minxForH1D[i],right1ForH1D[i]);
			if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  Logx! set xRange("<<minxForH1D[i]<<","<<right1ForH1D[i]<<")"<<std::endl;
		}
		else {
			if (left1ForH1D[i]!=right1ForH1D[i]){
				vecH1D[i]->GetXaxis()->SetRangeUser(left1ForH1D[i],right1ForH1D[i]);
				if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"        set xRange("<<left1ForH1D[i]<<","<<right1ForH1D[i]<<")"<<std::endl;
			}
		}
		if ( ylogForH1D[i] ) {
			if (currentMinimum<0){
				for ( int j = 0; j <= nCompare; j++ ){
					for (int i_bin = 0; i_bin<= vecH1D[i+j]->GetNbinsX(); i_bin++){
						vecH1D[i+j]->AddBinContent(i_bin,-currentMinimum*2);
					}
				}
				currentMinimum*=-1;
			}
			if (minyForH1D[i]>0) currentMinimum=minyForH1D[i];
			vecH1D[i]->GetYaxis()->SetRangeUser(currentMinimum/2,2*currentMaximum);
			if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  Logy! set yRange("<<minyForH1D[i]<<","<<2*currentMaximum<<")"<<std::endl;
		}
		else {
//			if (currentMinimum>minyForH1D[i]) currentMinimum=minyForH1D[i];
			if (currentMinimum>0) currentMinimum /= 1.1;
			else currentMinimum *= 1.1;
			vecH1D[i]->GetYaxis()->SetRangeUser(currentMinimum,1.05*currentMaximum);
			if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"        set yRange("<<minyForH1D[i]<<","<<1.05*currentMaximum<<")"<<std::endl;
		}
		if ( xlogForH1D[i] ) gPad->SetLogx(1);
		else gPad->SetLogx(0);
		if ( ylogForH1D[i] ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		gPad->SetGridx(1);
		gPad->SetGridy(1);
		vecH1D[i]->SetMarkerSize(0.5);
		vecH1D[i]->SetMarkerStyle(markerForH1D[i]);
		vecH1D[i]->SetMarkerColor(colorForH1D[i]);
		vecH1D[i]->SetLineColor(colorForH1D[i]);
		vecH1D[i]->GetXaxis()->SetTitle(xNameForH1D[i].c_str());
		vecH1D[i]->GetYaxis()->SetTitle(yNameForH1D[i].c_str());
		if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    Integral of ("<<nameForH1D[i]<<"): "<<vecH1D[i]->Integral()<<std::endl;
		vecH1D[i]->Draw(drawOptForH1D[i].c_str());
		int write_result = vecH1D[i]->Write();
		if (!write_result)
			std::cout<<prefix_HistInfo<<"Cannot wirte vecH1D"<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<xNameForH1D[i]<<", "<<yNameForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<", Color="<<colorForH1D[i]<<", xlogSyle="<<xlogForH1D[i]<<", ylogSyle="<<ylogForH1D[i]<<", nCompare="<<compareForH1D[i]<<", markerStyle="<<markerForH1D[i]<<", normalize ="<<normForH1D[i]<<", drawOpt=\""<<drawOptForH1D[i]<<"\""<<std::endl;
		TLegend *legend = new TLegend(legendx1ForH1D[i],legendy1ForH1D[i],legendx2ForH1D[i],legendy2ForH1D[i]);
		legend->AddEntry(vecH1D[i],legendNameForH1D[i].c_str());
		int useLegend = useLegendForH1D[i];
		double sep = sepForH1D[i];
		for ( int j = 0; j < nCompare; j++ ){
			i++;
			if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<" ->"<<j<<", vecH1D["<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<xNameForH1D[i]<<", "<<yNameForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<", Color="<<colorForH1D[i]<<", xlogSyle="<<xlogForH1D[i]<<", ylogSyle="<<ylogForH1D[i]<<", nCompare="<<compareForH1D[i]<<", markerStyle="<<markerForH1D[i]<<", normalize ="<<normForH1D[i]<<", drawOpt=\""<<drawOptForH1D[i]<<"\""<<std::endl;
			double offset = sep*((double)j)+sep;
			for (int i_bin = 0; i_bin<= vecH1D[i]->GetNbinsX(); i_bin++){
				vecH1D[i]->AddBinContent(i_bin,offset);
			}
			vecH1D[i]->SetLineColor(colorForH1D[i]);
			vecH1D[i]->SetMarkerSize(0.5);
			vecH1D[i]->SetMarkerStyle(markerForH1D[i]);
			vecH1D[i]->SetMarkerColor(colorForH1D[i]);
			vecH1D[i]->SetLineColor(colorForH1D[i]);
			std::string drawOpt = drawOptForH1D[i]+"SAME";
			if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    Integral of ("<<nameForH1D[i]<<"): "<<vecH1D[i]->Integral()<<std::endl;
			vecH1D[i]->Draw(drawOpt.c_str());
			legend->AddEntry(vecH1D[i],legendNameForH1D[i].c_str());
			int write_result = vecH1D[i]->Write();
			if (!write_result)
				std::cout<<prefix_HistInfo<<"Cannot write vecH1D"<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<xNameForH1D[i]<<", "<<yNameForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<", Color="<<colorForH1D[i]<<", xlogSyle="<<xlogForH1D[i]<<", ylogSyle="<<ylogForH1D[i]<<", nCompare="<<compareForH1D[i]<<", markerStyle="<<markerForH1D[i]<<", normalize ="<<normForH1D[i]<<", drawOpt=\""<<drawOptForH1D[i]<<"\""<<std::endl;
		}
		if (useLegend) legend->Draw("same");
		if (ipad >= padList.size()){
			std::string fileName = OutputDir + "/" + name + ".pdf";
			c->Print(fileName.c_str());
			fileName = OutputDir + "/" + name + ".png";
			c->Print(fileName.c_str());
		}
	}
	gStyle->SetOptStat(0);
	for ( int i = 0; i < vecH2D.size(); i++ ){
		if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Output vecH2D["<<i<<"]: "<<nameForH2D[i]<<", "<<titleForH2D[i]<<", "<<xNameForH2D[i]<<", "<<yNameForH2D[i]<<", "<<bin1ForH2D[i]<<", "<<left1ForH2D[i]<<", "<<right1ForH2D[i]<<", "<<bin2ForH2D[i]<<", "<<left2ForH2D[i]<<", "<<right2ForH2D[i]<<std::endl;
		std::string name = vecH2D[i]->GetName();
		TCanvas* c = new TCanvas(name.c_str(),name.c_str(),ww,wh);
		gPad->SetGridx(1);
		gPad->SetGridy(1);
		vecH2D[i]->GetXaxis()->SetTitle(xNameForH2D[i].c_str());
		vecH2D[i]->GetYaxis()->SetTitle(yNameForH2D[i].c_str());
		vecH2D[i]->SetContour(50);
		vecH2D[i]->Draw("COLZ");
		int write_result = vecH2D[i]->Write();
		if (!write_result)
			std::cout<<prefix_HistInfo<<"Cannot write vecH2D["<<i<<"]: "<<nameForH2D[i]<<", "<<titleForH2D[i]<<", "<<xNameForH2D[i]<<", "<<yNameForH2D[i]<<", "<<bin1ForH2D[i]<<", "<<left1ForH2D[i]<<", "<<right1ForH2D[i]<<", "<<bin2ForH2D[i]<<", "<<left2ForH2D[i]<<", "<<right2ForH2D[i]<<std::endl;
		std::string fileName = OutputDir + "/"+ name + ".pdf";
		c->Print(fileName.c_str());
		fileName = OutputDir + "/" + name + ".png";
		c->Print(fileName.c_str());
	}
	for ( int i = 0; i < nameForGraph.size(); i++ ){
		int sizeOfThisGraph = xForGraph[i].size();
		if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Output vecGraph["<<i<<"]: "<<nameForGraph[i]<<", "<<titleForGraph[i]<<", "<<xNameForGraph[i]<<", "<<yNameForGraph[i]<<", Color="<<colorForGraph[i]<<", xlogSyle="<<xlogForGraph[i]<<", ylogSyle="<<ylogForGraph[i]<<", nCompare="<<compareForGraph[i]<<", markerStyle="<<markerForGraph[i]<<", drawOpt=\""<<drawOptForGraph[i]<<"\", size = "<<sizeOfThisGraph<<std::endl;
		if ( sizeOfThisGraph <= 0 ) continue;
		if (m_verbose >= Verbose_HistInfo){
			for ( int j = 0; j < sizeOfThisGraph; j++ ){
				std::cout<<prefix_HistInfo<<"  ["<<j<<"]: ("<<xForGraph[i][j]<<","<<yForGraph[i][j]<<")"<<std::endl;
			}
		}
		std::string name = nameForGraph[i];
		TCanvas* c = new TCanvas(nameForGraph[i].c_str(),name.c_str(),ww,wh);
		TGraph *aTGraph = new TGraph(sizeOfThisGraph,&xForGraph[i][0],&yForGraph[i][0]);
		aTGraph->SetTitle(titleForGraph[i].c_str());
		int nCompare = compareForGraph[i];
		if ( nCompare ) if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<nCompare<<" graphs to be compared"<<std::endl;
		std::vector<double> yforgraph = yForGraph[i];
		std::vector<double> xforgraph = xForGraph[i];
		double currentMaximum = *std::max_element(yforgraph.begin(),yforgraph.end());
		if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    currentMaximum y value is ("<<currentMaximum<<")"<<std::endl;
		for ( int j = 1; j <= nCompare; j++ ){
			double maximum = *std::max_element(yForGraph[i+j].begin(),yForGraph[i+j].end());
			if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    Maximum y for "<<nameForGraph[i+j]<<" is ("<<maximum<<")"<<std::endl;
			if ( maximum > currentMaximum ){
				currentMaximum = maximum;
			}
		}
		if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  maximum y value is ("<<currentMaximum<<")"<<std::endl;
		if ( xlogForGraph[i] ) gPad->SetLogx(1);
		else gPad->SetLogx(0);
		if ( ylogForGraph[i] ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		gPad->SetGridx(1);
		gPad->SetGridy(1);
		if ( xlogForGraph[i] ) aTGraph->GetXaxis()->SetRangeUser(minxForGraph[i],2*maxxForGraph[i]);
		else aTGraph->GetXaxis()->SetRangeUser(minxForGraph[i],1.05*maxxForGraph[i]);
		if ( ylogForGraph[i] ) aTGraph->GetYaxis()->SetRangeUser(minyForGraph[i],2*currentMaximum);
		else aTGraph->GetYaxis()->SetRangeUser(minyForGraph[i],1.05*currentMaximum);
		aTGraph->GetXaxis()->SetTitle(xNameForGraph[i].c_str());
		aTGraph->GetYaxis()->SetTitle(yNameForGraph[i].c_str());
		aTGraph->SetMarkerSize(0.5);
		aTGraph->SetMarkerStyle(markerForGraph[i]);
		aTGraph->SetMarkerColor(colorForGraph[i]);
		aTGraph->SetLineColor(colorForGraph[i]);
		std::string drawOpt = "A"+drawOptForGraph[i];
		aTGraph->Draw(drawOpt.c_str());
		//aTGraph->Write();
		for ( int j = 0; j < nCompare; j++ ){
			i++;
			int sizeOfThisGraph = xForGraph[i].size();
			if (m_verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<" ->"<<j<<", vecGraph["<<i<<"]: "<<nameForGraph[i]<<", "<<titleForGraph[i]<<", "<<xNameForGraph[i]<<", "<<yNameForGraph[i]<<", Color="<<colorForGraph[i]<<", xlogSyle="<<xlogForGraph[i]<<", ylogSyle="<<ylogForGraph[i]<<", nCompare="<<compareForGraph[i]<<", markerStyle="<<markerForGraph[i]<<", drawOpt=\""<<drawOptForGraph[i]<<"\", size = "<<sizeOfThisGraph<<std::endl;
			if ( sizeOfThisGraph <= 0 ) continue;
			if (m_verbose >= Verbose_HistInfo){
				for ( int k = 0; k < sizeOfThisGraph; k++ ){
					std::cout<<prefix_HistInfo<<"  ["<<k<<"]: ("<<xForGraph[i][k]<<","<<yForGraph[i][k]<<")"<<std::endl;
				}
			}
			TGraph *bTGraph = new TGraph(sizeOfThisGraph,&xForGraph[i][0],&yForGraph[i][0]);
			bTGraph->SetTitle(titleForGraph[i].c_str());
			bTGraph->GetXaxis()->SetTitle(xNameForGraph[i].c_str());
			bTGraph->GetYaxis()->SetTitle(yNameForGraph[i].c_str());
			bTGraph->SetLineColor(colorForGraph[i]);
			aTGraph->SetMarkerSize(0.5);
			bTGraph->SetMarkerStyle(markerForGraph[i]);
			bTGraph->SetMarkerColor(colorForGraph[i]);
			bTGraph->SetLineColor(colorForGraph[i]);
			bTGraph->Draw(drawOptForGraph[i].c_str());
			//bTGraph->Write();
		}
		std::string fileName = OutputDir + "/" + name + ".pdf";
		c->Print(fileName.c_str());
		fileName = OutputDir + "/" + name + ".png";
		c->Print(fileName.c_str());
	}

	d_tree->Write();
	file->Close();

	std::string backupFileName = OutputDir + "/backup.root";
	if (m_backup){
		TFile *file2 = new TFile(backupFileName.c_str(),"RECREATE");
		m_TChain->CloneTree(-1,"fast");
		file2->Write();
		file2->Close();
	}

	delete file;
	return 0;
}

int MyRootInterface::get_TH2D_index(std::string name){
	for ( int i = 0; i < nameForH2D.size(); i++ ){
		if ( nameForH2D[i] == name ) return i;
	}
	if (m_verbose>=5) std::cout<<"###!!!In get_TH2D_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return -1;
}

int MyRootInterface::get_TH1D_index(std::string name){
	for ( int i = 0; i < nameForH1D.size(); i++ ){
		if ( nameForH1D[i] == name ) return i;
	}
	if (m_verbose>=5) std::cout<<"###!!!In get_TH1D_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return -1;
}

int MyRootInterface::get_TGraph_index(std::string name){
	for ( int i = 0; i < nameForGraph.size(); i++ ){
		if ( nameForGraph[i] == name ) return i;
	}
	if (m_verbose>=5) std::cout<<"###!!!In get_TGraph_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return -1;
}

int MyRootInterface::get_TBranch_index(std::string name){
	for ( int i = 0; i < vec_TBranchName.size(); i++ ){
		if ( vec_TBranchName[i] == name ) return i;
	}
	if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return -1;
}

int MyRootInterface::get_oTBranch_index(std::string name){
	for ( int i = 0; i < vec_oTBranchName.size(); i++ ){
		if ( vec_oTBranchName[i] == name ) return i;
	}
	if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return -1;
}

int MyRootInterface::get_value(std::string name, double &val, double scale){
	int index = -1;
	for ( int i = 0; i < vec_TBranchName.size(); i++ ){
		if ( vec_TBranchName[i] == name ){
			index = i;
			break;
		}
	}
	if (index == -1){
		if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
		return -1;
	}
	if (vec_TBranchType[index] != 0 || vec_TBranchIsVec[index] != 0){
		if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: Type does not match! original: ("<<vec_TBranchType[index]<<","<<vec_TBranchIsVec[index]<<") while required"<<"double"<<std::endl;
		return -1;
	}
	val = vec_double[index]*scale;
	return 0;
}

int MyRootInterface::get_value(std::string name, int &val, double scale){
	int index = -1;
	for ( int i = 0; i < vec_TBranchName.size(); i++ ){
		if ( vec_TBranchName[i] == name ){
			index = i;
			break;
		}
	}
	if (index == -1){
		if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
		return -1;
	}
	if (vec_TBranchType[index] != 1 || vec_TBranchIsVec[index] != 0){
		if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: Type does not match! original: ("<<vec_TBranchType[index]<<","<<vec_TBranchIsVec[index]<<") while required"<<"int"<<std::endl;
		return -1;
	}
	val = vec_int[index]*scale;
	return 0;
}

int MyRootInterface::get_value(std::string name, std::string &val){
	int index = -1;
	for ( int i = 0; i < vec_TBranchName.size(); i++ ){
		if ( vec_TBranchName[i] == name ){
			index = i;
			break;
		}
	}
	if (index == -1){
		if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
		return -1;
	}
	if (vec_TBranchType[index] != 2 || vec_TBranchIsVec[index] != 0){
		if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: Type does not match! original: ("<<vec_TBranchType[index]<<","<<vec_TBranchIsVec[index]<<") while required"<<"std::string"<<std::endl;
		return -1;
	}
	val = vec_string[index];
	return 0;
}

int MyRootInterface::get_value(std::string name, std::vector<double> &val, double scale){
	int index = -1;
	for ( int i = 0; i < vec_TBranchName.size(); i++ ){
		if ( vec_TBranchName[i] == name ){
			index = i;
			break;
		}
	}
	if (index == -1){
		if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
		return -1;
	}
	if (vec_TBranchType[index] != 0 || vec_TBranchIsVec[index] != 1){
		if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: Type does not match! original: ("<<vec_TBranchType[index]<<","<<vec_TBranchIsVec[index]<<") while required"<<"std::vector<double>"<<std::endl;
		return -1;
	}
	val = *vec_vecdouble[index];
	for (int i = 0; i < val.size(); i++){
		val[i] *= scale;
	}
	return 0;
}

int MyRootInterface::get_value(std::string name, std::vector<int> &val, double scale){
	int index = -1;
	for ( int i = 0; i < vec_TBranchName.size(); i++ ){
		if ( vec_TBranchName[i] == name ){
			index = i;
			break;
		}
	}
	if (index == -1){
		if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
		return -1;
	}
	if (vec_TBranchType[index] != 1 || vec_TBranchIsVec[index] != 1){
		if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: Type does not match! original: ("<<vec_TBranchType[index]<<","<<vec_TBranchIsVec[index]<<") while required"<<"std::vector<int>"<<std::endl;
		return -1;
	}
	val = *vec_vecint[index];
	for (int i = 0; i < val.size(); i++){
		val[i] *= scale;
	}
	return 0;
}

int MyRootInterface::get_value(std::string name, std::vector<std::string> &val){
	int index = -1;
	for ( int i = 0; i < vec_TBranchName.size(); i++ ){
		if ( vec_TBranchName[i] == name ){
			index = i;
			break;
		}
	}
	if (index == -1){
		if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
		return -1;
	}
	if (vec_TBranchType[index] != 2 || vec_TBranchIsVec[index] != 1){
		if (m_verbose>=5) std::cout<<"###!!!In get_TBranch_index: Type does not match! original: ("<<vec_TBranchType[index]<<","<<vec_TBranchIsVec[index]<<") while required"<<"std::vector<std::string>"<<std::endl;
		return -1;
	}
	val = *vec_vecstring[index];
	return 0;
}

int MyRootInterface::set_ovalue(std::string name, double val){
	int index = -1;
	for ( int i = 0; i < vec_oTBranchName.size(); i++ ){
		if ( vec_oTBranchName[i] == name ){
			index = i;
			break;
		}
	}
	if (index == -1){
		if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
		return -1;
	}
	if (vec_oTBranchType[index] != 0 || vec_oTBranchIsVec[index] != 0){
		if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: Type does not match! original: ("<<vec_oTBranchType[index]<<","<<vec_oTBranchIsVec[index]<<") while required"<<"double"<<std::endl;
		return -1;
	}
	ovec_double[index] = val;
	return 0;
}

int MyRootInterface::set_ovalue(std::string name, int val){
	int index = -1;
	for ( int i = 0; i < vec_oTBranchName.size(); i++ ){
		if ( vec_oTBranchName[i] == name ){
			index = i;
			break;
		}
	}
	if (index == -1){
		if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
		return -1;
	}
	if (vec_oTBranchType[index] != 1 || vec_oTBranchIsVec[index] != 0){
		if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: Type does not match! original: ("<<vec_oTBranchType[index]<<","<<vec_oTBranchIsVec[index]<<") while required"<<"int"<<std::endl;
		return -1;
	}
	ovec_int[index] = val;
	return 0;
}

int MyRootInterface::set_ovalue(std::string name, std::string val){
	int index = -1;
	for ( int i = 0; i < vec_oTBranchName.size(); i++ ){
		if ( vec_oTBranchName[i] == name ){
			index = i;
			break;
		}
	}
	if (index == -1){
		if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
		return -1;
	}
	if (vec_oTBranchType[index] != 2 || vec_oTBranchIsVec[index] != 0){
		if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: Type does not match! original: ("<<vec_oTBranchType[index]<<","<<vec_oTBranchIsVec[index]<<") while required"<<"std::string"<<std::endl;
		return -1;
	}
	ovec_string[index] = val;
	return 0;
}

int MyRootInterface::set_ovalue(std::string name, std::vector<double> val){
	int index = -1;
	for ( int i = 0; i < vec_oTBranchName.size(); i++ ){
		if ( vec_oTBranchName[i] == name ){
			index = i;
			break;
		}
	}
	if (index == -1){
		if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
		return -1;
	}
	if (vec_oTBranchType[index] != 0 || vec_oTBranchIsVec[index] != 1){
		if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: Type does not match! original: ("<<vec_oTBranchType[index]<<","<<vec_oTBranchIsVec[index]<<") while required"<<"std::vector<double>"<<std::endl;
		return -1;
	}
	ovec_vecdouble[index] = val;
	return 0;
}

int MyRootInterface::set_ovalue(std::string name, std::vector<int> val){
	int index = -1;
	for ( int i = 0; i < vec_oTBranchName.size(); i++ ){
		if ( vec_oTBranchName[i] == name ){
			index = i;
			break;
		}
	}
	if (index == -1){
		if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
		return -1;
	}
	if (vec_oTBranchType[index] != 1 || vec_oTBranchIsVec[index] != 1){
		if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: Type does not match! original: ("<<vec_oTBranchType[index]<<","<<vec_oTBranchIsVec[index]<<") while required"<<"std::vector<int>"<<std::endl;
		return -1;
	}
	ovec_vecint[index] = val;
	return 0;
}

int MyRootInterface::set_ovalue(std::string name, std::vector<std::string> val){
	int index = -1;
	for ( int i = 0; i < vec_oTBranchName.size(); i++ ){
		if ( vec_oTBranchName[i] == name ){
			index = i;
			break;
		}
	}
	if (index == -1){
		if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: CAN NOT FIND "<<name<<"!!!"<<std::endl;
		return -1;
	}
	if (vec_oTBranchType[index] != 2 || vec_oTBranchIsVec[index] != 1){
		if (m_verbose>=5) std::cout<<"###!!!In get_oTBranch_index: Type does not match! original: ("<<vec_oTBranchType[index]<<","<<vec_oTBranchIsVec[index]<<") while required"<<"std::vector<std::string>"<<std::endl;
		return -1;
	}
	ovec_vecstring[index] = val;
	return 0;
}

bool MyRootInterface::ISEMPTY(std::string content){
	bool flag = false;
	const char* c_card = content.c_str();
	int length = strlen(c_card);
	int offset = 0;
	for ( ; offset < length; offset++ ){
		if ( c_card[offset] != ' ' ) break;
	}
	if ( c_card[offset] == '#' || (c_card[offset] == '/' && c_card[offset+1] == '/') || length - offset == 0 ){
		flag = true;
	}
	return flag;
}

void MyRootInterface::seperate_string(std::string line, std::vector<std::string> &strs, const char sep ){
	std::string token;
	std::stringstream ss(line);
	while(std::getline(ss, token, sep)){
		token.erase(token.find_last_not_of('\t')+1);
		token.erase(0,token.find_first_not_of('\t'));
		token.erase(token.find_last_not_of(' ')+1);
		token.erase(0,token.find_first_not_of(' '));
		strs.push_back(token);
	}
}

double MyRootInterface::string2double(std::string str){
	double val;
	std::stringstream ss(str);
	ss>>val;
	return val;
}
