#include "OpEventActionMessenger.hh"

#include "OpEventAction.hh"

#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpEventActionMessenger::OpEventActionMessenger(OpEventAction* eventaction)
  : fEventAction(eventaction)
{
  fSetVerboseCmd = new G4UIcmdWithAnInteger("/Op/eventVerbose", this);
  fSetVerboseCmd->SetGuidance("Set verbose level .");
  fSetVerboseCmd->SetParameterName("level", true);
  fSetVerboseCmd->SetDefaultValue(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpEventActionMessenger::~OpEventActionMessenger() { delete fSetVerboseCmd; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpEventActionMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{
  if(command == fSetVerboseCmd)
    fEventAction->SetEventVerbose(fSetVerboseCmd->GetNewIntValue(newValue));
}
