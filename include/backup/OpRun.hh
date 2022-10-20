#ifndef OpRun_h
#define OpRun_h 1

#include "G4Run.hh"

class OpRun : public G4Run
{
 public:
  OpRun();
  ~OpRun();

  void AddTIR(G4int n)
  {
    G4double nd(n);
    fNTIR += nd;
    fNTIR2 += nd * nd;
  };
  void AddExiting(G4int n)
  {
    G4double nd(n);
    fNExiting += nd;
    fNExiting2 += nd * nd;
  };
  void AddEscapedEnd(G4int n)
  {
    G4double nd(n);
    fEscapedEnd += nd;
    fEscapedEnd2 += nd * nd;
  };
  void AddEscapedMid(G4int n)
  {
    G4double nd(n);
    fEscapedMid += nd;
    fEscapedMid2 += nd * nd;
  };
  void AddBounce(G4int n)
  {
    G4double nd(n);
    fBounce += nd;
    fBounce2 += nd * nd;
  };
  void AddOpBounce(G4int n)
  {
    G4double nd(n);
    fOpBounce += nd;
    fOpBounce2 += nd * nd;
  };
  void AddClad1Bounce(G4int n)
  {
    G4double nd(n);
    fClad1Bounce += nd;
    fClad1Bounce2 += nd * nd;
  };
  void AddClad2Bounce(G4int n)
  {
    G4double nd(n);
    fClad2Bounce += nd;
    fClad2Bounce2 += nd * nd;
  };
  void AddReflected(G4int n)
  {
    G4double nd(n);
    fReflected += nd;
    fReflected2 += nd * nd;
  };
  void AddDetected(G4int n)
  {
    G4double nd(n);
    fDetected += nd;
    fDetected2 += nd * nd;
  };
  void AddEscaped(G4int n)
  {
    G4double nd(n);
    fEscaped += nd;
    fEscaped2 += nd * nd;
  };
  void AddMirror(G4int n)
  {
    G4double nd(n);
    fMirror += nd;
    fMirror2 += nd * nd;
  };
  void AddDetectorHits(G4int n)
  {
    G4double nd(n);
    fDetectorHits += nd;
    fDetectorHits2 += nd * nd;
  };

  void EndOfRun();
  void Merge(const G4Run*) override;

 private:
  G4double fNTIR;
  G4double fNTIR2;
  G4double fNExiting;
  G4double fNExiting2;
  G4double fEscapedEnd;
  G4double fEscapedEnd2;
  G4double fEscapedMid;
  G4double fEscapedMid2;
  G4double fBounce;
  G4double fBounce2;
  G4double fOpBounce;
  G4double fOpBounce2;
  G4double fClad1Bounce;
  G4double fClad1Bounce2;
  G4double fClad2Bounce;
  G4double fClad2Bounce2;
  G4double fReflected;
  G4double fReflected2;
  G4double fDetected;
  G4double fDetected2;
  G4double fEscaped;
  G4double fEscaped2;
  G4double fMirror;
  G4double fMirror2;
  G4double fDetectorHits;
  G4double fDetectorHits2;
};

#endif
