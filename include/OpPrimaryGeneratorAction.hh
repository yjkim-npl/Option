#ifndef OpPrimaryGeneratorAction_h
#define OpPrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class OpDetectorConstruction;

class G4Event;
class G4GeneralParticleSource;
class G4PhysicsTable;

class OpPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
 public:
  OpPrimaryGeneratorAction(OpDetectorConstruction*);
  ~OpPrimaryGeneratorAction();

  void GeneratePrimaries(G4Event*) override;

  void BuildEmissionSpectrum();
  void SetOptPhotonPolar(G4double);
  void SetDecayTimeConstant(G4double);

  void SetUseSampledEnergy(G4bool v) { fUseSampledEnergy = v; }

 protected:
  G4PhysicsTable* fIntegralTable;

 private:
  void SetOptPhotonPolar();
  void SetOptPhotonTime();

  OpDetectorConstruction* fDetector;
  G4GeneralParticleSource* fParticleGun;

  static G4bool fFirst;
  G4double fTimeConstant;
  G4bool fUseSampledEnergy;
};

#endif
