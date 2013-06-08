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
#include "TArrow.h"
#include "TAxis.h"
#include "TFile.h"
#include "TPaveText.h"

/* constant */
double aDay = 24;
double inTime = 9;
double offTime = 22;

/* arguments */
char m_input_file[128];
char m_output_file[128];
char m_output_root[128];
char m_logbook[128];
char m_date[128];
char m_weekday[128];
int m_code_nlines=0;
int m_verbose=0;
bool m_generate=false;

void init_args()
{
	strcpy(m_input_file, "input.txt");
	strcpy(m_output_file, "output.pdf");
	strcpy(m_output_root, "output.root");
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
int check_status(const char* content, std::string &log, double &mark){
	std::string str = content;
	if (is_number(content[9])&&is_number(content[10])){
		log=str.substr(12,std::string::npos);
		int ten=get_number(content[9]);
		int one=get_number(content[10]);
		mark = ten*10+one;
		return 2;
	}
	else if (content[9]=='\t'&&content[10]=='\t'&&content[11]=='\t'){
		return -1;
	}
	else if ( str.find("Check in") < str.length() ||
			 str.find("Back to work") < str.length() ){
		return 1;
	}
	else if ( str.find("Check out") < str.length() ||
			 str.find("Take a rest") < str.length() ){
		return 0;
	}
	else{
		return -1;
	}
}
bool has_time(const char* content){
	if(m_verbose>=20) printf("%c%c%c%c%c%c\n",content[0],content[1],content[3],content[4],content[6],content[7]);
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
	if(m_verbose>=20) printf("%s: %d, %lf\n",content,nsec,time);
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
	fprintf(stderr,"\t -r\n");
	fprintf(stderr,"\t\t specify the output root file.\n");
	fprintf(stderr,"\t -d\n");
	fprintf(stderr,"\t\t specify the date.\n");
	fprintf(stderr,"\t -w\n");
	fprintf(stderr,"\t\t specify weekday.\n");
	fprintf(stderr,"\t -n\n");
	fprintf(stderr,"\t\t specify how many lines have been modified.\n");
	fprintf(stderr,"\t -v\n");
	fprintf(stderr,"\t\t specify verbose.\n");
	fprintf(stderr,"\t -g\n");
	fprintf(stderr,"\t\t Generate files from logbook.\n");
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
	while((result=getopt(argc,argv,"hgi:o:l:r:d:w:n:v:"))!=-1){
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
			case 'r':
				strcpy(m_output_root,optarg);
				printf("output root file: %s\n",m_output_root);
				break;
			case 'd':
				strcpy(m_date,optarg);
				printf("date: %s\n",m_date);
				break;
			case 'w':
				strcpy(m_weekday,optarg);
				printf("weekday: %s\n",m_weekday);
				break;
			case 'n':
				m_code_nlines = atoi(optarg);
				printf("nlines: %d\n",m_code_nlines);
				break;
			case 'v':
				m_verbose = atoi(optarg);
				printf("verbose: %d\n",m_verbose);
				break;
			case 'g':
				m_generate = true;
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
	int nbin = 3600*48;
	int left = 0;
	int right = 48;
	TH1D *h1d1 = new TH1D("ActiveChart","ActiveChart",nbin,left,right);
	TH1D *h1d2 = new TH1D("ActiveChart","ActiveChart",nbin,left,right);
	TH1D *h1d3 = new TH1D("ActiveChart","ActiveChart",nbin,left,right);

	// Read the file
	double late_time = 0;
	double early_time = 0;
	double duration = 0;
	double checkin_time = 0;
	double checkout_time = 0;
	int rest_num = 0;
	std::vector<double> node_mark;
	std::vector<double> node_time;
	std::vector<int> node_type;
	std::vector<std::string> node_cont;
	char* buf = (char *)malloc(2048);
	double prev_time = 0;
	while(fgets(buf,2048,fpi)){
		if (has_time(buf)){
			double cur_time = get_time(buf);
			if (cur_time < prev_time) cur_time += aDay; // must be overnight
			if(m_verbose>=15) printf("cur_time = %lf\n",cur_time);
			std::string log;
			double mark = 0;
			int status = check_status(buf,log,mark);
			if ( status == 0 ){ // End of work
				rest_num++;
			}
			else if ( status == -1 ){ // useless
				continue;
			}
			node_time.push_back(cur_time);
			node_type.push_back(status);
			node_mark.push_back(mark);
			node_cont.push_back(log);
			if(m_verbose>=20) printf("%lf:%d,%lf\n",cur_time,status,mark);
			prev_time = cur_time;
		}
		else{
			if(m_verbose>=15) printf("\"%s\" does not contain time\n",buf);
		}
	}
	checkin_time = node_time[0];
	checkout_time = node_time[node_time.size()-1];
	late_time = checkin_time - inTime;
	early_time = offTime - checkout_time ;
	duration = checkout_time - checkin_time;
	rest_num--; // Checkout is not a rest

	// Fill histograms
	double total_work_time = 0;
	double total_rest_time = 0;
	double average_mark = 0;
	double Karma = 0;
	prev_time = 0;
	double prev_mark = 0;
	int prev_type = 0;
	TH1D *h1d_temp = h1d1;
	for ( int inode = 0; inode < node_time.size(); inode++ ){
		int cur_type = node_type[inode];
		double cur_time = node_time[inode];
		double cur_mark = prev_mark;
		if ( cur_type == 2 ){
			cur_mark = node_mark[inode]/100.;
		}
		if(m_verbose>=20) printf("%lf: %d, %lf\n",cur_time,cur_type,cur_mark);
		double delta_time = cur_time - prev_time;
		int ibin_start = h1d_temp->FindBin(prev_time);
		int ibin_end = h1d_temp->FindBin(cur_time);
		double val1 = 0;
		double val2 = 0;
		if ( prev_type == 0 && cur_type == 1 ){ // end - start
			val1 = 0;
			val2 = 0;
			if (inode != 0) total_rest_time += delta_time;
		}
		else if ( prev_type == 1 && cur_type == 0  // start - end
		       || prev_type == 1 && cur_type == 2  // start - tag
		       || prev_type == 2 && cur_type == 0  // tag - end
		       || prev_type == 2 && cur_type == 2 ){ // tag - tag
			val1 = 1;
			val2 = prev_mark;
			total_work_time += delta_time;
			Karma += delta_time * prev_mark;
		}
		else{
			if(m_verbose>=20) printf("[%d,%lf], [%d,%lf] does not match any type!\n",prev_type,prev_time,cur_type,cur_time);
		}
		for ( int ibin = ibin_start; ibin < ibin_end; ibin++ ){
			if ( h1d_temp == h1d1 ){
				if (get_time(ibin) > offTime){ // Crossed offTime
					h1d_temp = h1d2;
				}
			}
			h1d_temp->SetBinContent(ibin,val1);
			h1d3->SetBinContent(ibin,val2);
		}
		prev_time = cur_time;
		prev_type = cur_type;
		prev_mark = cur_mark;
	}
	average_mark = Karma/total_work_time;

	// About output
	TFile *file = new TFile(m_output_root,"RECREATE");

	// Draw histograms
	std::stringstream buf_label;
	gStyle->SetOptStat(0);
	TCanvas *canv = new TCanvas("c","c");
	canv->SetBottomMargin(0.5);
	h1d1->SetLineColor(kGreen);
	h1d1->SetFillColor(kGreen);
	h1d2->SetLineColor(kRed);
	h1d2->SetFillColor(kRed);
	h1d3->SetLineColor(kBlue);
	h1d3->SetFillColor(kBlue);
	h1d1->GetXaxis()->SetLabelSize(0.02);
	h1d1->SetTickLength(0,"X");
	for ( int i = 0; i < node_cont.size(); i++ ){
		if ( node_type[i] != 2 ) continue;
		int ibin = h1d1->FindBin(node_time[i]);
		h1d1->GetXaxis()->SetBinLabel(ibin,node_cont[i].c_str());
	}
	for ( int i = 0; i < 48; i++ ){
		buf_label.clear();
		buf_label.str("");
		buf_label<<i;
		int ibin = h1d1->FindBin(i);
		h1d1->GetXaxis()->SetBinLabel(ibin,buf_label.str().c_str());
	}
	double max_time = checkout_time > 24 ? checkout_time: 24; 
	//printf("checkout_time = %lf, max_time = %lf\n",checkout_time,max_time);
	h1d1->GetXaxis()->SetRangeUser(0,max_time);
	h1d1->Draw("LF2");
	h1d2->Draw("LF2SAME");
	h1d3->Draw("LF2SAME");
	// Draw some arrows
	double x1 = inTime;
	double x2 = inTime;
	double y1 = 0;
	double y2 = 1;
	TArrow *arrow = new TArrow(x1,y1,x2,y2,0.01,"<|");
	arrow->SetFillColor(kRed);
	arrow->SetLineColor(kRed);
	arrow->Draw("SAME");
	arrow->Write();
	for ( int i = 0; i < node_time.size(); i++ ){
		if ( node_type[i] != 2 ) continue;
		x1 = node_time[i];
		x2 = node_time[i];
		y1 = 0;
		y2 = 1;
		arrow = new TArrow(x1,y1,x2,y2,0.01,"<|");
		arrow->Draw("SAME");
		arrow->Write();
	}
	// Draw some words
	TPaveText *pt = new TPaveText(0.02,0.02,0.2,0.4,"brNDC");
	pt->SetFillColor(18);
	pt->SetTextAlign(12);
	pt->SetTextSize(0.02);
	buf_label.clear();
	buf_label.str("");
	buf_label<<m_weekday<<" "<<m_date<<std::endl;
	pt->AddText(buf_label.str().c_str());
	buf_label.clear();
	buf_label.str("");
	buf_label<<"Check in time:  "<<checkin_time<<std::endl;
	pt->AddText(buf_label.str().c_str());
	buf_label.clear();
	buf_label.str("");
	buf_label<<"Check out time: "<<checkout_time<<std::endl;
	pt->AddText(buf_label.str().c_str());
	buf_label.clear();
	buf_label.str("");
	buf_label<<"Duration:       "<<duration<<std::endl;
	pt->AddText(buf_label.str().c_str());
	buf_label.clear();
	buf_label.str("");
	buf_label<<"Rest times:     "<<rest_num<<std::endl;
	pt->AddText(buf_label.str().c_str());
	buf_label.clear();
	buf_label.str("");
	buf_label<<"Total work time:"<<total_work_time<<std::endl;
	pt->AddText(buf_label.str().c_str());
	buf_label.clear();
	buf_label.str("");
	buf_label<<"Total rest time:"<<total_rest_time<<std::endl;
	pt->AddText(buf_label.str().c_str());
	buf_label.clear();
	buf_label.str("");
	buf_label<<"Average mark:   "<<average_mark<<std::endl;
	pt->AddText(buf_label.str().c_str());
	buf_label.clear();
	buf_label.str("");
	buf_label<<"Karma:          "<<Karma<<std::endl;
	pt->AddText(buf_label.str().c_str());
	pt->Draw();
	// Print
	canv->Print(m_output_file);

	// Save histograms
	h1d1->Write();
	h1d2->Write();
	h1d3->Write();
	file->Close();

	// Record main info to logbook
	int nDays = 0;
	std::vector<double> lb_nlines;
	std::vector<std::string> lb_date;
	std::vector<std::string> lb_weekday;
	std::vector<double> lb_Karma;
	std::vector<double> lb_mark;
	std::vector<double> lb_late;
	std::vector<double> lb_early;
	std::vector<double> lb_duration;
	std::vector<double> lb_work;
	std::vector<double> lb_rest;
	double t_lb_nlines;
	std::string t_lb_date;
	std::string t_lb_weekday;
	double t_lb_Karma;
	double t_lb_mark;
	double t_lb_late;
	double t_lb_early;
	double t_lb_duration;
	double t_lb_work;
	double t_lb_rest;
	std::string buffer;
	std::string logbook = m_logbook;
	if ( logbook != "NONE" ){
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
			if ( nDays > 0 ){
				buf_log_temp>>buffer>>t_lb_date>>t_lb_weekday>>buffer>>buffer>>t_lb_duration>>t_lb_work>>t_lb_rest>>t_lb_mark>>t_lb_Karma>>t_lb_late>>t_lb_early>>buffer>>t_lb_nlines;
				lb_nlines.push_back(t_lb_nlines);
				lb_date.push_back(t_lb_date);
				lb_weekday.push_back(t_lb_weekday);
				lb_Karma.push_back(t_lb_Karma);
				lb_mark.push_back(t_lb_mark);
				lb_late.push_back(t_lb_late);
				lb_early.push_back(t_lb_early);
				lb_duration.push_back(t_lb_duration);
				lb_work.push_back(t_lb_work);
				lb_rest.push_back(t_lb_rest);
			}
			nDays++;
		}
		lb_nlines.push_back(m_code_nlines);
		lb_date.push_back(m_date);
		lb_weekday.push_back(m_weekday);
		lb_Karma.push_back(Karma);
		lb_mark.push_back(average_mark);
		lb_late.push_back(late_time);
		lb_early.push_back(early_time);
		lb_duration.push_back(duration);
		lb_work.push_back(total_work_time);
		lb_rest.push_back(total_rest_time);
		int log_num = nDays;
		std::ofstream fout;
		fout.open(m_logbook);
		if(!fout){
			std::cout<<"Cannot open "<<m_logbook<<" in ofstream format"<<std::endl;
			return -1;
		}
		fout<<buf_log.str();
		fout<<std::setiosflags(std::ios::left)<<std::setw(5)<<log_num
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<m_date
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<m_weekday
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<checkin_time
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<checkout_time
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<duration
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<total_work_time
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<total_rest_time
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<average_mark
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<Karma
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<late_time
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<early_time
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(10)<<rest_num
				<<" "<<std::setiosflags(std::ios::left)<<std::setw(5)<<m_code_nlines
				<<std::endl;
		buf_log.str("");
		buf_log.clear();
		fout.close();
	}

	// Generate histograms according to logbook
	if ( m_generate ){
		TH1D *h1d_nlines = new TH1D("nlines","Number of lines you modified",nDays,0,1);
		TH1D *h1d_Karma = new TH1D("Karma","Karma",nDays,0,1);
		TH1D *h1d_mark = new TH1D("mark","Average Mark",nDays,0,1);
		TH1D *h1d_duration = new TH1D("duration","Time Statistics",nDays,0,1);
		TH1D *h1d_late = new TH1D("late","late",nDays,0,1);
		TH1D *h1d_early = new TH1D("early","early",nDays,0,1);
		TH1D *h1d_work = new TH1D("work","work",nDays,0,1);
		TH1D *h1d_rest = new TH1D("rest","rest",nDays,0,1);
		for ( int i = 1; i <= nDays; i++ ){
			std::string ilabel = lb_date[i-1] + " " + lb_weekday[i-1];
			h1d_nlines->GetXaxis()->SetBinLabel(i,ilabel.c_str());
			h1d_Karma->GetXaxis()->SetBinLabel(i,ilabel.c_str());
			h1d_mark->GetXaxis()->SetBinLabel(i,ilabel.c_str());
			h1d_late->GetXaxis()->SetBinLabel(i,ilabel.c_str());
			h1d_early->GetXaxis()->SetBinLabel(i,ilabel.c_str());
			h1d_duration->GetXaxis()->SetBinLabel(i,ilabel.c_str());
			h1d_work->GetXaxis()->SetBinLabel(i,ilabel.c_str());
			h1d_rest->GetXaxis()->SetBinLabel(i,ilabel.c_str());
			h1d_nlines->SetBinContent(i,lb_nlines[i-1]);
			h1d_Karma->SetBinContent(i,lb_Karma[i-1]);
			h1d_mark->SetBinContent(i,lb_mark[i-1]);
			h1d_late->SetBinContent(i,lb_late[i-1]);
			h1d_early->SetBinContent(i,lb_early[i-1]);
			h1d_duration->SetBinContent(i,lb_duration[i-1]);
			h1d_work->SetBinContent(i,lb_work[i-1]);
			h1d_rest->SetBinContent(i,lb_rest[i-1]);
		}
		// Save them
//		h1d_nlines->GetYaxis()->SetNdivisions(512);
//		h1d_Karma->GetYaxis()->SetNdivisions(512);
//		h1d_mark->GetYaxis()->SetNdivisions(512);
//		h1d_late->GetYaxis()->SetNdivisions(512);
//		h1d_early->GetYaxis()->SetNdivisions(512);
//		h1d_duration->GetYaxis()->SetNdivisions(512);
//		h1d_work->GetYaxis()->SetNdivisions(512);
//		h1d_rest->GetYaxis()->SetNdivisions(512);
//		h1d_nlines->GetXaxis()->SetNdivisions(nDays);
//		h1d_Karma->GetXaxis()->SetNdivisions(nDays);
//		h1d_mark->GetXaxis()->SetNdivisions(nDays);
//		h1d_late->GetXaxis()->SetNdivisions(nDays);
//		h1d_early->GetXaxis()->SetNdivisions(nDays);
//		h1d_duration->GetXaxis()->SetNdivisions(nDays);
//		h1d_work->GetXaxis()->SetNdivisions(nDays);
//		h1d_rest->GetXaxis()->SetNdivisions(nDays);
		h1d_nlines->SetMarkerStyle(3);
		h1d_Karma->SetMarkerStyle(3);
		h1d_mark->SetMarkerStyle(3);
		h1d_late->SetMarkerStyle(3);
		h1d_early->SetMarkerStyle(3);
		h1d_duration->SetMarkerStyle(3);
		h1d_work->SetMarkerStyle(3);
		h1d_rest->SetMarkerStyle(3);
		h1d_nlines->SetMarkerSize(0.5);
		h1d_Karma->SetMarkerSize(0.5);
		h1d_mark->SetMarkerSize(0.5);
		h1d_late->SetMarkerSize(0.5);
		h1d_early->SetMarkerSize(0.5);
		h1d_duration->SetMarkerSize(0.5);
		h1d_work->SetMarkerSize(0.5);
		h1d_rest->SetMarkerSize(0.5);
		std::string logbook_root = m_logbook;
		logbook_root += ".root";
		std::string logbook_pic = m_logbook;
		logbook_pic += ".pdf";
		TFile file_output( logbook_root.c_str(), "RECREATE" );
		TCanvas *lb_canv = new TCanvas("c2","c2");
		lb_canv->Divide(2,2);
//		gStyle->SetPadGridX(kTRUE);
//		gStyle->SetPadGridY(kTRUE);
		lb_canv->cd(1);
		double time_max = -48;
		double time_min = 48;
		if ( time_max < h1d_duration->GetMaximum() ) time_max = h1d_duration->GetMaximum();
		if ( time_min > h1d_duration->GetMinimum() ) time_min = h1d_duration->GetMinimum();
		if ( time_max < h1d_late->GetMaximum() ) time_max = h1d_late->GetMaximum();
		if ( time_min > h1d_late->GetMinimum() ) time_min = h1d_late->GetMinimum();
		if ( time_max < h1d_early->GetMaximum() ) time_max = h1d_early->GetMaximum();
		if ( time_min > h1d_early->GetMinimum() ) time_min = h1d_early->GetMinimum();
		if ( time_max < h1d_work->GetMaximum() ) time_max = h1d_work->GetMaximum();
		if ( time_min > h1d_work->GetMinimum() ) time_min = h1d_work->GetMinimum();
		if ( time_max < h1d_rest->GetMaximum() ) time_max = h1d_rest->GetMaximum();
		if ( time_min > h1d_rest->GetMinimum() ) time_min = h1d_rest->GetMinimum();
		if ( time_max > 0 ) time_max *= 1.1;
		else time_max *= 0.9;
		if ( time_min > 0 ) time_min *= 0.9;
		else time_min *= 1.1;
		h1d_duration->GetYaxis()->SetRangeUser(time_min,time_max);
		h1d_late->SetMarkerColor(kRed);
		h1d_early->SetMarkerColor(kOrange);
		h1d_work->SetMarkerColor(kBlue);
		h1d_rest->SetMarkerColor(kGreen);
		h1d_late->SetLineColor(kRed);
		h1d_early->SetLineColor(kOrange);
		h1d_work->SetLineColor(kBlue);
		h1d_rest->SetLineColor(kGreen);
		h1d_duration->Draw("CP");
		h1d_late->Draw("SAMECP");
		h1d_early->Draw("SAMECP");
		h1d_work->Draw("SAMECP");
		h1d_rest->Draw("SAMECP");
		lb_canv->cd(2);
		h1d_mark->Draw("CP");
		lb_canv->cd(3);
		h1d_Karma->Draw("CP");
		lb_canv->cd(4);
		h1d_nlines->Draw("CP");
		lb_canv->Print(logbook_pic.c_str());
		h1d_nlines->Write();
		h1d_Karma->Write();
		h1d_mark->Write();
		h1d_late->Write();
		h1d_early->Write();
		h1d_duration->Write();
		h1d_work->Write();
		h1d_rest->Write();
		file_output.Close();
	}

	return 0;
}
