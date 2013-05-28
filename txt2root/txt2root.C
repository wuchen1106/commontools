#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //getopt

/* arguments */
enum workmode{
	txt2root,
	bin2root,
	txt2bin,
	bin2txt
} m_workmode;
char m_intput_file[128];
char m_output_file[128];
/* end of arguments */

void init_args()
{
	m_workmode = txt2root;
	strcpy(m_intput_file, "input.txt");
	strcpy(m_output_file, "output.root");
}

void print_usage(char* prog_name)
{
	fprintf(stderr,"Usage %s [options] InputFile [OutputFile]\n",prog_name);
	fprintf(stderr,"[options]\n");
	fprintf(stderr,"\t -i\n");
	fprintf(stderr,"\t\t specify the input file.\n");
	fprintf(stderr,"\t -o\n");
	fprintf(stderr,"\t\t specify the output file.\n");
	fprintf(stderr,"\t -t\n");
	fprintf(stderr,"\t\t Generate root file directly from text file. NOTE: InputFile should be text file!!!\n");
	fprintf(stderr,"\t -b\n");
	fprintf(stderr,"\t\t Generate root file directly from binary file. NOTE: InputFile should be binary file!!!\n");
	fprintf(stderr,"\t -T\n");
	fprintf(stderr,"\t\t Generate binary file from text file. NOTE: InputFile should be text file!!!\n");
	fprintf(stderr,"\t -B\n");
	fprintf(stderr,"\t\t Generate test file from binary file. NOTE: InputFile should be binary file!!!\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t(1) Generate a root file called output.root from input.txt\n");
	fprintf(stderr,"\t\t%s -t -i input.txt -o output.root\n",prog_name);
	fprintf(stderr,"\t(2) Generate a root file called output.root from input.bin\n");
	fprintf(stderr,"\t\t%s -b -i input.bin -o output.root\n",prog_name);
	fprintf(stderr,"\t(3) Generate a binary file called output.bin from input.txt\n");
	fprintf(stderr,"\t\t%s -T -i input.txt -o output.bin\n",prog_name);
	fprintf(stderr,"\t(4) Generate a text file called output.text from input.bin\n");
	fprintf(stderr,"\t\t%s -B -i input.bin -o output.text\n",prog_name);
}

int main(int argc, char** argv){
	if (argc==1) {
		print_usage(argv[0]);
		return -1;
	}
	init_args();

	int result;
	while((result=getopt(argc,argv,"tbTBi:o:"))!=-1){
		switch(result){
			/* INPUTS */
			case 't':
				m_workmode = txt2root;
				printf("WorkMode: text to root\n");
				break;
			case 'b':
				m_workmode = bin2root;
				printf("WorkMode: binary to root\n");
				break;
			case 'T':
				m_workmode = txt2bin;
				printf("WorkMode: text to binary\n");
				break;
			case 'B':
				m_workmode = bin2txt;
				printf("WorkMode: binary to text\n");
				break;
			case 'i':
				strcpy(m_intput_file,optarg);
				printf("input file: %s\n",m_intput_file);
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
				exit(1);
		}
	}

	return 0;
}
