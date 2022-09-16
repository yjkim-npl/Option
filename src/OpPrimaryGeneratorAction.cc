#include "OpPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

OpPrimaryGeneratorAction::OpPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction()
{
	G4int n_particle = 1;
	fParticleGun = new G4ParticleGun(n_particle);
	G4ParticleTable* table = G4ParticleTable::GetParticleTable();
	G4String name;
	G4ParticleDefinition* d_particle = table -> FindParticle(name = "proton");
	fParticleGun -> SetParticleDefinition(d_particle);
	fParticleGun -> SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
	fParticleGun -> SetParticleEnergy(40.*MeV);
}

OpPrimaryGeneratorAction::~OpPrimaryGeneratorAction()
{
	delete fParticleGun;
}

void OpPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	fParticleGun -> GeneratePrimaryVertex(anEvent);
}
