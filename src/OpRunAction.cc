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

//#include <stdio.h>
//#include <algorithm>

OpRunAction::OpRunAction()
: G4UserRunAction()
{
	fcount = 0;
	fcount_i = 0;

  F = new TFile("out_Option.root","recreate");
  T = new TTree("Option","Option");
//  T -> SetBranchAddress("count",&fcount);
//  T -> SetBranchAddress("Edep",&fEdep);
//  T -> SetBranchAddress("Time",&fTime);
//  T -> Branch("Edep",fEdep);
//  T -> Branch("E_if",fE_if);
//  T -> Branch("P_i",fP_i);
  T -> Branch("count",&fcount);
  T -> Branch("count_i",&fcount_i);
  T -> Branch("E",fE,"E[count]/D");
  T -> Branch("E_i",fE_i,"E_i[count_i]/D");
  T -> Branch("Wavlen",fWavlen,"Wavlen[count]/D");
  T -> Branch("Time",fTime,"Time[count]/D");
  T -> Branch("DetID",fDetID,"DetID[count]/I");
  T -> Branch("Vx",fVx,"Vx[count]/D");
  T -> Branch("Vy",fVy,"Vy[count]/D");
  T -> Branch("Vz",fVz,"Vz[count]/D");
  T -> Branch("Px",fPx,"Px[count]/D");
  T -> Branch("Px_i",fPx_i,"Px_i[count_i]/D");
  T -> Branch("Py",fPy,"Py[count]/D");
  T -> Branch("Py_i",fPy_i,"Py_i[count_i]/D");
  T -> Branch("Pz",fPz,"Pz[count]/D");
  T -> Branch("Pz_i",fPz_i,"Pz_i[count_i]/D");
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
	std::fill_n(fE,500,0);
	std::fill_n(fE_i,500,0);
	std::fill_n(fWavlen,500,0);
	std::fill_n(fTime,500,0);
	std::fill_n(fDetID,500,0);
	std::fill_n(fVx,500,0);
	std::fill_n(fVy,500,0);
	std::fill_n(fVz,500,0);
	std::fill_n(fPx,500,0);
	std::fill_n(fPx_i,500,0);
	std::fill_n(fPy,500,0);
	std::fill_n(fPy_i,500,0);
	std::fill_n(fPz,500,0);
	std::fill_n(fPz_i,500,0);
//	fWavlen.clear();
//	fTime.clear();
//	fDetID.clear();
//	fVx.clear();
//	fVy.clear();
//	fVz.clear();
//	fPx.clear();
//	fPy.clear();
//	fPz.clear();
}

void OpRunAction::fillEi(G4double e)
{
	int idx = find_index(fE_i);
	fE_i[idx] = e;
}
void OpRunAction::fillE(G4double e)
{
	int idx = find_index(fE);
	fE[idx] = e;
}
void OpRunAction::fillWavlen(G4double wavlen)
{
	int idx = find_index(fWavlen);
	fWavlen[idx] = wavlen;
}

void OpRunAction::fillTime(G4double time)
{
	int idx = find_index(fTime);
	fTime[idx] = time;
}
void OpRunAction::fillDetID(G4int ID)
{
	int idx = find_index(fDetID);
	fDetID[idx] = ID;
}
void OpRunAction::fillVxyz(G4double X, G4double Y, G4double Z)
{
	int idx = find_index(fVx);
	fVx[idx] = X;
	fVy[idx] = Y;
	fVz[idx] = Z;
//	fVx.push_back(X);
//	fVy.push_back(Y);
//	fVz.push_back(Z);
}

void OpRunAction::fillPi(G4double pX, G4double pY, G4double pZ)
{
	int idx = find_index(fPx_i);
	fPx_i[idx] = pX;
	fPy_i[idx] = pY;
	fPz_i[idx] = pZ;
}

void OpRunAction::fillPf(G4double pX, G4double pY, G4double pZ)
{
	int idx = find_index(fPx);
	fPx[idx] = pX;
	fPy[idx] = pY;
	fPz[idx] = pZ;
//	fPx.push_back(pX);
//	fPy.push_back(pY);
//	fPz.push_back(pZ);
}

int OpRunAction::find_index(int* a)
{
	for(int b=0; b<500; b++)
	{
		if(a[b]!=0) continue;
		else return b;
	}
}

int OpRunAction::find_index(double* a)
{
	for(int b=0; b<500; b++)
	{
		if(a[b]!=0) continue;
		else return b;
	}
}

