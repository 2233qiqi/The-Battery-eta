#include "EventAction.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

EventAction::EventAction():G4UserEventAction(),fTotalEdep (0)
{

}

EventAction::~EventAction()
{

}

void EventAction :: BeginOfEventAction (const G4Event *event)
{
    fTotalEdep =0;
    fEnterCounts = false;
}

void EventAction::AddEdep(G4double edep)
{
    fTotalEdep += edep;
    
}

void EventAction::EneterCounts()
{
    fEnterCounts = true;
}

void EventAction :: EndOfEventAction(const G4Event *event)
{
    if (fTotalEdep > 0) 
        {
           auto analysisManager = G4AnalysisManager::Instance();
           analysisManager->FillH1(0, fTotalEdep);
        }
    if (fEnterCounts) 
    {
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillH1(2, 1);  
    }
    
    G4cout << "Total energy deposition in SiC: "
           << fTotalEdep / keV << " keV" << G4endl;
    fTotalEdep = 0.;
     
   
}

