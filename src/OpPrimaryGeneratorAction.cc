#include "OpPrimaryGeneratorAction.hh"

#include "OpDetectorConstruction.hh"

#include "G4AutoLock.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalPhoton.hh"
#include "G4PhysicsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

namespace
{
  G4Mutex gen_mutex = G4MUTEX_INITIALIZER;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool OpPrimaryGeneratorAction::fFirst = false;

OpPrimaryGeneratorAction::OpPrimaryGeneratorAction(
  OpDetectorConstruction* dc)
{
  fDetector      = dc;
  fIntegralTable = nullptr;

  fParticleGun = new G4GeneralParticleSource();


  fTimeConstant     = 0.;
  fUseSampledEnergy = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpPrimaryGeneratorAction::~OpPrimaryGeneratorAction()
{
  delete fParticleGun;
  if(fIntegralTable)
  {
    fIntegralTable->clearAndDestroy();
    delete fIntegralTable;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpPrimaryGeneratorAction::SetDecayTimeConstant(G4double time)
{
  fTimeConstant = time;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpPrimaryGeneratorAction::BuildEmissionSpectrum()
{
  if(fIntegralTable)
    return;

  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
  G4int numOfMaterials                    = G4Material::GetNumberOfMaterials();

  if(!fIntegralTable)
    fIntegralTable = new G4PhysicsTable(numOfMaterials);

  for(G4int i = 0; i < numOfMaterials; ++i)
  {
    G4PhysicsOrderedFreeVector* vec = new G4PhysicsOrderedFreeVector();

    G4MaterialPropertiesTable* MPT =
      (*theMaterialTable)[i]->GetMaterialPropertiesTable();

    if(MPT)
    {
      G4MaterialPropertyVector* theOpVector = MPT->GetProperty("OpCOMPONENT");

      if(theOpVector)
      {
        G4double currentIN = (*theOpVector)[0];
        if(currentIN >= 0.0)
        {
          G4double currentPM  = theOpVector->Energy(0);
          G4double currentCII = 0.0;
          vec->InsertValues(currentPM, currentCII);
          G4double prevPM  = currentPM;
          G4double prevCII = currentCII;
          G4double prevIN  = currentIN;

          for(size_t j = 1; j < theOpVector->GetVectorLength(); ++j)
          {
            currentPM  = theOpVector->Energy(j);
            currentIN  = (*theOpVector)[j];
            currentCII = 0.5 * (prevIN + currentIN);
            currentCII = prevCII + (currentPM - prevPM) * currentCII;
            vec->InsertValues(currentPM, currentCII);
            prevPM  = currentPM;
            prevCII = currentCII;
            prevIN  = currentIN;
          }
        }
      }
    }
    fIntegralTable->insertAt(i, vec);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(!fFirst)
  {
    fFirst = true;
    BuildEmissionSpectrum();
  }

  if(fUseSampledEnergy)
  {
    const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

    G4double sampledEnergy = 3. * eV;

    for(size_t j = 0; j < theMaterialTable->size(); ++j)
    {
      G4Material* fMaterial = (*theMaterialTable)[j];
      if(fMaterial->GetName() == "PMMA")
      {
        auto OpIntensity =
          fMaterial->GetMaterialPropertiesTable()->GetProperty("OpCOMPONENT");

        if(OpIntensity)
        {
          G4PhysicsOrderedFreeVector* OpIntegral =
            (G4PhysicsOrderedFreeVector*) ((*fIntegralTable)(
              fMaterial->GetIndex()));

          G4double CIImax   = OpIntegral->GetMaxValue();
          G4double CIIvalue = G4UniformRand() * CIImax;

          sampledEnergy = OpIntegral->GetEnergy(CIIvalue);
        }
      }
    }

    // this does not work.
    G4String cmd = "/gun/energy " + G4String(sampledEnergy / eV) + " eV";
    G4UImanager::GetUIpointer()->ApplyCommand(cmd);
  }

  // The code behind this line is not thread safe because polarization
  // and time are randomly selected and GPS properties are global

  G4AutoLock l(&gen_mutex);
  if(fParticleGun->GetParticleDefinition() == G4OpticalPhoton::Definition())
  {
    SetOptPhotonPolar();
    SetOptPhotonTime();
  }

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpPrimaryGeneratorAction::SetOptPhotonPolar()
{
  G4double angle = G4UniformRand() * 360.0 * deg;
  SetOptPhotonPolar(angle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpPrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
{
  if(fParticleGun->GetParticleDefinition()->GetParticleName() !=
     "opticalphoton")
  {
    G4cout << "-> warning from OpPrimaryGeneratorAction::SetOptPhotonPolar()"
           << ":  the ParticleGun is not an opticalphoton" << G4endl;
    return;
  }

  G4ThreeVector normal(1., 0., 0.);
  G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
  G4ThreeVector product = normal.cross(kphoton);
  G4double modul2       = product * product;

  G4ThreeVector e_perpend(0., 0., 1.);
  if(modul2 > 0.)
    e_perpend = (1. / std::sqrt(modul2)) * product;
  G4ThreeVector e_paralle = e_perpend.cross(kphoton);

  G4ThreeVector polar =
    std::cos(angle) * e_paralle + std::sin(angle) * e_perpend;
  fParticleGun->SetParticlePolarization(polar);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpPrimaryGeneratorAction::SetOptPhotonTime()
{
  G4double time = -std::log(G4UniformRand()) * fTimeConstant;
  fParticleGun->SetParticleTime(time);
}
