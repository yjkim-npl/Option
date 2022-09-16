#ifndef OPPRIMARYGENERATORACTION_HH
#define OPPRIMARYGENERATORACTION_HH 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "globals.hh"

class OpPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
	public:
		OpPrimaryGeneratorAction();
		virtual ~OpPrimaryGeneratorAction();

		virtual void GeneratePrimaries(G4Event*);

		const G4ParticleGun* GetParticleGun() const {return fParticleGun;}

	private:
		G4ParticleGun* fParticleGun;
};
#endif
