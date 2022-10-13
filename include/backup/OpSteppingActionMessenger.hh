#ifndef OpSteppingActionMessenger_h
#define OpSteppingActionMessenger_h 1

#include "G4UImessenger.hh"

class OpSteppingAction;

class G4UIdirectory;
class G4UIcmdWithAnInteger;

class OpSteppingActionMessenger : public G4UImessenger
{
 public:
  OpSteppingActionMessenger(OpSteppingAction*);
  ~OpSteppingActionMessenger();

  void SetNewValue(G4UIcommand*, G4String) override;

 private:
  OpSteppingAction* fSteppingAction;
  G4UIdirectory* fSteppingDir;
  G4UIcmdWithAnInteger* fSetBounceLimitCmd;
};

#endif
