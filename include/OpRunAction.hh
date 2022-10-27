#ifndef OpRunAction_h
#define OpRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

#include "TFile.h"
#include "TTree.h"

class G4Run;

class OpRunAction : public G4UserRunAction
{
  public:
    OpRunAction();
    virtual ~OpRunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

//    void AddEdep (G4double edep); 

	TFile* F;
	TTree* T;

	void fill(G4int count, G4double edep, G4double time);

	int fcount;
	double fEdep;
	double fTime;

  private:
};

#endif

