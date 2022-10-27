#include "OpRunAction.hh"
#include "OpPrimaryGeneratorAction.hh"
#include "OpDetectorConstruction.hh"
// #include "OpRun.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpRunAction::OpRunAction()
: G4UserRunAction()
{
	fcount = 0;
	fEdep = 0;
	fTime = 0;
//  // add new units for dose
//  // 
//  const G4double milligray = 1.e-3*gray;
//  const G4double microgray = 1.e-6*gray;
//  const G4double nanogray  = 1.e-9*gray;  
//  const G4double picogray  = 1.e-12*gray;
//   
//  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
//  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
//  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
//  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray); 
//
//  // Register accumulable to the accumulable manager
//  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
//  accumulableManager->RegisterAccumulable(fEdep);
//  accumulableManager->RegisterAccumulable(fEdep2); 

  F = new TFile("out_Option.root","UPDATE");
  T = new TTree("Option","Option");
//  T -> SetBranchAddress("count",&fcount);
//  T -> SetBranchAddress("Edep",&fEdep);
//  T -> SetBranchAddress("Time",&fTime);
  T -> Branch("count",&fcount);
  T -> Branch("Edep",&fEdep);
  T -> Branch("Time",&fTime);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpRunAction::~OpRunAction()
{
	F -> Write();
	F -> Close();
//	delete F;
//	delete T;
}

void OpRunAction::BeginOfRunAction(const G4Run*)
{ 


}


void OpRunAction::EndOfRunAction(const G4Run* run)
{
}

void OpRunAction::fill(G4int count, G4double edep, G4double time)
{
	fcount = count;
	fEdep = edep;
	fTime = time;
	T -> Fill();
}
