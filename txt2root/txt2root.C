#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <cstring>
#include <sstream>
#include <unistd.h> //getopt

#include "TFile.h"
#include "TTree.h"

/* arguments */
enum workmode{
    txt2root,
    bin2root,
    txt2bin,
    bin2txt
} m_workmode;
char m_input_file[256];
char m_output_file[256];
int nchars = 256;
std::stringstream buffer;

void init_args()
{
    m_workmode = txt2root;
    strcpy(m_input_file, "input.txt");
    strcpy(m_output_file, "output.root");
}
/* end of arguments */

/* begin of functions */
int get_names(const char* line, char** names, int *inames){
    //fprintf(stderr,"This is get_names! First line is:\n%s\n",line);
    *inames = 0;
    int count = 0;
    bool isC = false;
    int ic = -1;
    while(ic++<20480){
        char c = line[ic];
        //fprintf(stderr,"c[%d]=%c\n",ic,c);
        if ( c == ' ' || c == '\t' || c == '\r' ){
            if ( isC ){
                names[*inames][count] = '\0';
                (*inames)++;
                count = 0;
                isC = false;
            }
            continue;
        }
        else if ( c == '\n' ){
            if ( isC ){
                names[*inames][count] = '\0';
                (*inames)++;
                count = 0;
                isC = false;
            }
            break;
        }
        else{
            isC = true;
            names[*inames][count] = c;
            count++;
        }
    }
    //for ( int i = 0; i < *inames; i++ ){
    //    fprintf(stderr,"name[%d]:%s\n",i,names[i]);
    //}
    return 0;
}

void SetBranches(int inames, char* names[1000],TTree *d_tree,double* values_double, int* values_int, char* values_char, float* values_float, bool* values_bool){
    for ( int i = 0; i < inames; i++ ){
        std::string type = names[i];
        std::string name = names[i];
        int first_slash = type.find_first_of('/');
        if (first_slash>=0&&first_slash<=type.length()){
            type.erase(0,first_slash);
            name.erase(first_slash,name.length());
        }
        else
            type = "/D";

        if (type=="/D")
            d_tree->Branch(name.c_str(),&values_double[i],type.c_str());
        else if (type=="/I")
            d_tree->Branch(name.c_str(),&values_int[i],type.c_str());
        else if (type=="/C")
            d_tree->Branch(name.c_str(),&values_char[i*nchars],type.c_str());
        else if (type=="/F")
            d_tree->Branch(name.c_str(),&values_float[i],type.c_str());
        else if (type=="/O")
            d_tree->Branch(name.c_str(),&values_bool[i],type.c_str());
    }
}

int SetValues(int inames, char* names[1000],TTree *d_tree,double* values_double, int* values_int, char* values_char, float* values_float, bool* values_bool){
    for ( int ival = 0; ival < inames; ival++ ){
        double aval;
        std::string astr = names[ival];
        buffer.str("");
        buffer.clear();
        buffer<<astr;
        buffer>>aval;
        values_int[ival] = aval;
        values_double[ival] = aval;
        values_float[ival] = aval;
        values_bool[ival] = aval;
        //values_char[ival*nchars] = astr.c_str();
        std::strcpy(values_char+ival*nchars,astr.c_str());
        //printf("%d %s -> %f\n",ival,names[ival],values_double[ival]);
    }
    d_tree->Fill();
    return 0;
}

int txt_to_root(const char* input_file, const char* output_file){
    //fprintf(stderr,"This is txt_to_root!\n");
    FILE* fpi = 0;
    fpi = fopen(input_file,"r+");
    if ( !fpi ){
        fprintf(stderr,"Cannot open file \"%s\"!!!\n",input_file);
        return -1;
    }

    char* buf = (char *)malloc(20480);
    char* names[1000];
    for ( int i = 0; i < 1000; i++ ){
        names[i] = (char *) malloc(1000); // up to 1000 names with 1000 charectors inside
    }
    int inames = 0;
    int nBranches = 0;
    TTree* d_tree = 0;
    double* values_double = (double *) malloc(1000);
    int* values_int = (int*) malloc(1000);
    char* values_char = (char*) malloc(nchars*1000);
    float* values_float= (float*) malloc(1000);
    bool* values_bool= (bool*) malloc(1000);
    int iline = 0;

    bool gotit = false;
    TFile file_output( output_file, "RECREATE" );
    while(fgets(buf,20480,fpi)){
        iline++;
        int length = strlen(buf);
        int offset = 0;
        for ( ; offset < length; offset++ ){
            if ( buf[offset] != ' ' ) break;
        }
        if ( buf[offset] == '#' ) continue;
        else if ( buf[offset] == '/' && buf[offset+1] == '/' ) continue;
        else if ( length - offset == 0 ) continue;
        get_names(buf,names,&inames);
        if ( inames > 1000 ){
            fprintf(stderr,"More than 1000 names!\n");
            return -1;
        }

        if (inames == 2 && (std::string)(names[0]) == "TTREE"){ // assign the name to a tree
            printf("New tree: %s\n",names[1]);
            if (d_tree){ // there is a tree before
                d_tree->Write();
                delete d_tree;
                d_tree = 0;
            }
            d_tree = new TTree( names[1], names[1] );
            gotit = false; // need branch info
            continue;
        }

        if (!gotit){ // still need branch info
            if (!d_tree){ // no tree exists, create a default one
                d_tree = new TTree( "t", "t" );
            }
            SetBranches(inames,names,d_tree,values_double,values_int,values_char,values_float,values_bool);
            nBranches = inames;
            gotit = true;
        }
        else{ // this is value
            if ((std::string)names[0]=="#") // commented
                continue;
            if ( inames != nBranches){
                fprintf(stderr,"There are %d values in line %d, different from %d branches!\n",inames,iline,nBranches);
                continue;
            }
            SetValues(inames,names,d_tree,values_double,values_int,values_char,values_float,values_bool);
        }
    }

    if (d_tree) d_tree->Write();
    file_output.Close();
    return 0;
}

