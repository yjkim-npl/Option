#ifndef OpSD_h
#define OpSD_h 1

#include "OpHit.hh"
#include "G4VSensitiveDetecgtor.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"

class OpSD : public G4VSensitiveDetector
{
	public:
		OpSD(const G4String& name, const G4String& HCname);
		virtual ~OpSD();

		virtual void Initialize(G4HCofThisEvent* HCE);
		virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory*);
		virtual void EndOfEvent(G4HCofThisEvent* HCE);

	private:
		OpHitsCollection* fHitCollection;
};
#endif
