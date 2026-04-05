#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class EventAction;
class RunAction;
class DetectorConstruction;

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(EventAction* eventAction, RunAction* runAction, DetectorConstruction* det);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);

private:
    EventAction* fEventAction;
    RunAction* fRunAction;
    DetectorConstruction* fDetector;
};

#endif