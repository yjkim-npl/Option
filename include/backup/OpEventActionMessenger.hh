#ifndef OpEventActionMessenger_h
#define OpEventActionMessenger_h 1

#include "G4UImessenger.hh"

class OpEventAction;

class G4UIcmdWithAnInteger;

class OpEventActionMessenger : public G4UImessenger
{
 public:
  OpEventActionMessenger(OpEventAction*);
  ~OpEventActionMessenger();

  void SetNewValue(G4UIcommand*, G4String) override;

 private:
  OpEventAction* fEventAction;

  G4UIcmdWithAnInteger* fSetVerboseCmd;
};

#endif
