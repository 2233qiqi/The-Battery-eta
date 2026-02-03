#include "EventAction.hh"
#include "RunAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"

EventAction::EventAction()
: G4UserEventAction(),
  fEnteredSiC(false),
  fTotalEdep(0.)
{}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fEnteredSiC = false;  
    fTotalEdep = 0.; 
}

void EventAction::SetEnteredSiC()
{
    fEnteredSiC = true;
}

G4bool EventAction::HasEnteredSiC() const
{
    return fEnteredSiC;
}

void EventAction::EndOfEventAction(const G4Event*)
{
    
    if (!fEnteredSiC) return;

    const G4UserRunAction* userRunAction = G4RunManager::GetRunManager()->GetUserRunAction();
    RunAction* runAction = const_cast<RunAction*>(static_cast<const RunAction*>(userRunAction));
    
    if(runAction) {
        runAction->AddEnteredEvent();
    }
}