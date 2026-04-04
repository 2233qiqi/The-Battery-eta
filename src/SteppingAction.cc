#include "SteppingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(EventAction* eventAction, RunAction* runAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fRunAction(runAction)
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

    if (postLV->GetName() == "LogicalNi")
    {
        G4double edepNi = step->GetTotalEnergyDeposit();
        if (edepNi > 0. && fRunAction)
        {
            fRunAction->AddEnergyInNi(edepNi);
            
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillH1(1, edepNi);
        }
    }

    if (postLV->GetName() == "LogicalSic")
    {
        G4double edep = step->GetTotalEnergyDeposit();
        
        if (edep > 0.)
        {
            fEventAction->AddEdep(edep);
            if (fRunAction)
            {
                fRunAction->AddEnergyInSiC(edep);
            }

            G4double postPointZ = postPoint->GetPosition().z();
            
            G4double sicFrontSurfaceZ = 0.25 * um;
            

            G4double depth = postPointZ - sicFrontSurfaceZ;
            
            if (depth >= 0. && depth <= 310.3*um)
            {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, depth, edep);
            }
        }
    }
    

    if ( postLV->GetName() == "LogicalSic" )
    {
        fEventAction->SetEnteredSiC();
    }
}