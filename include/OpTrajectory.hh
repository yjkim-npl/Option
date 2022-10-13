#ifndef OpTrajectory_h
#define OpTrajectory_h 1

#include "globals.hh"
#include "G4Allocator.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4TrajectoryPoint.hh"
#include "G4VTrajectory.hh"

#include <vector>

typedef std::vector<G4VTrajectoryPoint*> OpTrajectoryPointContainer;

class OpTrajectory : public G4VTrajectory
{
 public:
  OpTrajectory();
  OpTrajectory(const G4Track*);
  OpTrajectory(OpTrajectory&);
  ~OpTrajectory();

  inline void* operator new(size_t);
  inline void operator delete(void*);
  inline int operator==(const OpTrajectory& right) const
  {
    return (this == &right);
  }

  inline virtual G4int GetTrackID() const { return fTrackID; }
  inline virtual G4int GetParentID() const { return fParentID; }
  inline virtual G4String GetParticleName() const { return fParticleName; }
  inline virtual G4double GetCharge() const { return fPDGCharge; }
  inline virtual G4int GetPDGEncoding() const { return fPDGEncoding; }
  inline virtual G4ThreeVector GetInitialMomentum() const
  {
    return fInitialMomentum;
  }

  virtual void ShowTrajectory(std::ostream& os = G4cout) const;
  virtual void AppendStep(const G4Step* aStep);
  virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);

  G4ParticleDefinition* GetParticleDefinition();

  virtual int GetPointEntries() const { return fpPointsContainer->size(); }
  virtual G4VTrajectoryPoint* GetPoint(G4int i) const
  {
    return (*fpPointsContainer)[i];
  }

  virtual const std::map<G4String, G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;

 private:
  OpTrajectoryPointContainer* fpPointsContainer;

  G4int fTrackID;
  G4int fParentID;
  G4double fPDGCharge;
  G4int fPDGEncoding;
  G4String fParticleName;
  G4ThreeVector fInitialMomentum;

  G4ParticleDefinition* fParticleDefinition;
};

extern G4ThreadLocal G4Allocator<OpTrajectory>* OpTrajectoryAllocator;

inline void* OpTrajectory::operator new(size_t)
{
  if(!OpTrajectoryAllocator)
    OpTrajectoryAllocator = new G4Allocator<OpTrajectory>;
  return (void*) OpTrajectoryAllocator->MallocSingle();
}

inline void OpTrajectory::operator delete(void* aTrajectory)
{
  OpTrajectoryAllocator->FreeSingle((OpTrajectory*) aTrajectory);
}

#endif
