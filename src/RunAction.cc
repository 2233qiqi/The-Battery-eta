#include "RunAction.hh"
#include "EventAction.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

RunAction :: RunAction()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager ::  Instance();
   
    analysisManager ->SetFileName("DespositionData");
    analysisManager ->SetDefaultFileType("root");
    analysisManager ->CreateH1(0,"Energy desposition in Sic",0,100*keV,100*MeV);
};

RunAction :: ~RunAction()
{

};

void RunAction :: BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager :: Instance();
    analysisManager ->OpenFile();
}

void RunAction ::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager ::Instance();
    analysisManager ->Write();
    analysisManager ->CloseFile();
}