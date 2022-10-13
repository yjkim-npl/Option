#include "OpPrimaryGeneratorMessenger.hh"

#include "OpPrimaryGeneratorAction.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIdirectory.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpPrimaryGeneratorMessenger::OpPrimaryGeneratorMessenger(
  OpPrimaryGeneratorAction* gun)
  : fAction(gun)
{
  fGunDir = new G4UIdirectory("/Op/gun/");
  fGunDir->SetGuidance("OpPrimaryGenerator control");

  fSetPolarizationCmd =
    new G4UIcmdWithADoubleAndUnit("/Op/gun/optPhotonPolar", this);
  fSetPolarizationCmd->SetGuidance("Set linear polarization");
  fSetPolarizationCmd->SetGuidance("  angle w.r.t. (k,n) plane");
  fSetPolarizationCmd->SetParameterName("angle", true);
  fSetPolarizationCmd->SetUnitCategory("Angle");
  fSetPolarizationCmd->SetDefaultValue(0.);
  fSetPolarizationCmd->AvailableForStates(G4State_Idle);

  fSetDecayTimeConstantCmd =
    new G4UIcmdWithADoubleAndUnit("/Op/gun/setDecayTimeConstant", this);
  fSetDecayTimeConstantCmd->SetGuidance("Set the decay time constant");
  fSetDecayTimeConstantCmd->SetGuidance("for the starting time of each photon");
  fSetDecayTimeConstantCmd->SetParameterName("time_const", false);
  fSetDecayTimeConstantCmd->SetUnitCategory("Time");
  fSetDecayTimeConstantCmd->SetRange("time_const>=0");
  fSetDecayTimeConstantCmd->AvailableForStates(G4State_Idle);

  // fSetUseSampledEnergyCmd = new G4UIcmdWithABool("/Op/gun/useSampledEnergy",
  // this); fSetUseSampledEnergyCmd->SetGuidance("Enable sampling of primary
  // energy."); fSetUseSampledEnergyCmd->SetGuidance("This is currently
  // disabled."); fSetUseSampledEnergyCmd->AvailableForStates();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpPrimaryGeneratorMessenger::~OpPrimaryGeneratorMessenger()
{
  delete fGunDir;
  delete fSetPolarizationCmd;
  delete fSetDecayTimeConstantCmd;
  // delete fSetUseSampledEnergyCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4String val)
{
  if(command == fSetPolarizationCmd)
    fAction->SetOptPhotonPolar(
      G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  else if(command == fSetDecayTimeConstantCmd)
    fAction->SetDecayTimeConstant(
      G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  // else if ( command == fSetUseSampledEnergyCmd )
  //   fAction->
  //     SetUseSampledEnergy(G4UIcmdWithABool::GetNewBoolValue(val));
}
