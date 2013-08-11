#ifndef MyRootInterface_h
#define MyRootInterface_h 1
#include <string>
#include <vector>

#include "TChain.h"
#include "TObject.h"

class TTree;
class TStyle;
class TCanvas;
class TFile;
class TH1D;
class TH2D;
class TGraph;
class TVector3;
class TGenPhaseSpace;
class TBranch;

class MyRootInterface{

	public:

		MyRootInterface(int verbose = 0,bool backup = false);
		virtual ~MyRootInterface();

	public:

		int read(std::string file);
		int init();
		int init_hist();
		int init_file();
		int GetEntry(Long64_t iEvent);
		int dump();
		int Fill(std::string name, double val){
			int i = get_TH1D_index(name);
			if (i!=-1){
				vecH1D[i]->Fill(val);
			}
			return 0;
		}
		int Fill(std::string name, double val1, double val2){
			int i = get_TH2D_index(name);
			if (i!=-1){
				vecH2D[i]->Fill(val1,val2);
			}
			return 0;
		}
		int Fill(){d_tree->Fill();}
		int Write(){d_tree->Write("",TObject::kOverwrite);}

		int get_TH2D_index(std::string name);
		int get_TH1D_index(std::string name);
		int get_TGraph_index(std::string name);
		int get_TBranch_index(std::string name);
		int get_oTBranch_index(std::string name);

		int add_TH1D(std::string name,
					 std::string title,
					 std::string xName,
					 std::string yName,
					 int bin1,
					 double left1,
					 double right1,
					 double minx = 0,
					 double miny = 0,
					 int color = 1,
					 int compare = 0,
					 int xlog = 0,
					 int ylog = 0,
					 int marker = 3,
					 double norm = 0,
		             std::string drawOpt = "",
					 int useLegend = 0,
					 std::string legendName = "",
					 double legendx1 = 0,
					 double legendy1 = 0,
					 double legendx2 = 0,
					 double legendy2 = 0,
					 int npadx = 1,
					 int npady = 1,
					 double sep = 0
		             ){
			nameForH1D.push_back(name);
			titleForH1D.push_back(title);
			xNameForH1D.push_back(xName);
			yNameForH1D.push_back(yName);
			bin1ForH1D.push_back(bin1);
			left1ForH1D.push_back(left1);
			right1ForH1D.push_back(right1);
			minxForH1D.push_back(minx);
			minyForH1D.push_back(miny);
			colorForH1D.push_back(color);
			compareForH1D.push_back(compare);
			xlogForH1D.push_back(xlog);
			ylogForH1D.push_back(ylog);
			markerForH1D.push_back(marker);
			normForH1D.push_back(norm);
			drawOptForH1D.push_back(drawOpt);
			useLegendForH1D.push_back(useLegend);
			legendNameForH1D.push_back(legendName);
			legendx1ForH1D.push_back(legendx1);
			legendy1ForH1D.push_back(legendy1);
			legendx2ForH1D.push_back(legendx2);
			legendy2ForH1D.push_back(legendy2);
			npadxForH1D.push_back(npadx);
			npadyForH1D.push_back(npady);
			sepForH1D.push_back(sep);
			int i = nameForH1D.size()-1;
			return i;
		}

		//*****************************************************************************
		// to get
		int get_TH2D_size(){return nameForH2D.size();}
		int get_TH1D_size(){return nameForH1D.size();}
		int get_TGraph_size(){return nameForGraph.size();}
		int get_TBranch_size(){return vec_TBranchName.size();}
		int get_oTBranch_size(){return vec_oTBranchName.size();}
		Long64_t get_Entries(){return (m_TChain?m_TChain->GetEntries():0);}

