#ifndef OpActionInitialization_h
#define OpActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "OpParameterContainer.hh"

/// Action initialization class.

class OpActionInitialization : public G4VUserActionInitialization
{
  public:
    OpActionInitialization(OpParameterContainer* parC);
    virtual ~OpActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
  private:
	OpParameterContainer* parameterContainer;
//	ifstream parameter;
};
#endif
