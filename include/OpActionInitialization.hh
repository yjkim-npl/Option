#ifndef OpActionInitialization_h
#define OpActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
//#include <iostream>
//#include <fstream>

/// Action initialization class.

class OpActionInitialization : public G4VUserActionInitialization
{
  public:
    OpActionInitialization();//char* parName);
    virtual ~OpActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
  private:
//	ifstream parameter;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
