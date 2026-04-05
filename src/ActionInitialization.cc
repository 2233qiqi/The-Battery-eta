#include "ActionInitialization.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh" 
#include "DetectorConstruction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction* det)
: G4VUserActionInitialization(),
  fDetector(det)
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
  auto runAction = new RunAction();
  SetUserAction(runAction);

  SetUserAction(new PrimaryGeneratorAction(runAction));

  auto eventAction = new EventAction();
  SetUserAction(eventAction);

  SetUserAction(new SteppingAction(eventAction, runAction, fDetector));
}