		std::string get_nameForH2D(int i){if (i>=nameForH2D.size()) return NULL; return nameForH2D[i];}
		std::string get_titleForH2D(int i){if (i>=titleForH2D.size()) return NULL; return titleForH2D[i];}
		std::string get_xNameForH2D(int i){if (i>=xNameForH2D.size()) return NULL; return xNameForH2D[i];}
		std::string get_yNameForH2D(int i){if (i>=yNameForH2D.size()) return NULL; return yNameForH2D[i];}
		int get_bin1ForH2D(int i){if (i>=bin1ForH2D.size()) return NULL; return bin1ForH2D[i];}
		double get_left1ForH2D(int i){if (i>=left1ForH2D.size()) return NULL; return left1ForH2D[i];}
		double get_right1ForH2D(int i){if (i>=right1ForH2D.size()) return NULL; return right1ForH2D[i];}
		int get_bin2ForH2D(int i){if (i>=bin2ForH2D.size()) return NULL; return bin2ForH2D[i];}
		double get_left2ForH2D(int i){if (i>=left2ForH2D.size()) return NULL; return left2ForH2D[i];}
		double get_right2ForH2D(int i){if (i>=right2ForH2D.size()) return NULL; return right2ForH2D[i];}
		TH2D* get_TH2D(int i){if (i>=vecH2D.size()) return NULL; return vecH2D[i];}

		std::string get_nameForH1D(int i){if (i>=nameForH1D.size()) return NULL; return nameForH1D[i];}
		std::string get_titleForH1D(int i){if (i>=titleForH1D.size()) return NULL; return titleForH1D[i];}
		int get_compareForH1D(int i){if (i>=compareForH1D.size()) return NULL; return compareForH1D[i];}
		double get_minxForH1D(int i){if (i>=minxForH1D.size()) return NULL; return minxForH1D[i];}
		double get_minyForH1D(int i){if (i>=minyForH1D.size()) return NULL; return minyForH1D[i];}
		int get_xlogForH1D(int i){if (i>=xlogForH1D.size()) return NULL; return xlogForH1D[i];}
		int get_ylogForH1D(int i){if (i>=ylogForH1D.size()) return NULL; return ylogForH1D[i];}
		int get_colorForH1D(int i){if (i>=colorForH1D.size()) return NULL; return colorForH1D[i];}
		int get_markerForH1D(int i){if (i>=markerForH1D.size()) return NULL; return markerForH1D[i];}
		double get_normForH1D(int i){if (i>=normForH1D.size()) return NULL; return normForH1D[i];}
		std::string get_drawOptForH1D(int i){if (i>=drawOptForH1D.size()) return NULL; return drawOptForH1D[i];}
		std::string get_xNameForH1D(int i){if (i>=xNameForH1D.size()) return NULL; return xNameForH1D[i];}
		std::string get_yNameForH1D(int i){if (i>=yNameForH1D.size()) return NULL; return yNameForH1D[i];}
		int get_bin1ForH1D(int i){if (i>=bin1ForH1D.size()) return NULL; return bin1ForH1D[i];}
		double get_left1ForH1D(int i){if (i>=left1ForH1D.size()) return NULL; return left1ForH1D[i];}
		double get_right1ForH1D(int i){if (i>=right1ForH1D.size()) return NULL; return right1ForH1D[i];}
		int get_useLegendForH1D(int i){if (i>=useLegendForH1D.size()) return NULL; return useLegendForH1D[i];}
		std::string get_legendNameForH1D(int i){if (i>=legendNameForH1D.size()) return NULL; return legendNameForH1D[i];}
		double get_legendx1ForH1D(int i){if (i>=legendx1ForH1D.size()) return NULL; return legendx1ForH1D[i];}
		double get_legendy1ForH1D(int i){if (i>=legendy1ForH1D.size()) return NULL; return legendy1ForH1D[i];}
		double get_legendx2ForH1D(int i){if (i>=legendx2ForH1D.size()) return NULL; return legendx2ForH1D[i];}
		double get_legendy2ForH1D(int i){if (i>=legendy2ForH1D.size()) return NULL; return legendy2ForH1D[i];}
		int get_npadxForH1D(int i){if (i>=npadxForH1D.size()) return NULL; return npadxForH1D[i];}
		int get_npadyForH1D(int i){if (i>=npadyForH1D.size()) return NULL; return npadyForH1D[i];}
		double get_sepForH1D(int i){if (i>=sepForH1D.size()) return NULL; return sepForH1D[i];}
		TH1D* get_TH1D(int i){if (i>=vecH1D.size()) return NULL; return vecH1D[i];}

