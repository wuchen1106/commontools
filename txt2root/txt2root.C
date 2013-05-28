#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //getopt

/* arguments */
int arg_seed;
char arg_genfitGeom_name[128];
char arg_output_root[128];
int arg_total;
char arg_event_type[128];
char arg_hit_type[128];
char arg_track_type[128];
char arg_extrap_pos[128];
char arg_input_root[128];
FILE* arg_writeout_track_fp;
double arg_posini_x;
double arg_posini_y;
double arg_posini_z;
double arg_momini_x;
double arg_momini_y;
double arg_momini_z;
/* end of arguments */

void init_args()
{
	arg_seed = -1;
	arg_total = 1; // one event
	arg_writeout_track_fp = NULL;
	strcpy(arg_genfitGeom_name, "empty");
	strcpy(arg_output_root, "genfit.root");
	strcpy(arg_input_root, "empty");
	strcpy(arg_event_type,"SIG"); // SIG,DIO,Proton,TXT
	strcpy(arg_hit_type,"GeaneTrackRep2"); // RKTrackRep,GeaneTrackRep2,ROOT
	strcpy(arg_track_type,"GeaneTrackRep2"); // RKTrackRep,GeaneTrackRep2
	strcpy(arg_extrap_pos,"FirstHit"); //FirtHit,BeforeInwall
}

void print_usage(char* prog_name)
{
	fprintf(stderr,"Usage %s [inputs] [outputs]\n",prog_name);
	fprintf(stderr,"[inputs]\n");
	fprintf(stderr,"\t -s <seed>\n");
	fprintf(stderr,"\t\t Note that seed=0 is an option to set random seed\n");
	fprintf(stderr,"\t -n <number_of_events>\n");
	fprintf(stderr,"\t -c <config.txt>\n");
	fprintf(stderr,"\t\t Check sample config.txt for the format\n");
	fprintf(stderr,"\t -e <event_type>\n");
	fprintf(stderr,"\t\t SIG(104.973MeV/c), DIO(watanabe_shanker, p > <dio_startpoint> (MeV/c), Proton(Silicon spectrum,thre=50MeV/c), TXT(-f <input_par.txt>)\n");
	fprintf(stderr,"\t -m <dio_startpoint>\n");
	fprintf(stderr,"\t\t Unit is MeV/c, 0.101 etc.\n");
	fprintf(stderr,"\t -j <hit_type>\n");
	fprintf(stderr,"\t\t RKTrackRep, GeaneTrackRep2, ROOT(-i <input.root>)\n");
	fprintf(stderr,"\t -t <track_type>\n");
	fprintf(stderr,"\t\t RKTrackRep, GeaneTrackRep2\n");
	fprintf(stderr,"\t -f <ini_param.txt>\n");
	fprintf(stderr,"\t\t format: x y z px py pz\n");
	fprintf(stderr,"\t\t x y z   : initial position in cm\n");
	fprintf(stderr,"\t\t px py pz: initial momentum in GeV/c\n");
	fprintf(stderr,"[outputs]\n");
	fprintf(stderr,"\t -o <output.root>\n");
	fprintf(stderr,"\t\t Check source code for the description on braches\n");
	fprintf(stderr,"\t -g <TGeometryFile.root>\n");
	fprintf(stderr,"\t\t TGeometryFile.root can be viewed by ROOT as follows.\n");
	fprintf(stderr,"\t\t $ root TGeometryFile.root\n");
	fprintf(stderr,"\t\t root[0] genfitGeom->GetTopVolume()->Draw(\"ogl\")\n");
	fprintf(stderr,"\t -z <hit_pos.txt>\n");
	fprintf(stderr,"\t\t format: x y z px py pz hit\n");
	fprintf(stderr,"\t\t x y z   : track position in cm\n");
	fprintf(stderr,"\t\t px py pz: track momentum in GeV/c\n");
	fprintf(stderr,"\t\t hit     : hit flag (1=hit at detector, 0=no hit at detector)\n");
	fprintf(stderr,"\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t(1) Run 1k signal events with config8.txt and records output to root/output.root\n");
	fprintf(stderr,"\t\t%s -s 1 -n 1000 -c user_config/config8.txt -e SIG -j GeaneTrackRep2 -t GeaneTrackRep2 -o root/output.root\n",prog_name);
	fprintf(stderr,"\t(2) Record TGeo root file for config8.txt\n");
	fprintf(stderr,"\t\t%s -s 1 -n 1 -c user_config/config8.txt -g tmp/tgeo.root\n",prog_name);
	fprintf(stderr,"\t(3) Record hit positions in tmp/hit_pos.txt run with the initial parameters in tmp/ini_param.txt\n");
	fprintf(stderr,"\t\t%s -s 1 -n 1 -c user_config/config8.txt -f tmp/ini_param.txt -z tmp/hit_pos.txt\n",prog_name);
}

int main(int argc, char** argv){
	if (argc==1) {
		print_usage(argv[0]);
		return -1;
	}
	init_args();

	int result;
	while((result=getopt(argc,argv,"s:i:n:e:j:t:p:f:c:o:g:z:m:h"))!=-1){
		switch(result){
			/* INPUTS */
			case 's':
				arg_seed = atoi(optarg);
				printf("gRandom->SetSeed(%d)\n",arg_seed);
				break;
			case 'i':
				strcpy(arg_input_root,optarg);
				printf("input_root %s\n",arg_input_root);
				break;
			case 'n':
				arg_total = atoi(optarg);
				printf("total %d\n",arg_total);
				break;
			case 'e':
				strcpy(arg_event_type,optarg);
				printf("event_type %s\n",arg_event_type);
				break;
			case 'j':
				strcpy(arg_hit_type,optarg);
				printf("hit_type %s\n",arg_hit_type);
				break;
			case 't':
				strcpy(arg_track_type,optarg);
				printf("track_type %s\n",arg_track_type);
				break;
			case 'p':
				strcpy(arg_extrap_pos,optarg);
				printf("extrap_pos %s\n",arg_extrap_pos);
				break;
			case 'f':
				break;
			case 'c':
				break;
			case 'm':
				break;

				/* OUTPUTS */
			case 'o':
				strcpy(arg_output_root,optarg);
				printf("output_root %s\n",arg_output_root);
				break;
			case 'g':
				strcpy(arg_genfitGeom_name,optarg);
				printf("genfitGeom_name %s\n",arg_genfitGeom_name);
				break;
			case 'z':
				arg_writeout_track_fp = fopen(optarg,"w");
				printf("writeout_track: fname %s\n",optarg);
				break;
			case 'h':
			default:
				print_usage(argv[0]);
				exit(1);
		}
	}

	return 0;
}
