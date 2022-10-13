#include "OpStackingAction.hh"

#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpStackingAction::OpStackingAction()
  : fPhotonCounter(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpStackingAction::~OpStackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack OpStackingAction::ClassifyNewTrack(
  const G4Track* aTrack)
{
  G4ParticleDefinition* particleType = aTrack->GetDefinition();

  // keep primary particle
  if(aTrack->GetParentID() == 0)
    return fUrgent;

  if(particleType == G4OpticalPhoton::OpticalPhotonDefinition())
  {
    // keep optical photon
    ++fPhotonCounter;
    return fUrgent;
  }
  else
  {
    // discard all other secondaries
    // return fKill;
  }
  return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpStackingAction::NewStage()
{
  // G4cout << "Number of optical photons produces in this event : "
  //        << fPhotonCounter << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpStackingAction::PrepareNewEvent() { fPhotonCounter = 0; }
