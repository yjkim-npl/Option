#include "OpSteppingAction.hh"

#include "OpDetectorConstruction.hh"
#include "OpEventAction.hh"
#include "OpRunAction.hh"
//#include "OpPhotonDetSD.hh"
//#include "OpUserTrackInformation.hh"

#include "G4ios.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpticalPhoton.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Run.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4UImanager.hh"
#include "G4VPhysicalVolume.hh"


OpSteppingAction::OpSteppingAction(OpEventAction* event, OpRunAction* run)
  : fEventAction(event),
	fRunAction(run)
{
}
OpSteppingAction::~OpSteppingAction()
{}


void OpSteppingAction::UserSteppingAction(const G4Step* step)
{
	G4Track* track = step->GetTrack();
	G4int trackID = track->GetTrackID();
	G4ParticleDefinition* particle = track -> GetDefinition();

	G4StepPoint* prevPoint  = step->GetPreStepPoint();
	G4Material* prevMaterial = prevPoint -> GetMaterial();
    
	G4StepPoint* postPoint = step->GetPostStepPoint();
	G4Material* postMaterial = postPoint -> GetMaterial();
	if(postPoint->GetStepStatus() != fGeomBoundary && prevPoint->GetStepStatus() != fGeomBoundary)
		return;
	if(particle != G4OpticalPhoton::OpticalPhotonDefinition()) return;

	if(postMaterial->GetName().find("Silicon") != std::string::npos)
	{
    	if(prevMaterial->GetName().find("Polystyrene") != std::string::npos)
		{
			fEventAction -> photon_count(trackID,SiPM);
			// wavlen
			G4double energy = track -> GetTotalEnergy();
			fRunAction -> fillE(energy,SiPM);

			G4double wav = ETowav(energy);
			fRunAction -> fillWavlen(wav);

			// time
			G4double time = postPoint -> GetGlobalTime();
			fRunAction -> fillTime(time);

			// edep

			// DetID
			G4int detID = postPoint -> GetPhysicalVolume() -> GetCopyNo();
			fRunAction -> fillDetID(detID);

			// final position : Vxyz
			G4ThreeVector pos = postPoint -> GetPosition();
			fRunAction -> fillV(trackID, pos.x(),pos.y(),pos.z(), SiPM);
			// initial & final E : E_if
			// initial momentum Pi
			// final momentum Pf
			G4ThreeVector mom = track -> GetMomentum();
			fRunAction -> fillP(trackID,mom.x(),mom.y(),mom.z(),SiPM);
		}
	}
	if(postMaterial->GetName().find("G4_AIR") != std::string::npos)
	{
    	if(prevMaterial->GetName().find("Polystyrene") != std::string::npos)
	    {
//			std::cout << "Optical photon at the boundary of SC and air" << std::endl;
		  	G4double prev_px = prevPoint->GetMomentumDirection().x();
  			G4double prev_py = prevPoint->GetMomentumDirection().y();
		  	G4double prev_pz = prevPoint->GetMomentumDirection().z();

		  	G4double post_px = postPoint->GetMomentumDirection().x();
		  	G4double post_py = postPoint->GetMomentumDirection().y();
		  	G4double post_pz = postPoint->GetMomentumDirection().z();
			G4double pos_x = prevPoint -> GetPosition().x();
			G4double pos_y = prevPoint -> GetPosition().y();
			G4double pos_z = prevPoint -> GetPosition().z();
//			std::cout << prev_x << " " << prev_y << " " << prev_z << " " << std::endl;
//			std::cout <<trackID << " " <<  pos_x << " " << pos_y << " " << pos_z << " " << std::endl << std::endl;
//			if(fRunAction->find_track(trackID) == false)	
		  	fEventAction -> photon_count(trackID,Air);

		  	G4double energy = track -> GetTotalEnergy();
	  		fRunAction -> fillE(energy,Air);
			if(fRunAction->Refl(prevPoint->GetMomentumDirection(),
								postPoint->GetMomentumDirection()))
			{
				fRunAction -> fillV(trackID,pos_x,pos_y,pos_z,Air);
				fRunAction -> fillP(trackID,prev_px,prev_py,prev_pz,Air);
			}
	    }
	}
}
