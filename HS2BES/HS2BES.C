#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //getopt
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <fcntl.h>
#include <math.h>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TArrow.h"
#include "TAxis.h"
#include "TFile.h"
#include "TPaveText.h"

/* constant */
double PI = 3.141592653589793238;
double FSC = 1/137.03599911; //fine structure constant
double M_MUON = 0.1056584; //mass of muon in GeV
double M_ELE = 0.000510999; //mass of electron in GeV
double M_U = 0.931494061; //atomic mass unit in GeV

/* arguments */
char m_workMode[128];
std::vector<std::string> m_input_files;
char m_output_file[1024];
int m_nEventsLimit = 0;

void init_args()
{
	strcpy(m_workMode,"normal");
	m_nEventsLimit=0;
}
/* end of arguments */

/* begin of functions */
void print_usage(char* prog_name)
{
	fprintf(stderr,"Usage %s [options (args)] [input files]\n",prog_name);
	fprintf(stderr,"[options]\n");
	fprintf(stderr,"\t -m\n");
	fprintf(stderr,"\t\t choose work mode: [normal(default)]\n");
	fprintf(stderr,"\t -n\n");
	fprintf(stderr,"\t\t nEvent limit\n");
	fprintf(stderr,"\t -o\n");
	fprintf(stderr,"\t\t specify the output file.\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m normal input.root -o output.txt input1.root input2.root\n",prog_name);
}
/* end of functions */

