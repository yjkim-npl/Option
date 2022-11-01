#ifndef OpRunAction_h
#define OpRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

#include "TFile.h"
#include "TTree.h"

#include <vector>
#include <tuple>
using namespace std;

class G4Run;

class OpRunAction : public G4UserRunAction
{
  public:
    OpRunAction();
    virtual ~OpRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

	void clear();
	void fillcount_i(G4int count)			{fcount_i = count;}
	void fillcount(G4int count)			{fcount = count;}
	void fillE(G4double e);
	void fillEi(G4double e);
	void fillWavlen(G4double wavlen); //	{fWavlen.push_back(wavlen);}
	void fillTime(G4double time); 	//	{fTime.push_back(time);}
	void fillDetID(G4int ID);		//	{fDetID.push_back(ID);}
	void fillVxyz(G4double X,G4double Y,G4double Z);
	void fillPi(G4double pX, G4double pY, G4double pZ);
	void fillPf(G4double pX, G4double pY, G4double pZ);
	void update()
		{T -> Fill();}
	int find_index(int* a);
	int find_index(double* a);

  private:
	TFile* F;
	TTree* T;


	int fcount_i;
	int fcount;
	double fE[500];
	double fE_i[500];
	double fWavlen[500];
	double fTime[500];
	int   fDetID[500];
	double fVx[500];
	double fVy[500];
	double fVz[500];
	double fPx[500];
	double fPx_i[500];
	double fPy[500];
	double fPy_i[500];
	double fPz[500];
	double fPz_i[500];

};

#endif

