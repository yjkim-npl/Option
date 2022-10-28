#ifndef OpSteppingAction_h
#define OpSteppingAction_h 1

#include "OpRunAction.hh"
#include "OpEventAction.hh"
#include "OpDetectorConstruction.hh"

#include "G4Types.hh"
#include "G4UserSteppingAction.hh"
#include "G4PhysicalConstants.hh"

class OpDetectorConstruction;

class G4OpBoundaryProcess;
class G4Track;
class G4StepPoint;

class OpSteppingAction : public G4UserSteppingAction
{
 public:
  OpSteppingAction(OpEventAction*, OpRunAction*);
  ~OpSteppingAction();

  void UserSteppingAction(const G4Step*) override;

 private:
  G4double wavToE(G4double wav) {return h_Planck*c_light/wav;}
  G4double ETowav(G4double E) {return h_Planck*c_light/E;}
  OpDetectorConstruction* fDetector;
  OpEventAction* fEventAction;
  OpRunAction* fRunAction;

  G4double leak_E;
  std::tuple<G4double,G4double,G4double> leak_p;
  std::tuple<G4double,G4double,G4double> leak_v;
  G4int pdgID;

};

#endif
