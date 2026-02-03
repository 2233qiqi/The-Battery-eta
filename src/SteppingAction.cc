#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4StepPoint* prePoint  = step->GetPreStepPoint();
    G4StepPoint* postPoint = step->GetPostStepPoint();
    if (!postPoint) return;

    G4VPhysicalVolume* prePV = prePoint->GetTouchableHandle()->GetVolume();
    G4VPhysicalVolume* postPV = postPoint->GetTouchableHandle()->GetVolume();
    
    if (!prePV || !postPV) return;

    G4LogicalVolume* postLV = postPV->GetLogicalVolume();
    G4LogicalVolume* preLV  = prePV->GetLogicalVolume();

    if (postLV->GetName() == "LogicalSic")
    {
        G4double edep = step->GetTotalEnergyDeposit();
        
        if (edep > 0.)
        {

            fEventAction->AddEdep(edep);


            G4double z_depth = prePoint->GetPosition().z();
            
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

            analysisManager->FillH1(0, z_depth, edep);
        }
    }
    
    if ( !fEventAction->HasEnteredSiC() &&
         preLV->GetName()  != "LogicalSic" &&
         postLV->GetName() == "LogicalSic" )
    {
        fEventAction->SetEnteredSiC();
    }
}