		std::string get_nameForGraph(int i){if (i>=nameForGraph.size()) return NULL; return nameForGraph[i];}
		std::string get_titleForGraph(int i){if (i>=titleForGraph.size()) return NULL; return titleForGraph[i];}
		int get_compareForGraph(int i){if (i>=compareForGraph.size()) return NULL; return compareForGraph[i];}
		double get_minxForGraph(int i){if (i>=minxForGraph.size()) return NULL; return minxForGraph[i];}
		double get_maxxForGraph(int i){if (i>=maxxForGraph.size()) return NULL; return maxxForGraph[i];}
		double get_minyForGraph(int i){if (i>=minyForGraph.size()) return NULL; return minyForGraph[i];}
		int get_xlogForGraph(int i){if (i>=xlogForGraph.size()) return NULL; return xlogForGraph[i];}
		int get_ylogForGraph(int i){if (i>=ylogForGraph.size()) return NULL; return ylogForGraph[i];}
		int get_colorForGraph(int i){if (i>=colorForGraph.size()) return NULL; return colorForGraph[i];}
		int get_markerForGraph(int i){if (i>=markerForGraph.size()) return NULL; return markerForGraph[i];}
		std::string get_drawOptForGraph(int i){if (i>=drawOptForGraph.size()) return NULL; return drawOptForGraph[i];}
		std::string get_xNameForGraph(int i){if (i>=xNameForGraph.size()) return NULL; return xNameForGraph[i];}
		std::string get_yNameForGraph(int i){if (i>=yNameForGraph.size()) return NULL; return yNameForGraph[i];}
		std::vector<double>  get_xForGraph(int i){std::vector<double> null; if (i>=xForGraph.size()) return null; return xForGraph[i];}
		std::vector<double>  get_yForGraph(int i){std::vector<double> null; if (i>=yForGraph.size()) return null; return yForGraph[i];}

		double get_vec_double(int i){if (i>=vec_double.size()) return NULL; return vec_double[i];}
		int get_vec_int(int i){if (i>=vec_int.size()) return NULL; return vec_int[i];}
		std::string get_vec_string(int i){if (i>=vec_string.size()) return NULL; return vec_string[i];}
		std::vector<double>*  get_vec_vecdouble(int i){if (i>=vec_vecdouble.size()) return NULL; return vec_vecdouble[i];}
		std::vector<int>*  get_vec_vecint(int i){if (i>=vec_vecint.size()) return NULL; return vec_vecint[i];}
		std::vector<std::string>*  get_vec_vecstring(int i){if (i>=vec_vecstring.size()) return NULL; return vec_vecstring[i];}

		int get_value(std::string name, double &val, double scale = 1);
		int get_value(std::string name, int &val, double scale = 1);
		int get_value(std::string name, std::string &val);
		int get_value(std::string name, std::vector<double> &val, double scale = 1);
		int get_value(std::string name, std::vector<int> &val, double scale = 1);
		int get_value(std::string name, std::vector<std::string> &val);

		double get_ovec_double(int i){if (i>=ovec_double.size()) return NULL; return ovec_double[i];}
		int get_ovec_int(int i){if (i>=ovec_int.size()) return NULL; return ovec_int[i];}
		std::string get_ovec_string(int i){if (i>=ovec_string.size()) return NULL; return ovec_string[i];}

