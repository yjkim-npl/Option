#ifndef OpPrimaryGeneratorAction_h
#define OpPrimaryGeneratorAction_h 1

#include "OpParameterContainer.hh"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

class OpPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    OpPrimaryGeneratorAction(OpParameterContainer* parC);    
    virtual ~OpPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
  private:
	OpParameterContainer* OpPar;
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
};

#endif
