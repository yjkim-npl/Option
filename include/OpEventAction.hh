#ifndef OpEventAction_h
#define OpEventAction_h 1

//#include "OpPhotonDetHit.hh"
#include "OpDetectorConstruction.hh"
#include "OpRunAction.hh"

//#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include "G4Types.hh"
#include "G4UserEventAction.hh"
#include "g4root.hh"
#include "globals.hh"

#include <vector>

using namespace std;

class OpEventAction : public G4UserEventAction
{
 public:
  OpEventAction(OpRunAction* runAction,OpDetectorConstruction* det);
  ~OpEventAction();

  void BeginOfEventAction(const G4Event*) override;
  void EndOfEventAction(const G4Event*) override;

  void fillLeaks(G4double, tuple<G4double,G4double,G4double>,tuple<G4double,G4double,G4double>);
  void photon_count() {fCount++;}
  G4int GetCount() {return fCount;}

  OpRunAction* frunAction;
  OpDetectorConstruction* fDetector;
 private:
//  void fillHits(OpPhotonDetHit* hit);
  void fillPtcs(G4PrimaryVertex* vtx, G4PrimaryParticle* ptc);

  int evtNum;

  G4int fCount;		// MPPC count
};

#endif
