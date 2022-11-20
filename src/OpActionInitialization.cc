#include "OpActionInitialization.hh"
#include "OpDetectorConstruction.hh"
#include "OpPrimaryGeneratorAction.hh"
#include "OpRunAction.hh"
#include "OpEventAction.hh"
#include "OpSteppingAction.hh"
#include "OpTrackingAction.hh"
#include "OpParameterContainer.hh"

OpActionInitialization::OpActionInitialization(OpParameterContainer* parC)
 : G4VUserActionInitialization()
{
	parameterContainer = parC;
}

OpActionInitialization::~OpActionInitialization()
{}

void OpActionInitialization::BuildForMaster() const
{
  OpRunAction* runAction = new OpRunAction(parameterContainer);
  SetUserAction(runAction);
}

void OpActionInitialization::Build() const
{
  SetUserAction(new OpPrimaryGeneratorAction(parameterContainer));

  OpDetectorConstruction* det = new OpDetectorConstruction(parameterContainer);
  OpRunAction* runAction = new OpRunAction(parameterContainer);
  SetUserAction(runAction);
  
  OpEventAction* eventAction = new OpEventAction(runAction,det);
  SetUserAction(eventAction);
  
  SetUserAction(new OpTrackingAction(runAction,eventAction));
  SetUserAction(new OpSteppingAction(eventAction,runAction));
}  