int main(int argc, char** argv){
	// Get arguments
	if (argc==1) {
		print_usage(argv[0]);
		return -1;
	}
	init_args();
	int result;
	while((result=getopt(argc,argv,"hm:n:o:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				strcpy(m_workMode,optarg);
				printf("work mode: %s\n",m_workMode);
				break;
			case 'n':
				m_nEventsLimit = atoi(optarg);
				printf("nEvent limit: %d\n",m_nEventsLimit);
				break;
			case 'o':
				strcpy(m_output_file,optarg);
				printf("output file: %s\n",m_output_file);
				break;
			case '?':
				printf("Wrong option! optopt=%c, optarg=%s\n", optopt, optarg);
				break;
			case 'h':
			default:
				print_usage(argv[0]);
				return 1;
		}
	}

	for (;optind<argc;optind++){
		m_input_files.push_back(argv[optind]);
	}

	// Open input file
	TChain *m_TChain = new TChain("t");
	for ( int i = 0; i < m_input_files.size(); i++ ){
		m_TChain->Add(m_input_files[i].c_str());
	}

	// Open output file
	std::ofstream fout;
	fout.open(m_output_file);
	if(!fout){
		std::cout<<"Cannot open "<<m_output_file<<" in ofstream format"<<std::endl;
		return -1;
	}

	// Statistics
	int nEvent_pass = 0;

	// output header
	fout<<"\n{ VERSION\n 1 0\n} VERSION\n";

	// Read input file
	//=> Prepare
	int wire_nhits;
	int wire_ilayer[1000];
	int wire_icell[1000];
	double wire_posx[1000];
	double wire_posy[1000];
	double wire_zreco[1000];
	double wire_momx[1000];
	double wire_momy[1000];
	double wire_momz[1000];
	double wire_rdrift_smeared[1000];
	double wire_tofg[1000];
	int nhits_abs;
	double hits_abs_tofg[1000];

	m_TChain->SetBranchAddress("wire_nhits", &wire_nhits);
	m_TChain->SetBranchAddress("wire_ilayer", wire_ilayer);
	m_TChain->SetBranchAddress("wire_icell", wire_icell);
	m_TChain->SetBranchAddress("wire_posx", wire_posx);
	m_TChain->SetBranchAddress("wire_posy", wire_posy);
	m_TChain->SetBranchAddress("wire_zreco", wire_zreco);
	m_TChain->SetBranchAddress("wire_momx", wire_momx);
	m_TChain->SetBranchAddress("wire_momy", wire_momy);
	m_TChain->SetBranchAddress("wire_momz", wire_momz);
	m_TChain->SetBranchAddress("wire_rdrift_smeared", wire_rdrift_smeared);
	m_TChain->SetBranchAddress("wire_tofg", wire_tofg);
	m_TChain->SetBranchAddress("nhits_abs", &nhits_abs);
	m_TChain->SetBranchAddress("hits_abs_tofg", hits_abs_tofg);

	//=> Read
	Long64_t nEvent = m_TChain->GetEntries();
	std::cout<<"Totally ["<<(m_nEventsLimit?m_nEventsLimit:nEvent)<<"] events will be scanned"<<std::endl;
	std::stringstream buf_MDCTRUTH;
	std::stringstream buf_MDCDIGI;
	//==>loop in events
	for( Long64_t iEvent = 0; iEvent < (m_nEventsLimit?m_nEventsLimit:nEvent); iEvent++ ){
		m_TChain->GetEntry(iEvent);
//		std::cout<<"=>["<<iEvent<<"]:"<<std::endl;
//		std::cout<<"  wire_nhits = "<<wire_nhits<<std::endl;
//		std::cout<<"  wire_ilayer = "<<wire_ilayer[0]<<std::endl;
//		std::cout<<"  wire_icell = "<<wire_icell[0]<<std::endl;
//		std::cout<<"  wire_posx = "<<wire_posx[0]<<std::endl;
//		std::cout<<"  wire_posy = "<<wire_posy[0]<<std::endl;
//		std::cout<<"  wire_zreco = "<<wire_zreco[0]<<std::endl;
//		std::cout<<"  wire_momx = "<<wire_momx[0]<<std::endl;
//		std::cout<<"  wire_momy = "<<wire_momy[0]<<std::endl;
//		std::cout<<"  wire_momz = "<<wire_momz[0]<<std::endl;
//		std::cout<<"  wire_rdrift_smeared = "<<wire_rdrift_smeared[0]<<std::endl;
//		std::cout<<"  wire_tofg = "<<wire_tofg[0]<<std::endl;
//		std::cout<<"  nhits_abs = "<<nhits_abs<<std::endl;
//		std::cout<<"  hits_abs_tofg = "<<hits_abs_tofg[0]<<std::endl;
		if (nhits_abs==0||wire_nhits==0) continue; // need hits
		if (wire_tofg[0] > hits_abs_tofg[0]) continue; // hit trigger after chamber

		// for EVHEAD
		int runNo = -30000;
		int eventNo = nEvent_pass;
		// for TRACKTRUTH
		int nTracks = 1;
		int trackIndex = 0;
		int PDGCode = 11;
		int PDGCharge = -1;
		double px = wire_momx[0]; // in GeV
		double py = wire_momy[0]; // in GeV
		double pz = wire_momz[0]; // in GeV
		double E = sqrt(px*px+py*py+pz*pz+M_ELE*M_ELE); // in GeV
		// for VERTEXTRUTH
		double time = wire_tofg[0];
		double posX = wire_posx[0]*10;
		double posY = wire_posy[0]*10;
		double posZ = wire_zreco[0]*10;
		// for MDCTRUTH
		buf_MDCTRUTH.clear();
		buf_MDCTRUTH.str("");
		int nHits = wire_nhits;
		double prevE = E;
		// for MDCDIGI
		buf_MDCDIGI.clear();
		buf_MDCDIGI.str("");

		buf_MDCTRUTH<<" "<<nHits;
		buf_MDCDIGI<<" "<<nHits;
		for ( int i = 0; i < nHits; i++ ){
			// for MDCTRUTH
			double px = wire_momx[i]; // in GeV
			double py = wire_momy[i]; // in GeV
			double pz = wire_momz[i]; // in GeV
			double curE = sqrt(px*px+py*py+pz*pz+M_ELE*M_ELE); // in GeV
			double edep = 0;
			if ( i == 0 ) edep = 1e-4;
			else edep = curE - prevE;
			prevE = curE;
			int layerNo = wire_ilayer[i];
			int cellNo = wire_icell[i];
			double driftD = wire_rdrift_smeared[i]*10;
			double posX = wire_posx[i]*10;
			double posY = wire_posy[i]*10;
			double posZ = wire_zreco[i]*10;
			int posFlag = 0;
			// for MDCDIGI
			double driftTime = driftD; //x=t

			buf_MDCTRUTH<<"  "<<"0 "<<" "<<layerNo<<" "<<cellNo<<" "<<edep<<" "<<driftD<<" "<<posX<<" "<<posY<<" "<<posZ<<" "<<posFlag<<"\n";
			buf_MDCDIGI<<"  "<<"0 "<<layerNo<<" "<<cellNo<<" "<<edep<<" "<<driftTime<<"\n";
		}

		fout<<"\n{ EVENT\n";
		fout<<"\n{ EVHEAD\n "<<runNo<<" "<<eventNo<<"\n} EVHEAD\n";
		fout<<"\n{ DECAYMODE\n 10 0 0 0 0 0 0 0 0 0 0\n} DECAYMODE\n";
		fout<<"\n{ TRACKTRUTH\n "<<nTracks<<"  "<<trackIndex<<" "<<PDGCode<<" "<<PDGCharge<<" 0 -99 "<<px<<" "<<py<<" "<<pz<<" "<<E<<" -99 -99\n\n} TRACKTRUTH\n";
		fout<<"\n{ VERTEXTRUTH\n 1  0 -99 "<<posX<<" "<<posY<<" "<<posZ<<" "<<time<<"\n\n} VERTEXTRUTH\n";
		fout<<"\n{ MDCTRUTH\n"<<buf_MDCTRUTH.str()<<"\n} MDCTRUTH\n";
		fout<<"\n{ MDCDIGI\n"<<buf_MDCDIGI.str()<<"\n} MDCDIGI\n";
		fout<<"\n{ TOFTRUTH\n\n} TOFTRUTH\n";
		fout<<"\n{ TOFDIGI\n\n} TOFDIGI\n";
		fout<<"\n{ EMCTRUTH\n\n} EMCTRUTH\n";
		fout<<"\n{ EMCDIGI\n\n} EMCDIGI\n";
		fout<<"\n{ MUCTRUTH\n\n} MUCTRUTH\n";
		fout<<"\n{ MUCDIGI\n\n} MUCDIGI\n";
		fout<<"\n} EVENT";

		nEvent_pass++;
	}

	// output statistics
	std::cout<<"Total event pass: "<<nEvent_pass<<std::endl;

	return 0;
}
