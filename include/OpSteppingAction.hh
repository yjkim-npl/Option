#ifndef OpSteppingAction_h
#define OpSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class OpEventAction;

class G4LogicalVolume;
class G4OpBoundaryProcess;

/// Stepping action class
/// 

class OpSteppingAction : public G4UserSteppingAction
{
  public:
    OpSteppingAction(OpEventAction* eventAction);
    virtual ~OpSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    OpEventAction*  fEventAction;
    G4LogicalVolume* fScoringVolume;
	G4OpBoundaryProcess* fOpProcess;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