		std::vector<double>  get_ovec_vecdouble(int i){if (i>=ovec_vecdouble.size()){std::vector<double> dum; return dum;} return ovec_vecdouble[i];}
		std::vector<int>  get_ovec_vecint(int i){if (i>=ovec_vecint.size()){std::vector<int> dum; return dum;} return ovec_vecint[i];}
		std::vector<std::string>  get_ovec_vecstring(int i){if (i>=ovec_vecstring.size()) {std::vector<std::string> dum; return dum;} return ovec_vecstring[i];}

		//*****************************************************************************
		// to set
		int set_beginCPU(int i, int val){
			if (i >= DirNames.size()) return -1;
			beginCPU[i]= val;
			if (m_verbose >= Verbose_GeneralInfo) std::cout<<prefix_GeneralInfo<<"Changing beginCPU["<<i<<"] to \""<<val<<"\""<<std::endl;
			return 0;
		}
		int set_NCPU(int i, int val){
			if (i >= DirNames.size()) return -1;
			NCPU[i] = val;
			if (m_verbose >= Verbose_GeneralInfo) std::cout<<prefix_GeneralInfo<<"Changing NCPU["<<i<<"] to \""<<val<<"\""<<std::endl;
			return 0;
		}
		int add_oFileName(std::string name){oFileName.push_back(name);}
		int set_OutputDir(std::string dir){
			OutputDir = dir;
			if (m_verbose >= Verbose_GeneralInfo) std::cout<<prefix_GeneralInfo<<"Changing OutputDir to \""<<OutputDir<<"\""<<std::endl;
		}
		int set_OutputName(std::string name){
			OutputName = name;
			if (m_verbose >= Verbose_GeneralInfo) std::cout<<prefix_GeneralInfo<<"Changing OutputName to \""<<OutputName<<"\""<<std::endl;
		}
		int set_backup(bool backup){m_backup=backup;}
		int set_TreeName(std::string name){
			TreeName = name;
			if (m_verbose >= Verbose_GeneralInfo) std::cout<<prefix_GeneralInfo<<"Changing TreeName to \""<<TreeName<<"\""<<std::endl;
		}
		int set_oTreeName(std::string name){
			oTreeName = name;
			if (m_verbose >= Verbose_GeneralInfo) std::cout<<prefix_GeneralInfo<<"Changing oTreeName to \""<<oTreeName<<"\""<<std::endl;
		}

		int set_nameForH2D(int i, std::string val){if (i>=nameForH2D.size()) return -1; nameForH2D[i] = val; return 0;}
		int set_titleForH2D(int i, std::string val){if (i>=titleForH2D.size()) return -1; titleForH2D[i] = val; return 0;}
		int set_xNameForH2D(int i, std::string val){if (i>=xNameForH2D.size()) return -1; xNameForH2D[i] = val; return 0;}
		int set_yNameForH2D(int i, std::string val){if (i>=yNameForH2D.size()) return -1; yNameForH2D[i] = val; return 0;}
		int set_bin1ForH2D(int i, int val){if (i>=bin1ForH2D.size()) return -1; bin1ForH2D[i] = val; return 0;}
		int set_left1ForH2D(int i, double val){if (i>=left1ForH2D.size()) return -1; left1ForH2D[i] = val; return 0;}
		int set_right1ForH2D(int i, double val){if (i>=right1ForH2D.size()) return -1; right1ForH2D[i] = val; return 0;}
		int set_bin2ForH2D(int i, int val){if (i>=bin2ForH2D.size()) return -1; bin2ForH2D[i] = val; return 0;}
		int set_left2ForH2D(int i, double val){if (i>=left2ForH2D.size()) return -1; left2ForH2D[i] = val; return 0;}
		int set_right2ForH2D(int i, double val){if (i>=right2ForH2D.size()) return -1; right2ForH2D[i] = val; return 0;}

