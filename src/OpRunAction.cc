#include "OpRunAction.hh"
#include "OpEventAction.hh"
#include "OpPrimaryGeneratorAction.hh"

//#include "OpDetectorConstruction.hh"
//#include "OpRun.hh"
//#include "OpSteppingAction.hh"

#include "TSystem.h"
#include "TObjString.h"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"
#include <vector>

OpRunAction::OpRunAction(G4int seed, G4String filename)
  : G4UserRunAction()
{
	fSeed = seed;
	fFilename = filename;
}

OpRunAction::~OpRunAction() 
{
	delete G4AnalysisManager::Instance();
}

G4Run* OpRunAction::GenerateRun()
{
  fRun = new OpRun();
  return fRun;
}

void OpRunAction::BeginOfRunAction(const G4Run*)
{
	G4AnalysisManager* anaMan = G4AnalysisManager::Instance();
	anaMan -> OpenFile("data");
}

void OpRunAction::EndOfRunAction(const G4Run*)
{
  if(isMaster)
    fRun->EndOfRun();
}
