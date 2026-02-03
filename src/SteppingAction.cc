#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "RunAction.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4AnalysisManager.hh"


SteppingAction::SteppingAction(EventAction* eventAction):G4UserSteppingAction(), fEventAction(eventAction), fScoringVolume(NULL)
{

}

SteppingAction :: ~SteppingAction()
{

}

void SteppingAction::UserSteppingAction(const G4Step* step)
{  
  G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  G4double edep = step->GetTotalEnergyDeposit();
  
    if (volume->GetName() == "LogicalSic") {
        if (edep > 0.) 
        {
            fEventAction->AddEdep(edep);
        }
      
    }

    G4double z = step->GetPreStepPoint()->GetPosition().z();
    G4double z0 =(1.5/ 2)*um;
    G4double z1 = z - z0;

    if (z1 < 0 || z1 > 310.3*um)
    return;

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH1(0, z1, edep);
    analysisManager->FillH1(1,z1);

}