int bin_to_root(const char* input_file, const char* output_file){
    fprintf(stderr,"This is bin_to_root!\n");
    FILE* fpi = 0;
    fpi = fopen(input_file,"r");
    if ( !fpi ){
        fprintf(stderr,"Cannot open file \"%s\"!!!\n",input_file);
        return -1;
    }
    FILE* fpo = 0;
    fpo = fopen(input_file,"w");
    return 0;
}
int txt_to_bin(const char* input_file, const char* output_file){
    fprintf(stderr,"This is txt_to_bin!\n");
    FILE* fpi = 0;
    fpi = fopen(input_file,"r");
    if ( !fpi ){
        fprintf(stderr,"Cannot open file \"%s\"!!!\n",input_file);
        return -1;
    }
    FILE* fpo = 0;
    fpo = fopen(input_file,"w");
    return 0;
}
int bin_to_txt(const char* input_file, const char* output_file){
    fprintf(stderr,"This is bin_to_txt!\n");
    FILE* fpi = 0;
    fpi = fopen(input_file,"r");
    if ( !fpi ){
        fprintf(stderr,"Cannot open file \"%s\"!!!\n",input_file);
        return -1;
    }
    FILE* fpo = 0;
    fpo = fopen(input_file,"w");
    return 0;
}
/* end of functions */

void print_usage(char* prog_name)
{
    fprintf(stderr,"Usage %s [options] [args] input output\n",prog_name);
    fprintf(stderr,"[options]\n");
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
    fprintf(stderr,"\t\t%s -B -i input.bin -o output.txt\n",prog_name);
}

int main(int argc, char** argv){
    m_workmode = txt2root;
    if (argc==1) {
        print_usage(argv[0]);
        return -1;
    }
    init_args();

    int result;
    while((result=getopt(argc,argv,"htbTB"))!=-1){
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
            case '?':
                printf("Wrong option! optopt=%c, optarg=%s\n", optopt, optarg);
                break;
            case 'h':
            default:
                print_usage(argv[0]);
                return 1;
        }
    }
    if (argc-optind<1){
        printf("You should specify an input file!\n");
        return -1;
    }
    strcpy(m_input_file,argv[optind++]);
    printf("input file: %s\n",m_input_file);
    if (argc-optind>=1)
        strcpy(m_output_file,argv[optind++]);
    printf("output file: %s\n",m_output_file);
    printf("workmode: %d\n",m_workmode);

    switch(m_workmode){
        case txt2root:
            if (!txt_to_root(m_input_file,m_output_file)){
                fprintf(stderr,"Successfully translated \"%s\" from text type to root file \"%s\"!\n",m_input_file,m_output_file);
            }
            else{
                fprintf(stderr,"Error when translating \"%s\" from text type to root file \"%s\"!\n",m_input_file,m_output_file);
                return -1;
            }
            break;
        case bin2root:
            if (!bin_to_root(m_input_file,m_output_file)){
                fprintf(stderr,"Successfully translated \"%s\" from binary type to root file \"%s\"!\n",m_input_file,m_output_file);
            }
            else{
                fprintf(stderr,"Error when translating \"%s\" from binary type to root file \"%s\"!\n",m_input_file,m_output_file);
                return -1;
            }
            break;
        case txt2bin:
            if (!txt_to_bin(m_input_file,m_output_file)){
                fprintf(stderr,"Successfully translated \"%s\" from text type to binary file \"%s\"!\n",m_input_file,m_output_file);
            }
            else{
                fprintf(stderr,"Error when translating \"%s\" from text type to binary file \"%s\"!\n",m_input_file,m_output_file);
                return -1;
            }
            break;
        case bin2txt:
            if (!bin_to_txt(m_input_file,m_output_file)){
                fprintf(stderr,"Successfully translated \"%s\" from binary type to text file \"%s\"!\n",m_input_file,m_output_file);
            }
            else{
                fprintf(stderr,"Error when translating \"%s\" from binary type to text file \"%s\"!\n",m_input_file,m_output_file);
                return -1;
            }
            break;
        default:
            printf("Cannot recognize work mode \"%i\"",m_workmode);
            return 1;
    }

    return 0;
}
