#ifndef OpSteppingAction_h
#define OpSteppingAction_h 1

#include "G4Types.hh"
#include "G4UserSteppingAction.hh"

class OpDetectorConstruction;
class OpEventAction;

class G4OpBoundaryProcess;
class G4Track;
class G4StepPoint;

class OpSteppingAction : public G4UserSteppingAction
{
 public:
  OpSteppingAction(OpEventAction*);
  ~OpSteppingAction();

  void UserSteppingAction(const G4Step*) override;

 private:
  OpDetectorConstruction* fDetector;
  OpEventAction* fEventAction;

  G4double leak_E;
  std::tuple<G4double,G4double,G4double> leak_p;
  std::tuple<G4double,G4double,G4double> leak_v;
  G4int pdgID;

};

#endif
