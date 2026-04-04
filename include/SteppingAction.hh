#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class EventAction;
class RunAction;

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(EventAction* eventAction, RunAction* runAction);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);

private:
    EventAction* fEventAction;
    RunAction* fRunAction;
};

#endif