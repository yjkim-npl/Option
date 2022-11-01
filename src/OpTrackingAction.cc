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

void OpTrackingAction::PreUserTrackingAction(const G4Track* track)
{
	if(track -> GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) return;
	G4ThreeVector mom = track -> GetMomentum();
	G4double E = track -> GetTotalEnergy();
//	G4ThreeVector position = track -> Get
	fEventAction -> photon_count_i();
	fRunAction -> fillEi(E);
	fRunAction -> fillPi(mom.x(),mom.y(),mom.z());
}

