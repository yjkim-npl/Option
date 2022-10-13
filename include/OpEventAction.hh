#ifndef OpEventAction_h
#define OpEventAction_h 1

#include "G4Types.hh"
#include "G4UserEventAction.hh"


class OpEventAction : public G4UserEventAction
{
 public:
  OpEventAction();
  ~OpEventAction();

  void BeginOfEventAction(const G4Event*) override;
  void EndOfEventAction(const G4Event*) override;

  void fillLeaks(G4double, tuple<G4double,G4double,G4double>,tuple<G4double,G4double,G4double>);
//  G4int GetEventNo();
//  void SetEventVerbose(G4int);

//  void AddTIR() { fNTIR += 1; };
//  void AddExiting() { fNExiting += 1; };
//  void AddEscapedEnd() { fEscapedEnd += 1; };
//  void AddEscapedMid() { fEscapedMid += 1; };
//  void AddBounce() { fBounce += 1; };
//  void AddOpBounce() { fOpBounce += 1; };
//  void AddClad1Bounce() { fClad1Bounce += 1; };
//  void AddClad2Bounce() { fClad2Bounce += 1; };
//  void AddReflected() { fReflected += 1; };
//  void AddDetected() { fDetected += 1; };
//  void AddEscaped() { fEscaped += 1; };
//  void AddMirror() { fMirror += 1; };

 private:

  void clear();
  void fillHits(OpPhotonDetHit* hit);
  void fillPtcs(G4PrimaryVertex* vtx, G4PrimaryParticle* ptc);

int evtNum;

  std::vector<int> count;		// MPPC count
  std::vector<int> fPhotonDetID;	// MPPC ID
  std::vector<tuple<G4double,G4double,G4double>> fPhotonDetPos;

  std::vector<G4int> fCollID;	// HC ID
  std::vector<tuple<G4double,G4double,G4double>> photon_pos;

  std::vector<G4double> leak_E;
  std::vector<tuple<G4double,G4double,G4double>> leak_p;
  std::vector<tuple<G4double,G4double,G4double>> leak_v;
  std::vector<G4int> leak_pdgID;

  std::vector<G4double> Gen_E;
  std::vector<tuple<G4double,G4double,G4double>> Gen_p;
  std::vector<tuple<G4double,G4double,G4double>> Gen_v;
  std::vector<G4double> Gen_t;
  std::vector<G4int> Gen_pdgID;
//  G4int fVerboseLevel;
//
//  G4int fMPPCCollID;
//
//  G4int fNTIR;
//  G4int fNExiting;
//  G4int fEscapedEnd;
//  G4int fEscapedMid;
//  G4int fBounce;
//  G4int fOpBounce;
//  G4int fClad1Bounce;
//  G4int fClad2Bounce;
//  G4int fReflected;
//  G4int fDetected;
//  G4int fEscaped;
//  G4int fMirror;
};

#endif