		int set_nameForH1D(int i, std::string val){if (i>=nameForH1D.size()) return -1; nameForH1D[i] = val; return 0;}
		int set_titleForH1D(int i, std::string val){if (i>=titleForH1D.size()) return -1; titleForH1D[i] = val; return 0;}
		int set_xNameForH1D(int i, std::string val){if (i>=xNameForH1D.size()) return -1; xNameForH1D[i] = val; return 0;}
		int set_yNameForH1D(int i, std::string val){if (i>=yNameForH1D.size()) return -1; yNameForH1D[i] = val; return 0;}
		int set_bin1ForH1D(int i, int val){if (i>=bin1ForH1D.size()) return -1; bin1ForH1D[i] = val; return 0;}
		int set_left1ForH1D(int i, double val){if (i>=left1ForH1D.size()) return -1; left1ForH1D[i] = val; return 0;}
		int set_right1ForH1D(int i, double val){if (i>=right1ForH1D.size()) return -1; right1ForH1D[i] = val; return 0;}
		int set_minxForH1D(int i, double val){if (i>=minxForH1D.size()) return -1; minxForH1D[i] = val; return 0;}
		int set_minyForH1D(int i, double val){if (i>=minyForH1D.size()) return -1; minyForH1D[i] = val; return 0;}
		int set_colorForH1D(int i, int val){if (i>=colorForH1D.size()) return -1; colorForH1D[i] = val; return 0;}
		int set_compareForH1D(int i, int val){if (i>=compareForH1D.size()) return -1; compareForH1D[i] = val; return 0;}
		int set_xlogForH1D(int i, int val){if (i>=xlogForH1D.size()) return -1; xlogForH1D[i] = val; return 0;}
		int set_ylogForH1D(int i, int val){if (i>=ylogForH1D.size()) return -1; ylogForH1D[i] = val; return 0;}
		int set_markerForH1D(int i, int val){if (i>=markerForH1D.size()) return -1; markerForH1D[i] = val; return 0;}
		int set_normForH1D(int i, double val){if (i>=normForH1D.size()) return -1; normForH1D[i] = val; return 0;}
		int set_drawOptForH1D(int i, std::string val){if (i>=drawOptForH1D.size()) return -1; drawOptForH1D[i] = val; return 0;}
		int set_useLegendForH1D(int i, int val){if (i>=useLegendForH1D.size()) return -1; useLegendForH1D[i] = val; return 0;}
		int set_legendNameForH1D(int i, std::string val){if (i>=legendNameForH1D.size()) return -1; legendNameForH1D[i] = val; return 0;}
		int set_legendx1ForH1D(int i, double val){if (i>=legendx1ForH1D.size()) return -1; legendx1ForH1D[i] = val; return 0;}
		int set_legendy1ForH1D(int i, double val){if (i>=legendy1ForH1D.size()) return -1; legendy1ForH1D[i] = val; return 0;}
		int set_legendx2ForH1D(int i, double val){if (i>=legendx2ForH1D.size()) return -1; legendx2ForH1D[i] = val; return 0;}
		int set_legendy2ForH1D(int i, double val){if (i>=legendy2ForH1D.size()) return -1; legendy2ForH1D[i] = val; return 0;}
		int set_npadxForH1D(int i, int val){if (i>=npadxForH1D.size()) return -1; npadxForH1D[i] = val; return 0;}
		int set_npadyForH1D(int i, int val){if (i>=npadyForH1D.size()) return -1; npadyForH1D[i] = val; return 0;}
		int set_sepForH1D(int i, double val){if (i>=sepForH1D.size()) return -1; sepForH1D[i] = val; return 0;}

