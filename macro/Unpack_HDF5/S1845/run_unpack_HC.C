#define cRED "\033[1;31m"
#define cYELLOW "\033[1;33m"
#define cNORMAL "\033[0m"
#define cGREEN "\033[1;32m"


void run_unpack_HC(std::string dataFile = "/Users/yassid/Desktop/run_0074.h5",TString parameterFile = "ATTPC.e15250.par",TString mappath="")
{

  // -----   Timer   --------------------------------------------------------
 TStopwatch timer;
 timer.Start();
 // ------------------------------------------------------------------------

  gSystem->Load("libXMLParser.so");
  // -----------------------------------------------------------------
  // Set file names
  TString scriptfile = "LookupProto20181201.xml";
  TString protomapfile = "proto20181201.map";
  TString dir = getenv("VMCWORKDIR");
  TString scriptdir = dir + "/scripts/"+ scriptfile;
  TString dataDir = dir + "/macro/data/";
  TString geomDir = dir + "/geometry/";
  TString protomapdir = dir + "/scripts/"+ protomapfile;
  TString geo = "proto20181201_geo_hires.root";
  gSystem -> Setenv("GEOMPATH", geomDir.Data());

  //TString inputFile   = dataDir + name + ".digi.root";
  //TString outputFile  = dataDir + "output.root";
  TString outputFile  = "output_proto.root";
  //TString mcParFile   = dataDir + name + ".params.root";
  TString loggerFile  = dataDir + "ATTPCLog.log";
  TString digiParFile = dir + "/parameters/" + parameterFile;
  TString geoManFile  = dir + "/geometry/ATTPC_Proto_v1.0.root";


  // -----------------------------------------------------------------
  // Logger
  FairLogger *fLogger = FairLogger::GetLogger();
  /*fLogger -> SetLogFileName(loggerFile);
  fLogger -> SetLogToScreen(kTRUE);
  fLogger -> SetLogToFile(kTRUE);
  fLogger -> SetLogVerbosityLevel("LOW");*/

  FairRunAna* run = new FairRunAna();
  run -> SetOutputFile(outputFile);
  //run -> SetGeomFile("../geometry/ATTPC_Proto_v1.0.root");
  run -> SetGeomFile(geoManFile);

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
  parIo1 -> open(digiParFile.Data(), "in");
  //FairParRootFileIo* parIo2 = new FairParRootFileIo();
  //parIo2 -> open("param.dummy_proto.root");
 // rtdb -> setFirstInput(parIo2);
  rtdb -> setSecondInput(parIo1);

  ATHDFParserTask* HDFParserTask = new ATHDFParserTask(1);
  HDFParserTask->SetPersistence(kTRUE);
  HDFParserTask->SetATTPCMap(scriptdir.Data());
  HDFParserTask->SetProtoGeoFile(geo.Data());
  HDFParserTask->SetProtoMapFile(protomapdir.Data());
  HDFParserTask->SetFileName(dataFile);

  //ATPSATask *psaTask = new ATPSATask();
  //psaTask -> SetPersistence(kTRUE);
  //psaTask -> SetThreshold(1);
  //psaTask -> SetPSAMode(1); //NB: 1 is ATTPC - 2 is pATTPC - 3 Filter for ATTPC - 4: Full Time Buckets
  //psaTask -> SetPeakFinder(); //NB: Use either peak finder of maximum finder but not both at the same time
  //psaTask -> SetMaxFinder();
  //psaTask -> SetBaseCorrection(kTRUE); //Directly apply the base line correction to the pulse amplitude to correct for the mesh induction. If false the correction is just saved
  //psaTask -> SetTimeCorrection(kFALSE); //Interpolation around the maximum of the signal peak

  //ATPRATask *praTask = new ATPRATask();
  //praTask -> SetPersistence(kTRUE);
  
  
  
  run -> AddTask(HDFParserTask);
  //run -> AddTask(psaTask);
  //run -> AddTask(praTask);

  run -> Init();

  run->Run(0,1);
  //run -> RunOnTBData();


  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully."  << std::endl << std::endl;
  std::cout << "- Output file : " << outputFile << std::endl << std::endl;
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------

  gApplication->Terminate();

}
