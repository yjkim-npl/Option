#ifndef OpTrajectoryPoint_h
#define OpTrajectoryPoint_h 1

#include "globals.hh"
#include "G4Allocator.hh"
#include "G4StepStatus.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryPoint.hh"

class G4Track;
class G4Step;
class G4VProcess;

class OpTrajectoryPoint : public G4TrajectoryPoint
{
 public:
  OpTrajectoryPoint();
  OpTrajectoryPoint(const G4Track*);
  OpTrajectoryPoint(const G4Step*);
  OpTrajectoryPoint(const OpTrajectoryPoint& right);
  ~OpTrajectoryPoint();

  inline void* operator new(size_t);
  inline void operator delete(void* aTrajectoryPoint);
  inline G4bool operator==(const OpTrajectoryPoint& right) const
  {
    return (this == &right);
  };

  inline G4double GetTime() const { return fTime; };
  inline const G4ThreeVector GetMomentum() const { return fMomentum; };
  inline G4StepStatus GetStepStatus() const { return fStepStatus; };
  inline G4String GetVolumeName() const { return fVolumeName; };

  virtual const std::map<G4String, G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;

 private:
  G4double fTime;
  G4ThreeVector fMomentum;
  G4StepStatus fStepStatus;
  G4String fVolumeName;
};

extern G4ThreadLocal G4Allocator<OpTrajectoryPoint>* OpTrajPointAllocator;

inline void* OpTrajectoryPoint::operator new(size_t)
{
  if(!OpTrajPointAllocator)
    OpTrajPointAllocator = new G4Allocator<OpTrajectoryPoint>;
  return (void*) OpTrajPointAllocator->MallocSingle();
}

inline void OpTrajectoryPoint::operator delete(void* aTrajectoryPoint)
{
  OpTrajPointAllocator->FreeSingle((OpTrajectoryPoint*) aTrajectoryPoint);
}

#endif
