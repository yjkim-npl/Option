#include "OpPhotonDetSD.hh"
#include "OpPhotonDetHit.hh"
#include "OpDetectorConstruction.hh"
//#include "OpUserTrackInformation.hh"

#include "G4HCofThisEvent.hh"
#include "G4OpticalPhoton.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

OpPhotonDetSD::OpPhotonDetSD(G4String name, G4String HCname, G4int OpPhotonDetID)
  : G4VSensitiveDetector(name)
  , fPhotonDetHitCollection(nullptr)
  , fHCID(-1)
  , fOpPhotonDetID(OpPhotonDetID)
{
  collectionName.insert(HCname);
}

OpPhotonDetSD::~OpPhotonDetSD() {}

void OpPhotonDetSD::Initialize(G4HCofThisEvent* HCE)
{
  fPhotonDetHitCollection =
    new OpPhotonDetHitsCollection(SensitiveDetectorName, collectionName[0]);
  if(fHCID < 0)
  {
    fHCID =
      G4SDManager::GetSDMpointer()->GetCollectionID(fPhotonDetHitCollection);
  }
  HCE->AddHitsCollection(fHCID, fPhotonDetHitCollection);
}

G4bool OpPhotonDetSD::ProcessHits(G4Step* step, G4TouchableHistory* TH)
{
	std::cout << "yjkim SD" << std::endl;
	if(step->GetTrack()->GetDefinition() !=G4OpticalPhoton::OpticalPhotonDefinition())
		return false;

	G4int OpPhotonDetID = step -> GetPostStepPoint() 
							-> GetTouchable() -> GetVolume(1) -> GetCopyNo();
	std::cout << OpPhotonDetID << std::endl;
	G4int NofHits = fPhotonDetHitCollection -> entries();
	G4double hitTime = step -> GetPostStepPoint() -> GetGlobalTime();
	G4double energy = step -> GetTrack() -> GetTotalEnergy();

	OpPhotonDetHit* hit = NULL;

	for( G4int a=0; a < NofHits; a++)
	{
		if((*fPhotonDetHitCollection)[a] -> GetOpPhotonDetID() == OpPhotonDetID)
		{
			hit = (*fPhotonDetHitCollection)[a];
			break;
		}
	}
	if(hit == NULL)
	{
		hit = new OpPhotonDetHit();
		hit -> SetOpPhotonDetID(OpPhotonDetID);
		hit -> SetOpPhotonDetPos(step->GetPostStepPoint()->GetTouchableHandle()
			-> GetHistory() -> GetTopTransform().Inverse().TransformPoint(G4ThreeVector(0.,0.,0)));
		fPhotonDetHitCollection -> insert(hit);
	}
	hit -> photonCount();
	hit -> SetWavlen(ETowav(energy));
	hit -> SetTime(hitTime);

	return true;
}

void OpPhotonDetSD::EndOfEvent(G4HCofThisEvent*)
{
	if(verboseLevel > 1)
	{
		G4int NofHits = fPhotonDetHitCollection->entries();
		G4cout<<G4endl<<
		"------->Hits Collection: in this event they are " << NofHits <<
		" hits in the tracker chambers: " << G4endl;
		for(G4int a=0; a<NofHits; a++) (*fPhotonDetHitCollection)[a] -> Print();
	}
}

//G4bool OpPhotonDetSD::ProcessHits_boundary(const G4Step* aStep,
//                                            G4TouchableHistory*)
//// Generates a hit and uses the postStepPoint; PostStepPoint because the hit
//// is generated manually when the photon hits the detector
//{
//  if(!aStep)
//    return false;
//  G4Track* theTrack = aStep->GetTrack();
//
//  // Need to know if this is an optical photon
//  if(theTrack->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
//  {
//    return false;
//  }
//
//  // Find out information regarding the hit
//  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
//
//  OpUserTrackInformation* trackInformation =
//    (OpUserTrackInformation*) theTrack->GetUserInformation();
//
//  G4TouchableHistory* theTouchable =
//    (G4TouchableHistory*) (thePostPoint->GetTouchable());
//
//  G4ThreeVector photonExit   = trackInformation->GetExitPosition();
//  G4ThreeVector photonArrive = thePostPoint->GetPosition();
//  G4double arrivalTime       = theTrack->GetGlobalTime();
//
//  // Convert the global coordinate for arriving photons into
//  // the local coordinate of the detector
//  photonArrive =
//    theTouchable->GetHistory()->GetTopTransform().TransformPoint(photonArrive);
//
//  // Creating the hit and add it to the collection
//  fPhotonDetHitCollection->insert(
//    new OpPhotonDetHit(photonExit, photonArrive, arrivalTime));
//
//  return true;
//}
