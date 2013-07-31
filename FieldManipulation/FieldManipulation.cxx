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

std::string m_workMode;
std::string m_runName;
std::string m_mapfile;
std::string m_output_file;
double m_ByOffset = 0;
int m_verbose = 0;
int nEvents = 0;
int printModule = 1;
bool backup = false;

void init_args();
void print_usage(char* prog_name);
bool ISEMPTY(std::string content);
bool StartWithNumber(std::string content);
void seperate_string(std::string line, std::vector<std::string> &strs, const char sep );
double string2double(std::string str);

int main(int argc, char** argv){

	clock_t t_START = clock();

	//=======================================
	//*************read parameter**********
	init_args();
	int result;
	while((result=getopt(argc,argv,"hbv:n:m:r:p:B:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				m_workMode = optarg;
				printf("work mode: %s\n",m_workMode.c_str());
				break;
			case 'r':
				m_runName=optarg;
				printf("run name: %s\n",m_runName.c_str());
				break;
			case 'v':
				m_verbose = atoi(optarg);
				printf("m_verbose level: %d\n",m_verbose);
				break;
			case 'b':
				backup = true;
				printf("restore backup file!\n");
				break;
			case 'n':
				nEvents = atoi(optarg);
				printf("nEvent: %d\n",nEvents);
				break;
			case 'B':
				m_ByOffset = atof(optarg);
				printf("By Offset: %lf\n",m_ByOffset);
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
	if (m_workMode == "sasaki" || m_workMode == "UK" || m_workMode == "UK_line" || m_workMode == "UK_line_r"){
		if (argc-optind<1){
			std::cout<<"This is \""<<m_workMode<<"\" mode which need arguments: input mapfile"<<std::endl;
			std::cout<<"Insufficient names!"<<std::endl;
			return -1;
		}
		m_mapfile = argv[optind++];
		std::cout<<"m_mapfile = "<<m_mapfile<<std::endl;
		if (argc-optind>=1)
			m_output_file=argv[optind++];
		printf("output file: %s\n",m_output_file.c_str());
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

	if (m_workMode == "sasaki"){
		//##########################PRESET############################
		int maxline = 128;
		int current = 1;
		int gradient = 1;
		int normB = 1;
		int normE = 1;

		int flip = -1;
		double scaleB = 1.1;
		double deltaZ = -12.5*m;

		int nR=181;
		int nZ=541;
//		int nR=81;
//		int nZ=221;
		double dR=5*mm;
		double dZ=5*mm;
//		double Z0=10.1095*m;
		double Z0=11.2095*m;

		Z0 += deltaZ;

		std::vector<double> in_x;
		std::vector<double> in_y;
		std::vector<double> in_z;
		std::vector<double> in_bx;
		std::vector<double> in_by;
		std::vector<double> in_bz;

		std::vector<double> out_r;
		std::vector<double> out_z;
		std::vector<double> out_br;
		std::vector<double> out_bz;

		//#######################READMAP################################
		if (m_verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"Read Map from \""<<m_mapfile<<"\""<<std::endl;
		std::stringstream buff;

		std::ifstream fin_card(m_mapfile.c_str());
		if(!fin_card){
			std::cout<<"Cannot find "<<m_mapfile<<std::endl;
			return -1;
		}
		std::string s_card;
		// read file
		int iline = 0;
		while(getline(fin_card,s_card)){
			s_card.erase(s_card.find_last_not_of('\t')+1);
			s_card.erase(0,s_card.find_first_not_of('\t'));
			s_card.erase(s_card.find_last_not_of(' ')+1);
			s_card.erase(0,s_card.find_first_not_of(' '));
			s_card.erase(s_card.find_last_not_of('\r')+1);
			s_card.erase(0,s_card.find_first_not_of('\r'));
			if ( ISEMPTY(s_card) ) continue;
			iline++;
			if ( iline<=1 ){
				continue;
			}
			//if (m_verbose >= Verbose_InputInfo) std::cout<<prefix_InputInfo<<": \""<<s_card<<"\""<<std::endl;
			std::vector<std::string> segments;
			seperate_string(s_card,segments,' ');
			int iterator = 1;
			if(iterator<segments.size()) in_x.push_back(m*string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
			if(iterator<segments.size()) in_y.push_back(m*string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
			if(iterator<segments.size()) in_z.push_back(m*string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
			if(iterator<segments.size()) in_bx.push_back(tesla*string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
			if(iterator<segments.size()) in_by.push_back(tesla*string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
			if(iterator<segments.size()) in_bz.push_back(tesla*string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
		}

		//######################GETMAP###############################
		for (int i = 0; i < in_z.size(); i++ ){
			double r = in_x[i];
			double z = in_z[i]+deltaZ;
			double br = in_bx[i]*scaleB*flip;
			double bz = in_bz[i]*scaleB*flip;
			out_r.push_back(r);
			out_z.push_back(z);
			out_br.push_back(br);
			out_bz.push_back(bz);
		}

		//######################Output###############################
		std::ofstream fout;
		fout.open("output.dat");
		if(!fout){
			std::cout<<"Cannot open output.dat in ofstream format"<<std::endl;
			return -1;
		}

		fout<<"param maxline=128 current=1 gradient=1 normB=1 normE=1"<<std::endl;
		fout<<"cylinder"
			<<" nR="<<nR
			<<" nZ="<<nZ
			<<" dR="<<dR/mm
			<<" dZ="<<dZ/mm
			<<" Z0="<<Z0/mm
			<<std::endl;
		fout<<"data"<<std::endl;
		for (int i = 0; i < out_z.size(); i++ ){
			fout<<out_r[i]/mm<<" "
				<<out_z[i]/mm<<" "
				<<out_br[i]/tesla<<" "
				<<out_bz[i]/tesla
				<<std::endl;
		}
	}// End of sasaki workmode
	else if (m_workMode == "UK" || m_workMode == "UK_line" || m_workMode == "UK_line_r"){
		//##########################PRESET############################
		int maxline = 128;
		int current = 1;
		int gradient = 1;
		int normB = 1;
		int normE = 1;

		double scaleB = 1;

		double deltaX = 7350*mm;
		double deltaZ = -5790.5*mm;

		double X0 = -550*mm;
		double Y0 = -650*mm;
		double Z0 = -6390.5*mm;
		int nX=891;
		int nY=132;
		int nZ=761;
		double dX=10*mm;
		double dY=10*mm;
		double dZ=10*mm;

		double x;
		double y;
		double z;
		double bx;
		double by;
		double bz;

		//######################Output###############################
		std::ofstream fout;
		fout.open(m_output_file.c_str());
		if(!fout){
			std::cout<<"Cannot open output.dat in ofstream format"<<std::endl;
			return -1;
		}
		if (m_workMode == "UK"){
			fout<<"param maxline=128 current=1 gradient=1 normB=1 normE=1"<<std::endl;
			fout<<"grid"
				<<" X0="<<X0/mm
				<<" Y0="<<Y0/mm
				<<" Z0="<<Z0/mm
				<<" nX="<<nX
				<<" nY="<<nY
				<<" nZ="<<nZ
				<<" dX="<<dX/mm
				<<" dY="<<dY/mm
				<<" dZ="<<dZ/mm
				<<std::endl;
			fout<<"data"<<std::endl;
		}
		else if (m_workMode == "UK_line" || m_workMode == "UK_line_r"){
			fout<<"x y z bx by bz"<<std::endl;
		}

		//#######################READMAP################################
		if (m_verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"Read Map from \""<<m_mapfile<<"\""<<std::endl;
		std::stringstream buff;

		std::ifstream fin_card(m_mapfile.c_str());
		if(!fin_card){
			std::cout<<"Cannot find "<<m_mapfile<<std::endl;
			return -1;
		}
		std::string s_card;
		// read file
		while(getline(fin_card,s_card)){
			if ( ISEMPTY(s_card) ) continue;
			if (!StartWithNumber(s_card)) continue;
			//if (m_verbose >= Verbose_InputInfo) std::cout<<prefix_InputInfo<<": \""<<s_card<<"\""<<std::endl;
			std::vector<std::string> segments;
			seperate_string(s_card,segments,' ');
			int iterator = 0;
			if (m_workMode=="UK_line_r"){
				if(iterator<segments.size()) z=-mm*string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
				if(iterator<segments.size()) x=-mm*string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
				if(iterator<segments.size()) y=mm*string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
				if(iterator<segments.size()) bz=tesla*string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
				if(iterator<segments.size()) bx=tesla*string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
				if(iterator<segments.size()) by=tesla*string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
			}
			else{
				if(iterator<segments.size()) x=mm*string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
				if(iterator<segments.size()) y=mm*string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
				if(iterator<segments.size()) z=mm*string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
				if(iterator<segments.size()) bx=tesla*string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
				if(iterator<segments.size()) by=tesla*string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
				if(iterator<segments.size()) bz=tesla*string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; continue;}
			}
			//######################GETMAP###############################
			if (m_workMode == "UK_line" || m_workMode == "UK_line_r"){
				//std::cout<<"Before change: "
				//         <<std::setiosflags(std::ios::left)<<std::setw(10)<<x
				//         <<std::setiosflags(std::ios::left)<<std::setw(10)<<y
				//         <<std::setiosflags(std::ios::left)<<std::setw(10)<<z
				//         <<std::setiosflags(std::ios::left)<<std::setw(10)<<bx
				//         <<std::setiosflags(std::ios::left)<<std::setw(10)<<by
				//         <<std::setiosflags(std::ios::left)<<std::setw(10)<<bz
				//         <<std::endl;
				if (x>13200*mm) continue; // Beyond Phase-1 geometry
				if (z<4350*mm){// SEG1,SEG2
					//if (x!=0||y!=0) continue;
				}
				else if (x<3000*mm){// MT1
					//if (y!=0) continue;
					double r = sqrt((3000*mm-x)*(3000*mm-x)+(z-4350*mm)*(z-4350*mm));
					//if (r<2995*mm||r>3005*mm) continue;
					double phi = atan((z-4350*mm)/(3000*mm-x));
					double length = 3000*mm*phi;
					z=length+4350*mm;
					x=r-3000*mm;
					double bzp=bx*sin(phi)+bz*cos(phi);
					double bxp=-bz*sin(phi)+bx*cos(phi);
					bx=bxp;
					bz=bzp;
				}
				else{// BLT and TS2
					//if (y!=0||z!=7350*mm) continue;
					double length = x-3000*mm;
					x = z-7350*mm;
					z=4350*mm+3000*mm*90*deg/rad+length;
					double bzp=bx;
					double bxp=-bz;
					bz=bzp;
					bx=bxp;
				}
				//std::cout<<" After change: "
				//         <<std::setiosflags(std::ios::left)<<std::setw(10)<<x
				//         <<std::setiosflags(std::ios::left)<<std::setw(10)<<y
				//         <<std::setiosflags(std::ios::left)<<std::setw(10)<<z
				//         <<std::setiosflags(std::ios::left)<<std::setw(10)<<bx
				//         <<std::setiosflags(std::ios::left)<<std::setw(10)<<by
				//         <<std::setiosflags(std::ios::left)<<std::setw(10)<<bz
				//         <<std::endl;
			}
			else if (m_workMode == "UK"){
				if (z>625*mm){// SEG1,SEG2
					continue;
				}
				if (x<3000*mm&&z>4350*mm){// MT1
					by += m_ByOffset*tesla;
				}
				double xp = -z+deltaX;
				double zp = x+deltaZ;
				x = xp;
				z = zp;
				double bxp = -bz*scaleB;
				double byp = by*scaleB;
				double bzp = bx*scaleB;
				bx = bxp;
				by = byp;
				bz = bzp;
			}
			//######################Output###############################
			fout<<x/mm<<" "
				<<y/mm<<" "
				<<z/mm<<" "
				<<bx/tesla<<" "
				<<by/tesla<<" "
				<<bz/tesla
				<<std::endl;
		}
	}
}

void init_args()
{
	m_workMode="sasaki";
	m_output_file="output.dat";
	m_verbose = 0;
	m_ByOffset=0;
	nEvents = 0;
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
	fprintf(stderr,"\t\t m_verbose level\n");
	fprintf(stderr,"\t -n\n");
	fprintf(stderr,"\t\t nEvent\n");
	fprintf(stderr,"\t -p\n");
	fprintf(stderr,"\t\t printModule\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"\t -b\n");
	fprintf(stderr,"\t\t restore backup file.\n");
	fprintf(stderr,"\t -B\n");
	fprintf(stderr,"\t\t Set By offset for MT1 region. Only for UK mode.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m ab -v 20 -n 100\n",prog_name);
}

bool ISEMPTY(std::string content){
	bool flag = false;
	const char* c_card = content.c_str();
	int length = strlen(c_card);
	int offset = 0;
	for ( ; offset < length; offset++ ){
		if ( c_card[offset] != ' ' && c_card[offset]!='\t') break;
	}
	if ( c_card[offset] == '#' || (c_card[offset] == '/' && c_card[offset+1] == '/') || length - offset == 0 ){
		flag = true;
	}
	return flag;
}

bool StartWithNumber(std::string content){
	bool flag = false;
	const char* c_card = content.c_str();
	int length = strlen(c_card);
	int offset = 0;
	for ( ; offset < length; offset++ ){
		if ( c_card[offset] != ' ' && c_card[offset]!='\t') break;
	}
	if ( (c_card[offset] >= '0' && c_card[offset] <= '9') || c_card[offset] == '-' || c_card[offset] == '+' ){
		flag = true;
	}
	return flag;
}

void seperate_string(std::string line, std::vector<std::string> &strs, const char sep ){
	std::string token;
	std::stringstream ss(line);
	if (sep!=' '&&sep!='\t'){
		while(std::getline(ss, token, sep)){
			token.erase(token.find_last_not_of('\t')+1);
			token.erase(0,token.find_first_not_of('\t'));
			token.erase(token.find_last_not_of(' ')+1);
			token.erase(0,token.find_first_not_of(' '));
			token.erase(token.find_last_not_of('\r')+1);
			token.erase(0,token.find_first_not_of('\r'));
			strs.push_back(token);
		}
	}
	else{
		while(ss>>token){
			token.erase(token.find_last_not_of('\t')+1);
			token.erase(0,token.find_first_not_of('\t'));
			token.erase(token.find_last_not_of(' ')+1);
			token.erase(0,token.find_first_not_of(' '));
			token.erase(token.find_last_not_of('\r')+1);
			token.erase(0,token.find_first_not_of('\r'));
			strs.push_back(token);
		}
	}
}

double string2double(std::string str){
	double val;
	std::stringstream ss(str);
	ss>>val;
	//std::cout<<"\""<<str<<"\" -> ("<<val<<")"<<std::endl;
	return val;
}