		int set_nameForGraph(int i, std::string val){if (i>=nameForGraph.size()) return -1; nameForGraph[i] = val; return 0;}
		int set_titleForGraph(int i, std::string val){if (i>=titleForGraph.size()) return -1; titleForGraph[i] = val; return 0;}
		int set_compareForGraph(int i, int val){if (i>=compareForGraph.size()) return -1; compareForGraph[i] = val; return 0;}
		int set_minxForGraph(int i, double val){if (i>=minxForGraph.size()) return -1; minxForGraph[i] = val; return 0;}
		int set_maxxForGraph(int i, double val){if (i>=maxxForGraph.size()) return -1; maxxForGraph[i] = val; return 0;}
		int set_minyForGraph(int i, double val){if (i>=minyForGraph.size()) return -1; minyForGraph[i] = val; return 0;}
		int set_xlogForGraph(int i, int val){if (i>=xlogForGraph.size()) return -1; xlogForGraph[i] = val; return 0;}
		int set_ylogForGraph(int i, int val){if (i>=ylogForGraph.size()) return -1; ylogForGraph[i] = val; return 0;}
		int set_colorForGraph(int i, int val){if (i>=colorForGraph.size()) return -1; colorForGraph[i] = val; return 0;}
		int set_markerForGraph(int i, int val){if (i>=markerForGraph.size()) return -1; markerForGraph[i] = val; return 0;}
		int set_drawOptForGraph(int i, std::string val){if (i>=drawOptForGraph.size()) return -1; drawOptForGraph[i] = val; return 0;}
		int set_xNameForGraph(int i, std::string val){if (i>=xNameForGraph.size()) return -1; xNameForGraph[i] = val; return 0;}
		int set_yNameForGraph(int i, std::string val){if (i>=yNameForGraph.size()) return -1; yNameForGraph[i] = val; return 0;}
		int set_xForGraph(int i, std::vector<double>  val){if (i>=xForGraph.size()) return -1; xForGraph[i] = val; return 0;}
		int set_yForGraph(int i, std::vector<double>  val){if (i>=yForGraph.size()) return -1; yForGraph[i] = val; return 0;}

		int set_ovec_double(int i, double val){if (i>=ovec_double.size()) return -1; ovec_double[i] = val; return 0;}
		int set_ovec_int(int i, int val){if (i>=ovec_int.size()) return -1; ovec_int[i] = val; return 0;}
		int set_ovec_string(int i, std::string val){if (i>=ovec_string.size()) return -1; ovec_string[i] = val; return 0;}

		int set_ovec_vecdouble(int i, std::vector<double> val){if (i>=ovec_vecdouble.size()) return -1; ovec_vecdouble[i] = val; return 0;}
		int set_ovec_vecint(int i, std::vector<int> val){if (i>=ovec_vecint.size()) return -1; ovec_vecint[i] = val; return 0;}
		int set_ovec_vecstring(int i, std::vector<std::string> val){if (i>=ovec_vecstring.size()) return -1; ovec_vecstring[i] = val; return 0;}

		int set_ovalue(std::string name, double val);
		int set_ovalue(std::string name, int val);
		int set_ovalue(std::string name, std::string val);
		int set_ovalue(std::string name, std::vector<double> val);
		int set_ovalue(std::string name, std::vector<int> val);
		int set_ovalue(std::string name, std::vector<std::string> val);

		// functions
		bool ISEMPTY(std::string content);
		void seperate_string(std::string line, std::vector<std::string> &strs, const char sep );
		double string2double(std::string str);

	private:

		// verbose control
		int m_verbose;
		bool m_backup;
		int Verbose_GeneralInfo; //有哪些hist,什么时候输出了，参数如何
		std::string prefix_GeneralInfo;
		int Verbose_HistInfo; //有哪些hist,什么时候输出了，参数如何
		std::string prefix_HistInfo;
		int Verbose_FileInfo; //有哪些FileList,都有多少file
		std::string prefix_FileInfo;
		int Verbose_InputInfo; //有哪些Input
		std::string prefix_InputInfo;
		int Verbose_InitInfo; //有哪些Init
		std::string prefix_InitInfo;
		int Verbose_BranchInfo; //有哪些Branch
		std::string prefix_BranchInfo;

		std::string OutputDir;
		std::string OutputName;

