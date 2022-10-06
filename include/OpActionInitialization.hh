#ifndef OpActionInitialization_h
#define OpActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class OpActionInitialization : public G4VUserActionInitialization
{
  public:
    OpActionInitialization();
    virtual ~OpActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
