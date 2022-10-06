#ifndef OpMaterials_h
#define OpMaterials_h 1

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"

class OpMaterials
{
	public:
		virtual ~OpMaterials();
		static OpMaterials* GetInstance();
		G4Material* GetMaterial(const G4String);
		G4OpticalSurface* GetOpticalSurface(const G4String);

	private:
		void CreateMaterials();

		OpMaterials();
		static OpMaterials* fInstance;
		G4NistManager* fNistMan;
		G4Material* fVac;
		G4Material* fAir;
		G4Material* fFluoPoly;
		G4Material* fPMMA;
		G4Material* fPS;
		G4Material* fCu;
		G4Material* fAcryl;
		G4Material* fSi;
		G4OpticalSurface* fSiPMSurf;
};
#endif
