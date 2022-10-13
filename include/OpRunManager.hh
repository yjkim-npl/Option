#ifndef OpRunManager_h
#define OpRunManager_h 1

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TObjArray.h"
#include "TString.h"

class OpRunManager : public G4RunManager
{
	public:
		OpRunManager();
		virtual ~OpRunManager();

		virtual void Initialize();
		void AddMCHit();
		void AddMCTrack();
		void AddMCPostTrack();
		void WriteToFile(TObject* obj);
		void EndOfRun();


	private:
		void SetOutputFile(TString name);
		TFile* fFile;
		TTree* fTree;
		TClonesArray* fTrackArray;
		TClonesArray* fPostTrackArray;
		TClonesArray* fHitArray;

}
#endif 
