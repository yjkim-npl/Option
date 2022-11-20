#include "OpTrackingAction.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4OpticalPhoton.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

OpTrackingAction::OpTrackingAction(OpRunAction* runAction,OpEventAction* eventAction)
: G4UserTrackingAction()
{
	fRunAction = runAction;
	fEventAction = eventAction;
}

void OpTrackingAction::PreUserTrackingAction(G4Track* track)
{
	G4int ftrackID = track -> GetTrackID();
	if(track -> GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) return;
	std::cout << fEventAction->GetCount(Scintillator) << std::endl;
	if(fEventAction->GetCount(Scintillator) !=0) track->SetTrackStatus(fStopAndKill);
	G4ThreeVector mom = track -> GetMomentum();
	G4double E = track -> GetTotalEnergy();
//	G4ThreeVector position = track -> Get
	fEventAction -> photon_count(ftrackID,Scintillator);
	fRunAction -> filltrackID(ftrackID);
	fRunAction -> fillE(E, Scintillator);
	fRunAction -> fillP(ftrackID, mom.x(),mom.y(),mom.z(), Scintillator);
}

