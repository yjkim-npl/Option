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
  OpSteppingAction(OpDetectorConstruction*, OpEventAction*);
  ~OpSteppingAction();

  void UserSteppingAction(const G4Step*) override;

  // Set the bounce limit, 0 for no limit
//  void SetBounceLimit(G4int);

//  G4int GetNumberOfBounces();
//  G4int GetNumberOfClad1Bounces();
//  G4int GetNumberOfClad2Bounces();
//  G4int GetNumberOfOpBounces();
  // return number of successful events and reset the counter
//  G4int ResetSuccessCounter();

 private:
//  // Artificially kill the photon after it has bounced more than this number
//  G4int fBounceLimit;
//  // number of photons that reach the end
//  G4int fCounterEnd;
//  // number of photons that didn't make it to the end
//  G4int fCounterMid;
//  // total number of bounces that a photon been through
//  G4int fCounterBounce;
//  // number of bounces that a photon been through within the fibre
//  G4int fCounterOpBounce;
//  // number of bounces that a photon been through from Cladding 1 to 2
//  G4int fCounterClad1Bounce;
//  // number of bounces that a photon been through from Cladding 2 to World
//  G4int fCounterClad2Bounce;
//
//  // initial gamma of the photon
//  G4double fInitGamma;
//  // initial theta of the photon
//  G4double fInitTheta;
//
//  G4OpBoundaryProcess* fOpProcess;

  OpDetectorConstruction* fDetector;
  OpEventAction* fEventAction;

  G4double leak_E;
  tuple<G4double,G4double,G4double> leak_p;
  tuple<G4double,G4double,G4double> leak_v;
  G4int pdgID;

//  inline void ResetCounters()
//  {
//    fCounterBounce      = 0;
//    fCounterOpBounce   = 0;
//    fCounterClad1Bounce = 0;
//    fCounterClad2Bounce = 0;
//    fInitGamma          = -1;
//    fInitTheta          = -1;
//  }
};

#endif
