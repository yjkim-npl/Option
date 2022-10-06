#ifndef OpDetectorConstruction_h
#define OpDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class OpDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    OpDetectorConstruction();
    virtual ~OpDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
  private:
	void DefineMaterials();

	G4Material* fVac;
	G4Material* fAir;
	G4Material* fPS;
	G4Material* fSi;
	G4Material* fFP;

	G4OpticalSurface* fSiPMSurf;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

