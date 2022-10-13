#include "OpActionInitialization.hh"

#include "OpDetectorConstruction.hh"
#include "OpEventAction.hh"
#include "OpPrimaryGeneratorAction.hh"
#include "OpRunAction.hh"
#include "OpStackingAction.hh"
#include "OpSteppingAction.hh"
#include "OpTrackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpActionInitialization::OpActionInitialization(OpDetectorConstruction* det)
  : G4VUserActionInitialization()
  , fDetector(det)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpActionInitialization::~OpActionInitialization() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpActionInitialization::BuildForMaster() const
{
  SetUserAction(new OpRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpActionInitialization::Build() const
{
  SetUserAction(new OpPrimaryGeneratorAction(fDetector));
  SetUserAction(new OpRunAction());

  OpEventAction* eventAction = new OpEventAction();
  SetUserAction(eventAction);
  
  SetUserAction(new OpTrackingAction());
  SetUserAction(new OpSteppingAction(fDetector, eventAction));
  SetUserAction(new OpStackingAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
