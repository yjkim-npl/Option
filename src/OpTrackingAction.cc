#include "OpTrackingAction.hh"

#include "OpTrajectory.hh"
#include "OpUserTrackInformation.hh"

#include "G4Track.hh"
#include "G4TrackingManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  // Let this be up to the user via vis.mac
  //  fpTrackingManager->SetStoreTrajectory(true);

  // Use custom trajectory class
  fpTrackingManager->SetTrajectory(new OpTrajectory(aTrack));

  OpUserTrackInformation* trackInformation = new OpUserTrackInformation();

  if(aTrack->GetMomentumDirection().z() > 0.0)
  {
    trackInformation->AddStatusFlag(right);
  }
  else
  {
    trackInformation->AddStatusFlag(left);
  }

  G4String PVName = aTrack->GetVolume()->GetName();

  if(PVName == "OpFiber" || PVName == "Clad1" || PVName == "Clad2")
    trackInformation->AddStatusFlag(InsideOfFiber);

  fpTrackingManager->SetUserTrackInformation(trackInformation);
}
