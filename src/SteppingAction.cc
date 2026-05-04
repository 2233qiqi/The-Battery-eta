#include "SteppingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(EventAction* eventAction, RunAction* runAction, DetectorConstruction* det)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fRunAction(runAction),
  fDetector(det)
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

    G4String postLVName = postLV->GetName();

    if (postLVName == "LogicalNi")
    {
        G4double edepNi = step->GetTotalEnergyDeposit();
        if (edepNi > 0.)
        {
            fEventAction->AddEdepNi(edepNi);
            if (fRunAction)
            {
                fRunAction->AddEnergyInNi(edepNi);
            }
        }
    }
    else if (postLVName == "LogicalSic")
    {
        // Mark that this event's particle has entered SiC
        fEventAction->SetEnteredSiC();

        G4double edep = step->GetTotalEnergyDeposit();
        
        if (edep > 0.)
        {
            fEventAction->AddEdep(edep);
            if (fRunAction)
            {
                fRunAction->AddEnergyInSiC(edep);
            }

            // Correct depth calculation:
            // Ni is centered at z=0, occupies [-Ni_Z/2, +Ni_Z/2]
            // SiC front surface is at z = +Ni_Z/2
            G4double postPointZ = postPoint->GetPosition().z();
            G4double sicFrontSurfaceZ = fDetector->GetNiThickness() / 2.0;
            G4double depth = postPointZ - sicFrontSurfaceZ;
            
            if (depth >= 0. && depth <= fDetector->GetSicThickness())
            {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, depth, edep);
            }
        }
    }
}