		std::vector<std::string> oFileName;

		std::vector<std::string> refFileName;
		std::vector<std::string> refHistName;

		std::vector<std::string> nameForH2D;
		std::vector<std::string> titleForH2D;
		std::vector<std::string> xNameForH2D;
		std::vector<std::string> yNameForH2D;
		std::vector<int>     bin1ForH2D;
		std::vector<double>  left1ForH2D;
		std::vector<double>  right1ForH2D;
		std::vector<int>     bin2ForH2D;
		std::vector<double>  left2ForH2D;
		std::vector<double>  right2ForH2D;
		std::vector<TH2D*>   vecH2D;

		std::vector<std::string> nameForH1D;
		std::vector<std::string> titleForH1D;
		std::vector<std::string> xNameForH1D;
		std::vector<std::string> yNameForH1D;
		std::vector<int>     bin1ForH1D;
		std::vector<double>  left1ForH1D;
		std::vector<double>  right1ForH1D;
		std::vector<double> minxForH1D;
		std::vector<double> minyForH1D;
		std::vector<int> colorForH1D;
		std::vector<int> compareForH1D;
		std::vector<int> xlogForH1D;
		std::vector<int> ylogForH1D;
		std::vector<int> markerForH1D;
		std::vector<double> normForH1D;
		std::vector<std::string> drawOptForH1D;
		std::vector<int> useLegendForH1D;
		std::vector<std::string> legendNameForH1D;
		std::vector<double> legendx1ForH1D;
		std::vector<double> legendy1ForH1D;
		std::vector<double> legendx2ForH1D;
		std::vector<double> legendy2ForH1D;
		std::vector<int> npadxForH1D;
		std::vector<int> npadyForH1D;
		std::vector<double> sepForH1D;
		std::vector<TH1D*>   vecH1D;

		std::vector<std::string> nameForGraph;
		std::vector<std::string> titleForGraph;
		std::vector<int> compareForGraph;
		std::vector<double> minxForGraph;
		std::vector<double> maxxForGraph;
		std::vector<double> minyForGraph;
		std::vector<int> xlogForGraph;
		std::vector<int> ylogForGraph;
		std::vector<int> colorForGraph;
		std::vector<int> markerForGraph;
		std::vector<std::string> drawOptForGraph;
		std::vector<std::string> xNameForGraph;
		std::vector<std::string> yNameForGraph;
		std::vector<std::vector<double> > xForGraph;
		std::vector<std::vector<double> > yForGraph;

		// for files
		std::vector<std::string> DirNames; 
		std::vector<std::string> RunNames; 
		std::vector<int> NCPU;
		std::vector<int> beginCPU;
		std::vector<int> NJob;

		// for output
		TFile *m_file;
		TTree *d_tree;
		std::vector<double> ovec_double;
		std::vector<int> ovec_int;
		std::vector<std::string> ovec_string;
		std::vector<std::vector<double> > ovec_vecdouble;
		std::vector<std::vector<int> > ovec_vecint;
		std::vector<std::vector<std::string> > ovec_vecstring;

		// for input
		TChain *m_TChain;
		std::string TreeName;
		std::vector<TBranch*> vec_TBranch;
		std::vector<std::string> vec_TBranchName;
		std::vector<int> vec_TBranchType;
		std::vector<int> vec_TBranchIsVec;
		std::string oTreeName;
		std::vector<TBranch*> vec_oTBranch;
		std::vector<std::string> vec_oTBranchName;
		std::vector<int> vec_oTBranchType;
		std::vector<int> vec_oTBranchIsVec;
		std::vector<double> vec_double;
		std::vector<int> vec_int;
		std::vector<std::string> vec_string;
		std::vector<std::vector<double>* > vec_vecdouble;
		std::vector<std::vector<int>* > vec_vecint;
		std::vector<std::vector<std::string>* > vec_vecstring;

};

#endif
