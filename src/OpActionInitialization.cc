#include "OpActionInitialization.hh"
#include "OpPrimaryGeneratorAction.hh"
#include "OpRunAction.hh"
#include "OpEventAction.hh"
#include "OpSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpActionInitialization::OpActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpActionInitialization::~OpActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpActionInitialization::BuildForMaster() const
{
  OpRunAction* runAction = new OpRunAction;
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpActionInitialization::Build() const
{
  SetUserAction(new OpPrimaryGeneratorAction);

  OpRunAction* runAction = new OpRunAction;
  SetUserAction(runAction);
  
  OpEventAction* eventAction = new OpEventAction(runAction);
  SetUserAction(eventAction);
  
  SetUserAction(new OpSteppingAction(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......