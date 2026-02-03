#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction()
: G4UserRunAction(),
  fEnterCounts(0)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetFileName("DepositionData");
    analysisManager->SetDefaultFileType("root");

    analysisManager->CreateH1("Edep_vs_depth", "Energy deposition vs depth in SiC", 100, 0.0, 10.3*um);
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile();
    
    fEnterCounts = 0; 
}

void RunAction::AddEnteredEvent()
{
    fEnterCounts++;
}

void RunAction::EndOfRunAction(const G4Run*)
{
    
    G4cout << " Total events entered SiC: " << fEnterCounts << G4endl;
   
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}