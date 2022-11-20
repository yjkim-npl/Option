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

#include <random>

OpPrimaryGeneratorAction::OpPrimaryGeneratorAction(OpParameterContainer* parC)
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0) 
{
	OpPar = parC;
	G4int n_particle = OpPar->GetParInt("NperEvent");
	fParticleGun  = new G4ParticleGun(n_particle);

	// default particle kinematic
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	G4ParticleDefinition* particle
		= particleTable->FindParticle(particleName=OpPar->GetParString("particle"));
//    = particleTable->FindParticle(particleName="mu-");
	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
	fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.*mm));
//	fParticleGun->SetParticleEnergy(10.*GeV);
	fParticleGun->SetParticleEnergy(OpPar->GetParDouble("energy")*GeV);
}

OpPrimaryGeneratorAction::~OpPrimaryGeneratorAction()
{
	delete fParticleGun;
}

void OpPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	// random num
	std::random_device RD;
	std::mt19937_64 RDGen(RD());
	G4double x0 = OpPar->GetParDouble("x0")*mm;
	G4double y0 = OpPar->GetParDouble("y0")*mm;
	G4double z0 = OpPar->GetParDouble("z0")*mm;

	G4double rx = OpPar->GetParDouble("rx")*mm;
	G4double ry = OpPar->GetParDouble("ry")*mm;

	std::uniform_real_distribution<> RDdistX(-rx,rx);
	std::uniform_real_distribution<> RDdistY(-ry,ry);
	rx = RDdistX(RDGen);
	ry = RDdistY(RDGen);

	x0+=rx;
	y0+=ry;

	fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

	fParticleGun->GeneratePrimaryVertex(anEvent);
}
