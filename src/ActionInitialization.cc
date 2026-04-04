#include "ActionInitialization.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh" 

ActionInitialization::ActionInitialization()
: G4VUserActionInitialization()
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

  SetUserAction(new SteppingAction(eventAction, runAction));
}