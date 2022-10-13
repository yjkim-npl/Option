#ifndef OpPhotonDetSD_h
#define OpPhotonDetSD_h 1

#include "OpPhotonDetHit.hh"

#include "G4VSensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"

class G4Step;
class G4HCofThisEvent;

class OpPhotonDetSD : public G4VSensitiveDetector
{
 public:
  OpPhotonDetSD(G4String name, G4String HCname, G4int OpPhotonDetID);
  virtual ~OpPhotonDetSD();

  virtual void Initialize(G4HCofThisEvent*);// override;
  
  G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
  // A version of processHits, not called automatically by sensitive detector
//  G4bool ProcessHits_boundary(const G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent* HCE);

 private:
  OpPhotonDetHitsCollection* fPhotonDetHitCollection;
  G4int fOpPhotonDetID;
  G4int fHCID;
  G4double wavToE(G4double wav) {return h_Plank* c_light/wav;}
  G4double ETowav(G4double E) {return h_Plank * c_light / E;}
};

#endif
