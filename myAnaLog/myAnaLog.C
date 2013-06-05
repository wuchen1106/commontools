#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //getopt
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <fcntl.h>

#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"

/* constant */
double aDay = 24;
double offTime = 22;

/* arguments */
char m_input_file[128];
char m_output_file[128];
char m_logbook[128];
char m_date[128];
int m_code_nlines=0;

void init_args()
{
	strcpy(m_input_file, "input.txt");
	strcpy(m_output_file, "output.root");
}
/* end of arguments */

/* begin of functions */
bool is_number(char c){
	if (c>= '0' && c<='9') return true;
	else return false;
}
int get_number(char c){
	int i = c - '0';
	return i;
}
int check_status(const char* content){
	std::string str = content;
	if ( str.find("Check in") < str.length() ||
			 str.find("Back to work") < str.length() ){
		return 1;
	}
	else if ( str.find("Check out") < str.length() ||
			 str.find("Take a rest") < str.length() ){
		return 0;
	}
	else return -1;
}
bool has_time(const char* content){
	//printf("%c%c%c%c%c%c\n",content[0],content[1],content[3],content[4],content[6],content[7]);
	if ( is_number(content[0]) &&
			 is_number(content[1]) &&
			 content[2] == ':' &&
			 is_number(content[3]) &&
			 is_number(content[4]) &&
			 content[5] == ':' &&
			 is_number(content[6]) &&
			 is_number(content[7])	){
		return true;
	}
	else{
		return false;
	}
}
double get_time(int isec){
	return (isec-1.)/3600.;
}
double get_time(const char* content){
	int h1 = get_number(content[0]);
	int h2 = get_number(content[1]);
	int m1 = get_number(content[3]);
	int m2 = get_number(content[4]);
	int s1 = get_number(content[6]);
	int s2 = get_number(content[7]);
	int nsec = (h1*10+h2)*3600 + (m1*10+m2)*60 + s1*10 + s2;
	double time = (double) nsec/3600.;
	//printf("%s: %d, %lf\n",content,nsec,time);
	return time;
}
void print_usage(char* prog_name)
{
	fprintf(stderr,"Usage %s [options] [args]\n",prog_name);
	fprintf(stderr,"[options]\n");
	fprintf(stderr,"\t -i\n");
	fprintf(stderr,"\t\t specify the input file.\n");
	fprintf(stderr,"\t -o\n");
	fprintf(stderr,"\t\t specify the output file.\n");
	fprintf(stderr,"\t -l\n");
	fprintf(stderr,"\t\t specify the logbook.\n");
	fprintf(stderr,"\t -d\n");
	fprintf(stderr,"\t\t specify the date.\n");
	fprintf(stderr,"\t -n\n");
	fprintf(stderr,"\t\t specify how many lines have been modified.\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t(1) Generate a picture file called output.pdf from input.txt\n");
	fprintf(stderr,"\t\t%s -i input.txt -o output.pdf\n",prog_name);
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
	while((result=getopt(argc,argv,"hi:o:l:d:n:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'i':
				strcpy(m_input_file,optarg);
				printf("input file: %s\n",m_input_file);
				break;
			case 'o':
				strcpy(m_output_file,optarg);
				printf("output file: %s\n",m_output_file);
				break;
			case 'l':
				strcpy(m_logbook,optarg);
				printf("logbook: %s\n",m_logbook);
				break;
			case 'd':
				strcpy(m_date,optarg);
				printf("date: %s\n",m_date);
				break;
			case 'n':
				m_code_nlines = atoi(optarg);
				printf("nlines: %d\n",m_code_nlines);
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

	// Open the file
	FILE* fpi = 0;
	fpi = fopen(m_input_file,"r+");
	if ( !fpi ){
		fprintf(stderr,"Cannot open file \"%s\"!!!\n",m_input_file);
		return -1;
	}

	// Create a histogram
	int nbin1 = 3600*24;
	int left1 = 0;
	int right1 = 24;
	TH1D *h1d1 = new TH1D("ActiveChart","ActiveChart",nbin1,left1,right1);
	int nbin2 = 3600*48;
	int left2 = 0;
	int right2 = 48;
	TH1D *h1d2 = new TH1D("ActiveChart","ActiveChart",nbin2,left2,right2);

	// Read the file
	int rest_num = 0;
	bool workOvernight = false;
	double total_work_time = 0;
	double total_rest_time = 0;
	std::vector<double> work_time;
	std::vector<double> rest_time;
	char* buf = (char *)malloc(2048);
	double cur_time = 0; // in hour
	int ibin = 1;
	TH1D *h1d_temp = h1d1;
	while(fgets(buf,2048,fpi)){
		//printf("Got Line: %s\n",buf);
		if (has_time(buf)){
			//printf("Has time!\n");
			cur_time = get_time(buf);
			if (cur_time < get_time(ibin)) cur_time += aDay;
			//printf("cur_time = %lf\n",cur_time);
			int status = check_status(buf);
			if ( status == 1 ){ // Start of work
				if (total_work_time>0) total_rest_time += (cur_time-get_time(ibin));
				//printf("Start at %lf\n",cur_time);
				for ( ; get_time(ibin) < cur_time && ibin <= nbin2; ibin++ ){
					if (get_time(ibin) > offTime){
						workOvernight = true;
						h1d_temp = h1d2;
					}
					h1d_temp->SetBinContent(ibin,0);
				}
			}
			else if ( status == 0 ){ // End of work
				rest_num++;
				total_work_time += (cur_time-get_time(ibin));
				//printf("Stop at %lf\n",cur_time);
				for ( ; get_time(ibin) < cur_time && ibin <= nbin2; ibin++ ){
					if (get_time(ibin) > offTime){
						workOvernight = true;
						h1d_temp = h1d2;
					}
					h1d_temp->SetBinContent(ibin,1);
				}
			}
			else{
				//printf("cannot recognize \"%s\"\n",buf);
			}
		}
		else{
			printf("\"%s\" does not contain time\n",buf);
		}
	}

	// Print the histogram
	gStyle->SetOptStat(0);
	TCanvas *canv = new TCanvas("c","c");
	h1d1->SetLineColor(kBlue);
	h1d1->SetFillColor(kBlue);
	h1d2->SetLineColor(kRed);
	h1d2->SetFillColor(kRed);
	if (workOvernight){
		//printf("cur_time = %lf, ibin = %d\n",cur_time,ibin);
		h1d2->GetXaxis()->SetRangeUser(0,cur_time);
		h1d2->Draw("LF2");
		h1d1->Draw("LF2SAME");
	}
	else{
		h1d1->Draw("LF2");
	}
	canv->Print(m_output_file);

	// Record main info to logbook
  bool isOK = ( access( m_logbook, 0 ) == 0 );
  if (!isOK){
  	std::cout<<"File "<<m_logbook<<" dose not exist!!!"<<std::endl;
		return -1;
	}
  int fd = 0;
  if (fd = open(m_logbook, O_RDWR|O_CREAT) < 0){
  	std::cout<<"Cannnot open file "<<m_logbook<<std::endl;
		return -1;
	}
	std::ifstream fin_log(m_logbook);
	if(!fin_log){
		std::cout<<"Cannot open "<<m_logbook<<" in ifstream format"<<std::endl;
		return -1;
	}
	std::stringstream buf_log;
	std::stringstream buf_log_temp;
	buf_log.clear();
	buf_log.str("");
	std::string s_log;
	while(getline(fin_log,s_log)){
		buf_log_temp.clear();
		buf_log_temp.str("");
		buf_log_temp<<s_log;
		buf_log<<s_log<<"\n";
	}
	int log_num;
	buf_log_temp >> log_num;
	log_num++;
	std::ofstream fout;
	fout.open(m_logbook);
	if(!fout){
		std::cout<<"Cannot open "<<m_logbook<<" in ofstream format"<<std::endl;
		return -1;
	}
	fout<<buf_log.str();
	fout<<std::setiosflags(std::ios::left)<<std::setw(5)<<log_num
		  <<" "<<std::setiosflags(std::ios::left)<<std::setw(15)<<m_date
		  <<" "<<std::setiosflags(std::ios::left)<<std::setw(15)<<total_work_time
		  <<" "<<std::setiosflags(std::ios::left)<<std::setw(15)<<total_rest_time
		  <<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<rest_num
		  <<" "<<std::setiosflags(std::ios::left)<<std::setw(5)<<m_code_nlines
		  <<std::endl;
	buf_log.str("");
	buf_log.clear();
	fout.close();

	return 0;
}
