#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

namespace B1
{

ActionInitialization::ActionInitialization(RunAction* runAction) 
    : G4VUserActionInitialization(), fRunAction(runAction) {}

void ActionInitialization::BuildForMaster() const
{
    SetUserAction(fRunAction);  // Use the same RunAction instance
}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGeneratorAction);
    SetUserAction(fRunAction); 
    SetUserAction(new EventAction(fRunAction));

}

}

