#ifndef OpRunAction_h
#define OpRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "globals.hh"
//#include "g4root.hh"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TObjArray.h"


//class OpRun;

class G4Run;

class OpRunAction : public G4UserRunAction
{
 public:
  OpRunAction(G4int seed, G4String filename);
  ~OpRunAction();

  void BeginOfRunAction(const G4Run*) override;
  void EndOfRunAction(const G4Run*) override;
  G4Run* GenerateRun() override;

 private:
  G4int fSeed;
  G4String fFilename;
  TFile* fFile;
  TTree* fTree;
  TClonesArray* fTrackArray;
  TClonesArray* fPostTrackArray;

  bool fMCTrack = false;
  bool fMCPostTrack = false;
//  OpRun* fRun;
};

#endif
