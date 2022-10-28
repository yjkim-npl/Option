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
	void fillcount(G4int count)			{fcount = count;}
	void fillWavlen(G4double wavlen) 	{fWavlen.push_back(wavlen);}
	void fillTime(G4double time) 		{fTime.push_back(time);}
	void fillDetID(G4int ID)			{fDetID.push_back(ID);}
	void fillVxyz(G4double X,G4double Y,G4double Z);
	void fillPf(G4double pX, G4double pY, G4double pZ);
	void update()
		{T -> Fill();}

  private:
	TFile* F;
	TTree* T;

	int fcount;
	vector<double> fWavlen;
	vector<double> fTime;
	vector<int> fDetID;
	vector<double> fVx;
	vector<double> fVy;
	vector<double> fVz;
	vector<double> fPx;
	vector<double> fPy;
	vector<double> fPz;

};

#endif

