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
    analysisManager ->CreateH2("EnergyDeposition","Depth",300,0,300*um,100,0,10*MeV);
};