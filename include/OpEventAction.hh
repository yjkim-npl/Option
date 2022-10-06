#ifndef OpEventAction_h
#define OpEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class OpRunAction;

/// Event action class
///

class OpEventAction : public G4UserEventAction
{
  public:
    OpEventAction(OpRunAction* runAction);
    virtual ~OpEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddEdep(G4double edep) { fEdep += edep; }

  private:
    OpRunAction* fRunAction;
    G4double     fEdep;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
