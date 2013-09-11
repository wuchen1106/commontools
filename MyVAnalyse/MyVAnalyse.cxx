#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>

#include "globals.hh"
#include "Randomize.hh"

#include "TH1D.h"
#include "TH2D.h"

#include "MyRootInterface.hxx"

char m_workMode[128];
std::string m_runName;
int verbose = 0;
int nEvents = 0;
int bEvent = 0;
int printModule = 1;
bool backup = false;
std::vector<int> Ncut;
std::vector<std::string> Ncut_message;

void init_Ncut();
void inc_Ncut(std::string);
void dump_Ncut();
void init_args();
void print_usage(char* prog_name);

int main(int argc, char** argv){

	clock_t t_START = clock();

	//=======================================
	//*************read parameter**********
	init_args();
	int result;
	while((result=getopt(argc,argv,"B:hbv:n:m:r:p:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				strcpy(m_workMode,optarg);
				printf("work mode: %s\n",m_workMode);
				break;
			case 'r':
				m_runName=optarg;
				printf("run name: %s\n",m_runName.c_str());
				break;
			case 'v':
				verbose = atoi(optarg);
				printf("verbose level: %d\n",verbose);
				break;
			case 'b':
				backup = true;
				printf("restore backup file!\n");
				break;
			case 'B':
				bEvent = atoi(optarg);
				printf("bEvent: %d\n",bEvent);
				break;
			case 'n':
				nEvents = atoi(optarg);
				printf("nEvent: %d\n",nEvents);
				break;
			case 'p':
				printModule = atoi(optarg);
				printf("printModule: %d\n",printModule);
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

	//=======================================
	//************Verbose Control***********
	int Verbose_SectorInfo = 5; //大概的流程情况
	std::string prefix_SectorInfo = "### ";
	int Verbose_HistInfo = 10; //有哪些hist,什么时候输出了，参数如何
	std::string prefix_HistInfo= "  [Histograms] ";
	int Verbose_Statistics = 10; //跟统计相关的(效率，分辨，粒子鉴别的情况)
	std::string prefix_Statistics="  [Statistics] ";
	int Verbose_FileInfo = 10; //有哪些FileList,都有多少file
	std::string prefix_FileInfo="  [FileInfo] ";
	int Verbose_EffInfo = 15; //Efficiency info
	std::string prefix_EffInfo="  [EffInfo] ";
	int Verbose_EventInfo = 20; //每个event的基本流程
	std::string prefix_EventInfoStart="    =>[EventInfo] ";
	std::string prefix_EventInfo="      [EventInfo] ";
	int Verbose_ParticleInfo=25; //每个particle的基本信息
	std::string prefix_ParticleInfoStart="    ->[ParticleInfo]";
	std::string prefix_ParticleInfo="      [ParticleInfo]";

	//##########################PRESET############################
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In Preset###"<<std::endl;
	MyRootInterface *fMyRootInterface = new MyRootInterface(verbose,backup);
	fMyRootInterface->set_OutputDir("result");
	int index_temp = 0;
	TH1D *h1d_temp=0;
	int bin_temp = 0;
	std::string name_temp = "";
	std::string title_temp = "";
	std::string  xName_temp = "";
	std::string  yName_temp = "";
	int  bin1_temp = 0;
	double  left1_temp = 0;
	double  right1_temp = 0;
	double  minx_temp = 0;
	double  miny_temp = 0;
	int  color_temp = 0;
	int  compare_temp = 0;
	int  xlog_temp = 0;
	int  ylog_temp = 0;
	int  marker_temp = 0;
	double  norm_temp = 0;
	std::string  drawOpt_temp = "";
	std::stringstream buff;

	//=>About Constant
	double PI = 3.141592653589793238;
	double FSC = 1/137.03599911; //fine structure constant
	double M_MUON = 105.6584*MeV; //mass of muon in MeV
	double M_ELE = 0.510999*MeV; //mass of electron in MeV
	double M_U = 931.494061*MeV; //atomic mass unit in MeV
	double M_p = 0.9382723*GeV;  // mass of proton// proton mass unit in GeV

	//##########################Prepare histograms############################
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET HISTOGRAMS###"<<std::endl;
	fMyRootInterface->read("input");
	fMyRootInterface->set_OutputName(m_runName);
	fMyRootInterface->init();

	//************SET Statistics********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET Statistics###"<<std::endl;
	//=>About Statistical
	init_Ncut();

	/*
	//*********If you have to read a histogram************************
	int ihist_SPEC = 1;
	TH1D* h_SPEC = fMyRootInterface->get_TH1D(ihist_SPEC);
	h_SPEC->Scale(1./h_SPEC->Integral());

	//********If you have to create a histogram**********************
	int num_bin = h_SPEC->GetNbinsX(); //How many bins do you want
	//name_temp = fMyRootInterface->get_nameForH1D(iHist);
	//title_temp = fMyRootInterface->get_titleForH1D(iHist);
	xName_temp = fMyRootInterface->get_xNameForH1D(ihist_SPEC);
	yName_temp = fMyRootInterface->get_yNameForH1D(ihist_SPEC);
	//bin1_temp = fMyRootInterface->get_bin1ForH1D(iHist);
	//left1_temp = fMyRootInterface->get_left1ForH1D(iHist);
	//right1_temp = fMyRootInterface->get_right1ForH1D(iHist);
	bin1_temp = h_SPEC->GetNbinsX();
	left1_temp = h_SPEC->GetXaxis()->GetXmin();
	right1_temp = h_SPEC->GetXaxis()->GetXmax();
	minx_temp = fMyRootInterface->get_minxForH1D(ihist_SPEC);
	miny_temp = fMyRootInterface->get_minyForH1D(ihist_SPEC);
	color_temp = fMyRootInterface->get_colorForH1D(ihist_SPEC);
	compare_temp = fMyRootInterface->get_compareForH1D(ihist_SPEC);
	xlog_temp = fMyRootInterface->get_xlogForH1D(ihist_SPEC);
	ylog_temp = fMyRootInterface->get_ylogForH1D(ihist_SPEC);
	marker_temp = fMyRootInterface->get_markerForH1D(ihist_SPEC);
	norm_temp = fMyRootInterface->get_normForH1D(ihist_SPEC);
	drawOpt_temp = fMyRootInterface->get_drawOptForH1D(ihist_SPEC);

	name_temp = "MisMeas_hist";
	title_temp = "Momentum Cut Efficiency";
	xName_temp = "p_{a}(MeV/c)";
	yName_temp = "probability";
	miny_temp = 1e-5;
	ylog_temp = 1;
	TH1D* h_MisMeas = fMyRootInterface->add_TH1D(name_temp,title_temp,xName_temp,yName_temp,bin1_temp,left1_temp,right1_temp,minx_temp,miny_temp,color_temp,compare_temp,xlog_temp,ylog_temp,marker_temp,norm_temp,drawOpt_temp);
	*/

	// for initial information
	int evt_num;
	int run_num;
	int ini_tid;
	int ini_pid;
	int ini_ppid = 0;
	double ini_t;
	double ini_x;
	double ini_y;
	double ini_z;
	double ini_px;
	double ini_py;
	double ini_pz;
	double ini_ot;
	double ini_ox;
	double ini_oy;
	double ini_oz;
	double ini_opx;
	double ini_opy;
	double ini_opz;
	std::string ini_process = "NONE";
	std::string ini_volume = "NONE";
	double weight0 = 1;

	std::vector<int> Monitor_pid;
	std::vector<double> Monitor_t;
	std::vector<double> Monitor_x;
	std::vector<double> Monitor_y;
	std::vector<double> Monitor_z;
	std::vector<double> Monitor_px;
	std::vector<double> Monitor_py;
	std::vector<double> Monitor_pz;

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK###"<<std::endl;
	Long64_t nEvent = fMyRootInterface->get_Entries();
	for( Long64_t iEvent = bEvent; iEvent < (nEvents&&nEvents<nEvent?nEvents:nEvent); iEvent++ ){
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
		fMyRootInterface->GetEntry(iEvent);
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;
		inc_Ncut("Got entries");

		fMyRootInterface->get_value("evt_num",evt_num);
		fMyRootInterface->get_value("run_num",run_num);
		//************If you Need to read TTree*************
		fMyRootInterface->get_value("t",ini_t,ns);
		fMyRootInterface->get_value("x",ini_x,mm);
		fMyRootInterface->get_value("y",ini_y,mm);
		fMyRootInterface->get_value("z",ini_z,mm);
		fMyRootInterface->get_value("px",ini_px,MeV);
		fMyRootInterface->get_value("py",ini_py,MeV);
		fMyRootInterface->get_value("pz",ini_pz,MeV);
		fMyRootInterface->get_value("ot",ini_ot,ns);
		fMyRootInterface->get_value("ox",ini_ox,mm);
		fMyRootInterface->get_value("oy",ini_oy,mm);
		fMyRootInterface->get_value("oz",ini_oz,mm);
		fMyRootInterface->get_value("opx",ini_opx,MeV);
		fMyRootInterface->get_value("opy",ini_opy,MeV);
		fMyRootInterface->get_value("opz",ini_opz,MeV);
		fMyRootInterface->get_value("pid",ini_pid);
		fMyRootInterface->get_value("tid",ini_tid);
		fMyRootInterface->get_value("ppid",ini_ppid);
		fMyRootInterface->get_value("process",ini_process);
		fMyRootInterface->get_value("volume",ini_volume);
		fMyRootInterface->get_value("weight",weight0);

//		fMyRootInterface->get_value("CDCMonitor_x",Monitor_x,cm);
//		fMyRootInterface->get_value("CDCMonitor_y",Monitor_y,cm);
//		fMyRootInterface->get_value("CDCMonitor_z",Monitor_z,cm);
//		fMyRootInterface->get_value("CDCMonitor_px",Monitor_px,GeV);
//		fMyRootInterface->get_value("CDCMonitor_py",Monitor_py,GeV);
//		fMyRootInterface->get_value("CDCMonitor_pz",Monitor_pz,GeV);
//		fMyRootInterface->get_value("CDCMonitor_t",Monitor_t,ns);
//		fMyRootInterface->get_value("CDCMonitor_pid",Monitor_pid);
//		pid=11;

		//****************************CUT************************
		//
		double pa = sqrt(ini_px*ini_px+ini_py*ini_py+ini_pz*ini_pz);
		if (pa<75*MeV) continue;
//		inc_Ncut("pa <= 15 MeV");
//		if (t<350*ns) continue;
//		if (cvolume=="Target"||cvolume=="CDCLayer"||cvolume=="McTruth") continue;
//		if (cvolume!="McTruth") continue;
		//
		//****************************CUT************************

		//************If you need to output for every hit*************
//		for ( int i_Mon = 0; i_Mon < Monitor_t.size(); i_Mon++ ){
//			fMyRootInterface->set_ovalue("x",Monitor_x[i_Mon]/mm);
//			fMyRootInterface->set_ovalue("y",Monitor_y[i_Mon]/mm);
//			fMyRootInterface->set_ovalue("z",Monitor_z[i_Mon]/mm);
//			fMyRootInterface->set_ovalue("px",Monitor_px[i_Mon]/MeV);
//			fMyRootInterface->set_ovalue("py",Monitor_py[i_Mon]/MeV);
//			fMyRootInterface->set_ovalue("pz",Monitor_pz[i_Mon]/MeV);
//			fMyRootInterface->set_ovalue("t",Monitor_t[i_Mon]/ns);
//			fMyRootInterface->set_ovalue("pid",Monitor_pid[i_Mon]);
//			fMyRootInterface->Fill();
//		}

		//************If you need to ouput TTree***************
		fMyRootInterface->set_ovalue("run_num",run_num);
		fMyRootInterface->set_ovalue("evt_num",evt_num);
		fMyRootInterface->set_ovalue("x",ini_x/mm);
		fMyRootInterface->set_ovalue("y",ini_y/mm);
		fMyRootInterface->set_ovalue("z",ini_z/mm);
		fMyRootInterface->set_ovalue("px",ini_px/MeV);
		fMyRootInterface->set_ovalue("py",ini_py/MeV);
		fMyRootInterface->set_ovalue("pz",ini_pz/MeV);
		fMyRootInterface->set_ovalue("t",ini_t/ns);
		fMyRootInterface->set_ovalue("ox",ini_ox);
		fMyRootInterface->set_ovalue("oy",ini_oy);
		fMyRootInterface->set_ovalue("oz",ini_oz);
		fMyRootInterface->set_ovalue("opx",ini_opx);
		fMyRootInterface->set_ovalue("opy",ini_opy);
		fMyRootInterface->set_ovalue("opz",ini_opz);
		fMyRootInterface->set_ovalue("ot",ini_ot);
		fMyRootInterface->set_ovalue("pid",ini_pid);
		fMyRootInterface->set_ovalue("tid",ini_tid);
		fMyRootInterface->set_ovalue("ppid",ini_ppid);
		fMyRootInterface->set_ovalue("weight",weight0);
		fMyRootInterface->set_ovalue("volume",ini_volume);
		fMyRootInterface->set_ovalue("process",ini_process);
		fMyRootInterface->Fill();

		//************If you need to fill TH1D*****************
//		if (((index_temp = fMyRootInterface->get_TH1D_index(m_runName+"x")))!=-1)
//			fMyRootInterface->get_TH1D(index_temp)->Fill(x/cm);

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Done"<<std::endl;
	}

	//=>For output
	clock_t t_END = clock();
	//#########################THEEND###############################
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In WRITE AND OUTPUT ###"<<std::endl;
	std::cout<<"\n################BENTCH MARK###################"<<std::endl;
	std::cout<<"TOTAL TIME COST IS:  "<<(double)(t_END-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"PRESET:              "<<(double)(t_PRESET-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"INITIALIZE:          "<<(double)(t_INITIALIZE-t_PRESET)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"GENERATEEVENTS:      "<<(double)(t_GENERATEEVENTS-t_BUILDHISTOGRAMS)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"TIME COST PER EVENT: "<<(double)(t_GENERATEEVENTS-t_BUILDHISTOGRAMS)/CLOCKS_PER_SEC/num_evt*1000<<"ms"<<std::endl;
	std::cout<<"##############################################\n"<<std::endl;
	dump_Ncut();

	fMyRootInterface->dump();
	return 0;
}

void init_args()
{
	strcpy(m_workMode,"gen");
	m_runName="output";
	verbose = 0;
	nEvents = 0;
	bEvent = 0;
	printModule = 10000;
	backup = false;
}

void print_usage(char* prog_name)
{
	fprintf(stderr,"Usage %s [options (args)] [input files]\n",prog_name);
	fprintf(stderr,"[options]\n");
	fprintf(stderr,"\t -m\n");
	fprintf(stderr,"\t\t choose work mode: [gen(default), com]\n");
	fprintf(stderr,"\t -r\n");
	fprintf(stderr,"\t\t set run name\n");
	fprintf(stderr,"\t -v\n");
	fprintf(stderr,"\t\t verbose level\n");
	fprintf(stderr,"\t -n\n");
	fprintf(stderr,"\t\t nEvent\n");
	fprintf(stderr,"\t -p\n");
	fprintf(stderr,"\t\t printModule\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"\t -b\n");
	fprintf(stderr,"\t\t restore backup file.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m ab -v 20 -n 100\n",prog_name);
}

void init_Ncut(){
	Ncut.clear();
	Ncut_message.clear();
}

void inc_Ncut(std::string mess){
	int index = -1;
	for (int i = 0; i < Ncut_message.size(); i++){
		if (Ncut_message[i]==mess){
			index = i;
			break;
		}
	}
	if (index == -1){
		index = Ncut_message.size();
		Ncut_message.push_back(mess);
		Ncut.push_back(0);
	}
	Ncut[index]++;
}

void dump_Ncut(){
	for (int i = 0; i < Ncut.size(); i++ ){
		std::cout<<"  "<<Ncut_message[i]<<": "<<Ncut[i]<<std::endl;
	}
}
