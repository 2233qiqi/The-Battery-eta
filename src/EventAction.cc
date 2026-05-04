#include "EventAction.hh"
#include "RunAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

EventAction::EventAction()
: G4UserEventAction(),
  fEnteredSiC(false),
  fTotalEdep(0.),
  fTotalEdepNi(0.)
{}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fEnteredSiC = false;  
    fTotalEdep = 0.;
    fTotalEdepNi = 0.;
}

void EventAction::SetEnteredSiC()
{
    fEnteredSiC = true;
}

G4bool EventAction::HasEnteredSiC() const
{
    return fEnteredSiC;
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    G4int eventID = event->GetEventID();

    // Fill Ntuple columns
    analysisManager->FillNtupleIColumn(0, eventID);           // EventID
    analysisManager->FillNtupleIColumn(1, (fTotalEdepNi > 0.) ? 1 : 0);  // ParticleCrossedNi
    analysisManager->FillNtupleIColumn(2, fEnteredSiC ? 1 : 0);          // EnteredSiC
    analysisManager->FillNtupleIColumn(3, 0);                             // CarriersPair (placeholder)
    analysisManager->FillNtupleIColumn(4, 0);                             // EhPairsFromNi (placeholder)
    analysisManager->FillNtupleIColumn(5, 0);                             // EhPairsFromSiC (placeholder)
    analysisManager->FillNtupleDColumn(6, fTotalEdepNi / MeV);           // EnergyNi (MeV)
    analysisManager->FillNtupleDColumn(7, fTotalEdep / MeV);             // EnergySiC (MeV)
    analysisManager->FillNtupleDColumn(8, 0.);                            // TotalInitialEnergy (filled in RunAction later)
    analysisManager->AddNtupleRow();

    // Count events that entered SiC
    if (fEnteredSiC)
    {
        const G4UserRunAction* userRunAction = G4RunManager::GetRunManager()->GetUserRunAction();
        RunAction* runAction = const_cast<RunAction*>(static_cast<const RunAction*>(userRunAction));
        
        if(runAction)
        {
            runAction->AddEnteredEvent();
        }
    }
}
