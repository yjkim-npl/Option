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

//#include <cmath>
//#include <stdio.h>
//#include <algorithm>

OpRunAction::OpRunAction(OpParameterContainer* par)
: G4UserRunAction(),
  OpatSiPM(false),
  OpfromSC(false),
  OpatAir(false),
  edge(false)
{
	OpPar = par;
	OpatSiPM = OpPar->GetParBool("OpatSiPM");
	OpfromSC = OpPar->GetParBool("OpfromSC");
	OpatAir = OpPar->GetParBool("OpatAir");

	n_refl = 0;
	fcount = 0;
	fcount_Scint = 0;
	fcount_air = 0;

	F = new TFile(OpPar->GetParString("outName").c_str(),"recreate");
	T = new TTree("Opsim","Opsim");


	if(OpatSiPM)
	{
		T -> Branch("count",&fcount);
		T -> Branch("E",fE,"E[count]/D");
		T -> Branch("Wavlen",fWavlen,"Wavlen[count]/D");
		T -> Branch("Time",fTime,"Time[count]/D");
		T -> Branch("DetID",fDetID,"DetID[count]/I");
		T -> Branch("Vx",fVx,"Vx[count]/D");
		T -> Branch("Vy",fVy,"Vy[count]/D");
		T -> Branch("Vz",fVz,"Vz[count]/D");
		T -> Branch("Px",fPx,"Px[count]/D");
		T -> Branch("Py",fPy,"Py[count]/D");
		T -> Branch("Pz",fPz,"Pz[count]/D");
	}
	if(OpfromSC)
	{
		T -> Branch("count_Scint",&fcount_Scint);
		T -> Branch("trackID",trackID,"trackID[count]/I");
		T -> Branch("E_Scint",fE_Scint,"E_Scint[count_Scint]/D");
		T -> Branch("Px_Scint",fPx_Scint,"Px_Scint[count_Scint]/D");
		T -> Branch("Py_Scint",fPy_Scint,"Py_Scint[count_Scint]/D");
		T -> Branch("Pz_Scint",fPz_Scint,"Pz_Scint[count_Scint]/D");
	}
	if(OpatAir)
	{
		T -> Branch("count_air",&fcount_air);
		T -> Branch("n_refl",&n_refl);
		T -> Branch("E_air",fE_air,"E_air[count_air]/D");
		T -> Branch("Vx_air",fVx_air);//,"Vx_air[count_air]/D");
		T -> Branch("Vy_air",fVy_air);//,"Vy_air[count_air]/D");
		T -> Branch("Vz_air",fVz_air);//,"Vz_air[count_air]/D");
		T -> Branch("Px_air",fPx_air);//,"Px_air[count_air]/D");
		T -> Branch("Py_air",fPy_air);//,"Py_air[count_air]/D");
		T -> Branch("Pz_air",fPz_air);//,"Pz_air[count_air]/D");
	}
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
	std::fill_n(fWavlen,500,0);
	std::fill_n(fTime,500,0);
	std::fill_n(fDetID,500,0);
	std::fill_n(fVx,500,0);
	std::fill_n(fVy,500,0);
	std::fill_n(fVz,500,0);
	std::fill_n(fPx,500,0);
	std::fill_n(fPy,500,0);
	std::fill_n(fPz,500,0);

	std::fill_n(fE_Scint,500,0);
	std::fill_n(trackID,500,0);
	std::fill_n(fPx_Scint,500,0);
	std::fill_n(fPy_Scint,500,0);
	std::fill_n(fPz_Scint,500,0);

	std::fill_n(fE_air,500,0);
//	std::fill_n(fVx_air,500,0);
//	std::fill_n(fVy_air,500,0);
//	std::fill_n(fVz_air,500,0);
	n_refl = 0;
	fVx_air.clear();
	fVy_air.clear();
	fVz_air.clear();
	fPx_air.clear();
	fPy_air.clear();
	fPz_air.clear();
}

void OpRunAction::fillcount(G4int count, G4int OPT)
{
	if(OPT == SiPM){
		fcount = count;
	}else if (OPT == Air){
		fcount_air = count;
	}else if (OPT == Scintillator){
		fcount_Scint = count;
	}else{
		return;
	}
}

void OpRunAction::filltrackID(G4int ID)
{
	int idx = find_index(trackID);
	trackID[idx] = ID;
}

void OpRunAction::fillE(G4double e, G4int OPT)
{
	if(OPT == SiPM){
		int idx = find_index(fE);
		fE[idx] = e;
	}else if(OPT == Air){
		int idx = find_index(fE_air);
		fE_air[idx] = e;
	}else if(OPT == Scintillator){
		int idx = find_index(fE_Scint);
	}else{
		return;
	}
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
void OpRunAction::fillV(G4int ID, G4double X, G4double Y, G4double Z, G4int OPT)
{
	if(OPT == SiPM){
		int idx = find_index(fVx);
		fVx[idx] = X;
		fVy[idx] = Y;
		fVz[idx] = Z;
	}else if (OPT == Air){
		int idx = ID;
		if((fabs(X) > (0.499*OpPar->GetParDouble("SCX")) &&
			fabs(X) < (5.001*OpPar->GetParDouble("SCX")) ) ||
		   (fabs(Y) > (0.499*OpPar->GetParDouble("SCY")) &&
			fabs(X) < (5.001*OpPar->GetParDouble("SCY")) ) ||
		   (fabs(Z) > (0.499*OpPar->GetParDouble("SCZ")) &&
			fabs(Z) < (5.001*OpPar->GetParDouble("SCZ"))))// && fVz_air[idx]==0)
		{
			edge = true;
//			std::cout << X << " " << Y << " " << Z << std::endl;
			fVx_air.push_back(X);
			fVy_air.push_back(Y);
			fVz_air.push_back(Z);
//			std::cout << fVx_air[idx] << " " << fVy_air[idx] << " " << fVz_air[idx] << std::endl;
		}else{
			edge = false;
			return;
		}
	}else{
		return;
	}

}

void OpRunAction::fillP(G4int ID, G4double pX, G4double pY, G4double pZ, G4int OPT)
{
	if(OPT == SiPM){
		int idx = find_index(fPx);
		fPx[idx] = pX;
		fPy[idx] = pY;
		fPz[idx] = pZ;
	}else if(OPT == Air){
		int idx = ID;
		if(edge)
		{
			trackID[idx] = ID;
			fPx_air.push_back(pX);
			fPy_air.push_back(pY);
			fPz_air.push_back(pZ);
			edge = false;
		}
	}else{
		return;
	}
}

bool OpRunAction::Refl(G4ThreeVector prev_p, G4ThreeVector post_p)
{
	G4double prev_x = prev_p.x();
	G4double prev_y = prev_p.y();
	G4double prev_z = prev_p.z();

	G4double post_x = post_p.x();
	G4double post_y = post_p.y();
	G4double post_z = post_p.z();
	if(prev_x*post_x * prev_y*post_y * prev_z*post_z <0)
	{
		n_refl++;
		return true;
	}else{
		return false;
	}
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

bool OpRunAction::find_track(G4int ID)
{
	for(int b=0; b<500; b++)
	{
		if(trackID[b] == ID)
			return true;
	}
	return false;
}
