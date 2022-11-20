#ifndef OpRunAction_h
#define OpRunAction_h 1

#include "OpParameterContainer.hh"
#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

#include "TFile.h"
#include "TTree.h"

#include <vector>
#include <tuple>
using namespace std;

class G4Run;

enum {SiPM,Air,Scintillator};

class OpRunAction : public G4UserRunAction
{
  public:
    OpRunAction(OpParameterContainer* par);
    virtual ~OpRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

	void clear();
	void fillcount(G4int count, G4int OPT);
	void fillE(G4double e, G4int OPT);
	void fillWavlen(G4double wavlen); //	{fWavlen.push_back(wavlen);}
	void fillTime(G4double time); 	//	{fTime.push_back(time);}
	void fillDetID(G4int ID);		//	{fDetID.push_back(ID);}
	void fillV(G4int ID, G4double X,G4double Y,G4double Z, G4int OPT);
	void fillP(G4int ID,G4double pX, G4double pY, G4double pZ, G4int OPT);
	void filltrackID(G4int ID);
	bool Refl(G4ThreeVector prev_p, G4ThreeVector post_p);
	void update()
		{T -> Fill();}
	bool find_track(G4int ID);
	int find_index(int* a);
	int find_index(double* a);

  private:
	OpParameterContainer* OpPar;
	TFile* F;
	TTree* T;

	bool OpatSiPM;
	bool OpfromSC;
	bool OpatAir;


	// Optical photon at SiPM
	int fcount;
	double fE[500];
	double fWavlen[500];
	double fTime[500];
	double fVx[500];
	double fVy[500];
	double fVz[500];
	double fPx[500];
	double fPy[500];
	double fPz[500];
	int   fDetID[500];

	// Optical photon from Scintillator
	int fcount_Scint;
	int trackID[500];
	double fPx_Scint[500];
	double fPy_Scint[500];
	double fPz_Scint[500];
	double fE_Scint[500];

	// Optical photon at Air
	int fcount_air;
	double fE_air[500];
	bool edge;
	int n_refl;
	vector<double> fVx_air;
	vector<double> fVy_air;
	vector<double> fVz_air;
	vector<double> fPx_air;
	vector<double> fPy_air;
	vector<double> fPz_air;
};

#endif

