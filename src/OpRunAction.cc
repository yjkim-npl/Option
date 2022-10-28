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

OpRunAction::OpRunAction()
: G4UserRunAction()
{
	fcount = 0;

  F = new TFile("out_Option.root","UPDATE");
  T = new TTree("Option","Option");
//  T -> SetBranchAddress("count",&fcount);
//  T -> SetBranchAddress("Edep",&fEdep);
//  T -> SetBranchAddress("Time",&fTime);
//  T -> Branch("Edep",fEdep);
//  T -> Branch("E_if",fE_if);
//  T -> Branch("P_i",fP_i);
  T -> Branch("count",&fcount);
  T -> Branch("Time",&fTime);
  T -> Branch("DetID",&fDetID);
  T -> Branch("Vx",&fVx);
  T -> Branch("Vy",&fVy);
  T -> Branch("Vz",&fVz);
  T -> Branch("Px",&fPx);
  T -> Branch("Py",&fPy);
  T -> Branch("Pz",&fPz);
}


OpRunAction::~OpRunAction()
{
	F -> Write();
	F -> Close();
}

void OpRunAction::BeginOfRunAction(const G4Run*)
{ 
}


void OpRunAction::EndOfRunAction(const G4Run* run)
{
}

void OpRunAction::clear()
{
	fWavlen.clear();
	fTime.clear();
	fDetID.clear();
	fVx.clear();
	fVy.clear();
	fVz.clear();
	fPx.clear();
	fPy.clear();
	fPz.clear();
}

void OpRunAction::fillVxyz(G4double X, G4double Y, G4double Z)
{
	fVx.push_back(X);
	fVy.push_back(Y);
	fVz.push_back(Z);
}

void OpRunAction::fillPf(G4double pX, G4double pY, G4double pZ)
{
	fPx.push_back(pX);
	fPy.push_back(pY);
	fPz.push_back(pZ);
}
