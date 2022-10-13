#ifndef OpPrimaryGeneratorMessenger_h
#define OpPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"

class OpPrimaryGeneratorAction;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;

class OpPrimaryGeneratorMessenger : public G4UImessenger
{
 public:
  OpPrimaryGeneratorMessenger(OpPrimaryGeneratorAction*);
  ~OpPrimaryGeneratorMessenger();

  void SetNewValue(G4UIcommand*, G4String) override;

 private:
  OpPrimaryGeneratorAction* fAction;

  G4UIdirectory* fGunDir;

  G4UIcmdWithADoubleAndUnit* fSetPolarizationCmd;
  G4UIcmdWithADoubleAndUnit* fSetDecayTimeConstantCmd;
  // G4UIcmdWithABool*            fSetUseSampledEnergyCmd;
};

#endif
