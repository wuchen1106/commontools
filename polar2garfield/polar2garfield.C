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

#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TArrow.h"
#include "TAxis.h"
#include "TFile.h"
#include "TPaveText.h"

/* constant */
double PI = 3.141592653589793238;

/* arguments */
char m_config_file[128];
char m_input_file[128];
char m_output_file[128];

// Parameters
std::vector<double> m_para_value;
std::vector<std::string> m_para_identifier;
std::vector<std::string> m_para_name;

void init_args()
{
	strcpy(m_config_file, "config.txt");
	strcpy(m_input_file, "input.txt");
	strcpy(m_output_file, "output.txt");
}
/* end of arguments */

/* begin of functions */
bool is_number(char c){
	if (c>= '0' && c<='9') return true;
	else return false;
}
bool is_name(char c){
	if (c>= '0' && c<='9' ||
			c>= 'a' && c<= 'z' ||
			c>= 'A' && c<= 'Z' ||
			c== '_')
		return true;
	else return false;
}
bool get_formula(std::string line,std::string &formula, std::vector<std::string> &paras){
	bool stillNumber = false;
	bool stillName = false;
	std::string temp = "";
	formula = "";
	//std::cout<<"line = \""<<line<<"\""<<std::endl;
	for ( int i = 0; i < line.size(); i++ ){
		char c = line[i];
		bool isEnd = false;
		bool isNothing = false;
		std::stringstream buf;
		if (is_number(c)){
			if ( !stillName && !stillNumber || stillNumber ) stillNumber = true;
			else stillName = true;
			temp += c;
		}
		else if (is_name(c)){
			stillNumber = false;
			stillName = true;
			temp += c;
		}
		else {
			if ( c == '{' ) c = '(';
			if ( c == '}' ) c = ')';
			isEnd = true;
			isNothing = true;
		}
		if (i==line.size()-1) isEnd = true;
		//std::cout<<"["<<i<<"]: c=\'"<<c<<"\', isNum?"<<is_number(c)<<", isName?"<<is_name(c)<<", isEnd?"<<isEnd<<", temp=\""<<temp<<", formula=\""<<formula<<std::endl;
		if (isEnd) {
			if ( stillNumber ){
				formula += temp;
				//std::cout<<"  stillNumber!"<<std::endl;
			}
			if ( stillName ){
				//std::cout<<"  stillName!"<<std::endl;
				if ( temp == 'i' ){
					formula += 'x';
				}
				else{
					int i_para = -1;
					for ( int j = 0; j < paras.size(); j++ ){
						if ( temp == paras[j] ){
							i_para = j;
						}
					}
					if (i_para == -1){
						i_para = paras.size();
						paras.push_back(temp);
					}
					//std::cout<<"	  paras["<<i_para<<"] = "<<paras[i_para]<<std::endl;
					buf.str("");
					buf.clear();
					buf<<"["<<i_para<<"]";
					formula += buf.str();
				}
			}
			if (isNothing) formula += c;
			temp = "";
			stillName = false;
			stillNumber = false;
		}
	}
	//std::cout<<"formula = \""<<formula<<"\""<<std::endl;
	return true;
}
double get_para(std::string name){
	for ( int i = 0; i < m_para_name.size(); i++ ){
		if ( name == m_para_name[i] ) return m_para_value[i];
	}
	return 0;
}
void print_usage(char* prog_name)
{
	fprintf(stderr,"Usage %s [options] [args]\n",prog_name);
	fprintf(stderr,"[options]\n");
	fprintf(stderr,"\t -c\n");
	fprintf(stderr,"\t\t specify the config file.\n");
	fprintf(stderr,"\t -i\n");
	fprintf(stderr,"\t\t specify the input file.\n");
	fprintf(stderr,"\t -o\n");
	fprintf(stderr,"\t\t specify the output file.\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"[example]\n");
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
	while((result=getopt(argc,argv,"hc:i:o:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'c':
				strcpy(m_config_file,optarg);
				printf("config file: %s\n",m_config_file);
				break;
			case 'i':
				strcpy(m_input_file,optarg);
				printf("input file: %s\n",m_input_file);
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

	// wire info
	std::vector<std::string> m_layer_name;
	std::vector<double> m_layer_radius;
	std::vector<std::string> m_layer_nWire;
	std::vector<std::string> m_layer_nWire_string;
	std::vector<std::vector<std::string> > m_layer_nWire_paras;
	std::vector<std::string> m_layer_phi;
	std::vector<std::string> m_layer_phi_string;
	std::vector<std::vector<std::string> > m_layer_phi_paras;

	std::stringstream buf;
	std::string temp;
	// Read from configure file
	std::ifstream fin_config(m_config_file);
	if(!fin_config){
		std::cout<<"Cannot open "<<m_config_file<<" in ifstream format"<<std::endl;
		return -1;
	}
	while(getline(fin_config,temp)){
		buf.str("");
		buf.clear();
		buf<<temp;
		double value;
		std::string identifier;
		std::string name;
		std::string trash;
		buf>>identifier>>name>>trash>>value;
		m_para_value.push_back(value);
		m_para_identifier.push_back(identifier);
		m_para_name.push_back(name);
	}

	// Read from input file
	std::ifstream fin_input(m_input_file);
	if(!fin_input){
		std::cout<<"Cannot open "<<m_input_file<<" in ifstream format"<<std::endl;
		return -1;
	}
	bool inWireLines = false;
	while(getline(fin_input,temp)){
		//std::cout<<temp<<std::endl;
		buf.str("");
		buf.clear();
		buf<<temp;
		double double1,double2;
		std::string seg1,seg2,seg3,seg4;
		std::string trash;
		buf>>seg1>>seg2;
		bool isParameter = false;
		int i_para = -1;
		for ( int i = 0; i < m_para_value.size(); i++ ){
			//std::cout<<"m_para_identifier["<<i<<"] = "<<m_para_identifier[i]<<", m_para_name["<<i<<"] = "<<m_para_name[i]<<std::endl;
			if ( seg1 == m_para_identifier[i] && seg2 == m_para_name[i] ){
				isParameter = true;
				i_para = i;
				break;
			}
		}
		if (isParameter){
			buf>>trash>>m_para_value[i_para];
			continue;
		}
		else if ( seg1 == "ROWS" && seg2 == "Polar" ) {
			inWireLines = true;
			continue;
		}
		if ( inWireLines ){
			if (seg1.size()==0){
				inWireLines = false;
				break;
			}
			else{
				buf>>trash>>double1>>seg3;
				m_layer_name.push_back(seg1);
				m_layer_radius.push_back(double1);
				std::vector<std::string> vec;
				vec.clear();
				m_layer_nWire_string.push_back(seg2);
				m_layer_nWire.push_back("");
				m_layer_nWire_paras.push_back(vec);
				get_formula(seg2,m_layer_nWire[m_layer_nWire.size()-1],m_layer_nWire_paras[m_layer_nWire_paras.size()-1]);
				m_layer_phi_string.push_back(seg3);
				m_layer_phi.push_back("");
				m_layer_phi_paras.push_back(vec);
				get_formula(seg3,m_layer_phi[m_layer_phi.size()-1],m_layer_phi_paras[m_layer_phi_paras.size()-1]);
			}
		}
	}

	// Open output file
	std::ofstream fout;
	fout.open(m_output_file);
	if(!fout){
		std::cout<<"Cannot open "<<m_output_file<<" in ofstream format"<<std::endl;
		return -1;
	}
	for ( int i_layer = 0; i_layer < m_layer_name.size(); i_layer++ ){
		int nWire = 0;
		TF1 *nWire_formula = new TF1("nWire",m_layer_nWire[i_layer].c_str());
		std::cout<<"["<<i_layer<<"]("<<m_layer_name[i_layer]<<")  nWire = "<<m_layer_nWire[i_layer]<<", radius = "<<m_layer_radius[i_layer]<<", phi = "<<m_layer_phi[i_layer]<<std::endl;
		std::cout<<"      nWire = "<<m_layer_nWire_string[i_layer]<<", radius = "<<m_layer_radius[i_layer]<<", phi = "<<m_layer_phi_string[i_layer]<<std::endl;
		for ( int i_para = 0; i_para < m_layer_nWire_paras[i_layer].size(); i_para++ ){
			nWire_formula->SetParameter(i_para,get_para(m_layer_nWire_paras[i_layer][i_para]));
			std::cout<<"         nWire_para["<<i_para<<"]("<<m_layer_phi_paras[i_layer][i_para]<<") = "<<get_para(m_layer_phi_paras[i_layer][i_para])<<std::endl;
		}
		nWire = nWire_formula->Eval(0);
		delete nWire_formula;
		TF1 *phi_formula = new TF1("phi",m_layer_phi[i_layer].c_str());
		for ( int i_para = 0; i_para < m_layer_phi_paras[i_layer].size(); i_para++ ){
			phi_formula->SetParameter(i_para,get_para(m_layer_phi_paras[i_layer][i_para]));
			std::cout<<"         phi_para["<<i_para<<"]("<<m_layer_phi_paras[i_layer][i_para]<<") = "<<get_para(m_layer_phi_paras[i_layer][i_para])<<std::endl;
		}
		for ( int i_wire = 0; i_wire < nWire; i_wire++ ){
			double phi = phi_formula->Eval(i_wire);
			double x = m_layer_radius[i_layer]*cos(phi*PI/180.);
			double y = m_layer_radius[i_layer]*sin(phi*PI/180.);
			if ( i_wire == 0 ){
				std::cout<<"  ["<<i_wire<<"]"<<"  phi["<<i_wire<<"] = "<<phi<<", x["<<i_wire<<"] = "<<x<<", y["<<i_wire<<"] = "<<y<<std::endl;
			}
			fout<<std::setiosflags(std::ios::left)<<std::setw(5)<<m_layer_name[i_layer]
			   	<<" "<<std::setiosflags(std::ios::left)<<std::setw(5)<<i_wire
			   	<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<x
			   	<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<y
			   	<<std::endl;
		}
		delete phi_formula;
	}

	return 0;
}
