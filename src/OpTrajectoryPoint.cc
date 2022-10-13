#include "OpTrajectoryPoint.hh"

#include "G4AttDef.hh"
#include "G4AttDefStore.hh"
#include "G4AttValue.hh"
#include "G4Step.hh"
#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"

G4ThreadLocal G4Allocator<OpTrajectoryPoint>* OpTrajPointAllocator = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpTrajectoryPoint::OpTrajectoryPoint()
  : fTime(0.)
  , fMomentum(0., 0., 0.)
  , fStepStatus(fUndefined)
  , fVolumeName("")
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpTrajectoryPoint::OpTrajectoryPoint(const G4Step* aStep)
  : G4TrajectoryPoint(aStep->GetPostStepPoint()->GetPosition())
{
  auto postStepPoint = aStep->GetPostStepPoint();
  fTime              = postStepPoint->GetGlobalTime();
  fMomentum          = postStepPoint->GetMomentum();
  fStepStatus        = postStepPoint->GetStepStatus();
  if(postStepPoint->GetPhysicalVolume())
  {
    fVolumeName = postStepPoint->GetPhysicalVolume()->GetName();
  }
  else
  {
    fVolumeName = " ";
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpTrajectoryPoint::OpTrajectoryPoint(const G4Track* aTrack)
  : G4TrajectoryPoint(aTrack->GetPosition())
{
  fTime       = aTrack->GetGlobalTime();
  fMomentum   = aTrack->GetMomentum();
  fStepStatus = fUndefined;
  fVolumeName = aTrack->GetVolume()->GetName();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpTrajectoryPoint::OpTrajectoryPoint(const OpTrajectoryPoint& right)
  : G4TrajectoryPoint(right)
{
  fTime       = right.fTime;
  fMomentum   = right.fMomentum;
  fStepStatus = right.fStepStatus;
  fVolumeName = right.fVolumeName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpTrajectoryPoint::~OpTrajectoryPoint() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String, G4AttDef>* OpTrajectoryPoint::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String, G4AttDef>* store =
    G4AttDefStore::GetInstance("TrajectoryPoint", isNew);
  if(isNew)
  {
    G4String Pos("Pos");
    (*store)[Pos] =
      G4AttDef(Pos, "Position", "Physics", "G4BestUnit", "G4ThreeVector");

    G4String Time("Time");
    (*store)[Time] =
      G4AttDef(Time, "Time", "Physics", "G4BestUnit", "G4double");

    G4String Momentum("Momentum");
    (*store)[Momentum] =
      G4AttDef(Momentum, "Momentum", "Physics", "G4BestUnit", "G4ThreeVector");

    G4String StepStatus("StepStatus");
    (*store)[StepStatus] =
      G4AttDef(StepStatus, "StepStatus", "Physics", "", "G4StepStatus");

    G4String VolumeName("VolumeName");
    (*store)[VolumeName] =
      G4AttDef(VolumeName, "VolumeName", "Physics", "", "G4String");
  }
  return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* OpTrajectoryPoint::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("Time", G4BestUnit(fTime, "Time"), ""));
  values->push_back(
    G4AttValue("Momentum", G4BestUnit(fMomentum, "Momentum"), ""));
  values->push_back(G4AttValue("StepStatus", fStepStatus, ""));
  values->push_back(G4AttValue("VolumeName", fVolumeName, ""));

  return values;
}
