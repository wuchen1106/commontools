{
  std::string old_file="/home/chen/workarea/Analyse/common_tools/tuple2hist/old.root";
  std::string new_file="/home/chen/workarea/Analyse/common_tools/tuple2hist/new.root";
  TH1F* PvHisto = new TH1F ("PV", "PV Distribution"  ,200, -0.1 , 1.1);

  gROOT->ProcessLine(".L AutoHistoCompare.C+");
  AutoHistoCompare * myPV = new AutoHistoCompare(PvHisto, old_file.c_str(), new_file.c_str(), 1 );
  myPV-> Execute();

  PvHisto.Draw();

  delete myPV;

}
