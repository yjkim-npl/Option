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

    G4VPhysicalVolume* Construct() override;
//	void ConstructSDandField() override;
	int NofY;

  private:
	void DefineMaterials();

	G4double fWorldSizeX;
	G4double fWorldSizeY;
	G4double fWorldSizeZ;

	G4double fEnvX;
	G4double fEnvY;
	G4double fEnvZ;

	G4double fSCX;
	G4double fSCY;
	G4double fSCZ;

	G4double fMPPCX;
	G4double fGlassX;
	G4double fMPPCY;
	G4double fMPPCZ;

	G4Material* fVac;
	G4Material* fAir;
	G4Material* fPS;
	G4Material* fSi;
	G4Material* fFP;
	G4Material* fGlass;

	G4OpticalSurface* SkinSurfSi;
	G4OpticalSurface* BorderSurfAir;
	G4OpticalSurface* BorderSurfSi;

	std::vector<G4LogicalVolume*> vec_logicMPPCL;
	std::vector<G4LogicalVolume*> vec_logicMPPCR;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#endif

