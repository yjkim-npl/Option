#include "OpMaterials.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

#include <algorithm>
#include <cmath>

OpMaterials* OpMaterials::fInstance = nullptr;

OpMaterials::OpMaterials()
{
  fNistMan = G4NistManager::Instance();
  fNistMan->SetVerbose(2);

  CreateMaterials();
}

OpMaterials::~OpMaterials()
{
//  delete fPMMA;
//  delete fPethylene;
//  delete fFPethylene;
  delete fPS;
  delete fSi;
  delete fAir;
  delete fVac;
}

OpMaterials* OpMaterials::GetInstance()
{
  if(!fInstance)
  {
    fInstance = new OpMaterials();
  }
  return fInstance;
}

G4Material* OpMaterials::GetMaterial(const G4String material)
{
  G4Material* mat = fNistMan->FindOrBuildMaterial(material);

  if(!mat)
    mat = G4Material::GetMaterial(material);
  if(!mat)
  {
    G4ExceptionDescription ed;
    ed << "Material " << material << " not found!";
    G4Exception("OpMaterials::GetMaterial", "", FatalException, ed);
  }

  return mat;
}

G4OpticalSurface* OpMaterials::GetOpticalSurface(const G4String surfName)
{
	if(surfName=="SiSurf") return surfSi;
	else
	{
    	G4ExceptionDescription ed;
	    ed << "Surf " << surfName << " not found!";
    	G4Exception("OpMaterials::GetOpticalSurfacel", "", FatalException, ed);
	}
	return nullptr;
}
void OpMaterials::CreateMaterials()
{
	// Materials Definitions
	// =====================
	// Elements
	G4Element* H = new G4Element("Hydrogen","H",1,1.01*g/mole);
	G4String symbol;
	G4double a, z, density;
	G4Element* H  = new G4Element("Hydrogen",symbol="H" , z=1., a=1.01*g/mole);
	G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z=6., a=12.01*g/mole);
	G4Element* N  = new G4Element("Nitrogen",symbol="N" , z=7., a=14.01*g/mole);
	G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z=8., a=16.00*g/mole);
	G4Element* F  = new G4Element("Fluorine",symbol="F" , z=9., a=18.9984*g/mole);

	fCu = new G4Material("Copper"  , z = 29., a = 63.546 * g/mole, density = 8.96  * g/cm3);
	fW  = new G4Material("Tungsten", z = 74., a = 183.84 * g/mole, density = 19.30 * g/cm3);
	fFe = new G4Material("Iron"    , z = 26., a = 55.845 * g/mole, density = 7.874 * g/cm3);
	fPb = new G4Material("Lead"    , z = 82., a = 207.2  * g/mole, density = 11.35 * g/cm3);

	fSi = new G4Material("Silicon", z=14., a=28.09*g/mole, density=2.33*g/cm3);
	fAl = new G4Material("Aluminum", z=13., a=26.98*g/mole, density=2.699*g/cm3);
	//--------------------------------------------------
	// Vacuum
	//--------------------------------------------------

	fVac = fNistMan->FindOrBuildMaterial("G4_Galactic");

	//--------------------------------------------------
	// Air
	//--------------------------------------------------

	fAir = fNistMan->FindOrBuildMaterial("G4_AIR");

	//--------------------------------------------------
	// Opfiber PMMA
	//--------------------------------------------------

	//--------------------------------------------------
	// Cladding (polyethylene)
	//--------------------------------------------------

	//--------------------------------------------------
	// Double Cladding (fluorinated polyethylene)
	//--------------------------------------------------

	//--------------------------------------------------
	// Polystyrene
	//--------------------------------------------------
	fPS = new G4Material("Polystyrene", 1.032*g/cm3, 2);
	fPS -> AddElement(C,2);
	fPS -> AddElement(F,2);

	//--------------------------------------------------
	// Silicone (Template for Optical Grease)
	//--------------------------------------------------
	fSi = new G4Material("Silicon", 14, 28.09*g/mole, 2.33*g/cm3);

	//--------------------------------------------------
	// Scintillator Coating - 15% TiO2 and 85% polystyrene by weight.
	//--------------------------------------------------

	//
	// ------------ Generate & Add Material Properties Table ------------
	G4double opEn[] = { // from 900nm to 300nm with 25nm step
		1.37760*eV, 1.41696*eV, 1.45864*eV, 1.50284*eV, 1.54980*eV, 1.59980*eV,
		1.65312*eV, 1.71013*eV,	1.77120*eV, 1.83680*eV, 1.90745*eV, 1.98375*eV,
		2.06640*eV, 2.15625*eV, 2.25426*eV, 2.36160*eV,	2.47968*eV, 2.61019*eV,
		2.75520*eV, 2.91728*eV, 3.09960*eV, 3.30625*eV, 3.54241*eV, 3.81490*eV, 4.13281*eV
	};
	const G4int nEnt = sizeof(opEn) / sizeof(opEn[0]);

//  std::vector<G4double> energySmall = { 2.0 * eV, 3.47 * eV };

	//--------------------------------------------------
	// Air
	//--------------------------------------------------
	G4MaterialPropertiesTable* mpAir = new G4MaterialPropertiesTable();
	G4double RI_Air[nEnt];	std::fill_n(RI_Air,nEnt,1.0);
	mpAir -> AddProperty("RINDEX",opEn,RI_Air,nEnt);
	fAir -> SetMaterialPropertiesTable(mpAir);

	//--------------------------------------------------
	//  PMMA for Opfibers
	//--------------------------------------------------

	//--------------------------------------------------
	//  Polyethylene
	//--------------------------------------------------

	//--------------------------------------------------
	// Fluorinated Polyethylene
	//--------------------------------------------------


	//--------------------------------------------------
	// Silicone
	//--------------------------------------------------

//	std::vector<G4double> refractiveIndexSilicone = { 1.46, 1.46 };
//
//	// Add entries into properties table
//	G4MaterialPropertiesTable* mptSilicone = new G4MaterialPropertiesTable();
//	mptSilicone->AddProperty("RINDEX", energySmall, refractiveIndexSilicone);
//	mptSilicone->AddProperty("ABSLENGTH", energySmall, absClad);
//
//	fSilicone->SetMaterialPropertiesTable(mptSilicone);

	//--------------------------------------------------
	//  Polystyrene
	//--------------------------------------------------
	G4double RI_PS[nEnt] = {
		1.57483, 1.57568, 1.57644, 1.57726, 1.57817, 1.57916, 1.58026, 1.58148,
		1.58284, 1.58435, 1.58605, 1.58796, 1.59013, 1.59328, 1.59621, 1.59960,
		1.60251, 1.60824, 1.61229, 1.62032, 1.62858, 1.63886, 1.65191, 1.66888, 1.69165
	};
	G4double AbsLen_PS[nEnt] = {
		2.714*m, 3.102*m, 3.619*m, 4.343*m, 5.791*m, 7.896*m, 4.343*m, 7.896*m,
		5.429*m, 36.19*m, 17.37*m, 36.19*m, 5.429*m, 28.95*m, 21.71*m, 14.48*m,
		12.41*m, 8.686*m, 7.238*m, 1.200*m, 0.200*m, 0.500*m, 0.200*m, 0.100*m, 0.100*m
	};
	G4double scintFast_PS[nEnt] = {
		0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
		0.00, 0.00, 0.00, 0.00, 0.00, 0.03, 0.07, 0.13,
		0.33, 0.63, 1.00, 0.50, 0.00, 0.00, 0.00, 0.00, 0.00
	};
	G4MaterialPropertiesTable* mpPS = new G4MaterialPropertiesTable();
	mpPS->AddProperty("RINDEX",opEn,RI_PS,nEnt);
	mpPS->AddProperty("ABSLENGTH",opEn,AbsLen_PS,nEnt);
	mpPS->AddProperty("FASTCOMPONENT",opEn,scintFast_PS,nEnt);
	mpPS->AddConstProperty("SCINTILLATIONYIELD",10./keV);
	mpPS->AddConstProperty("RESOLUTIONSCALE",1.0);
	mpPS->AddConstProperty("FASTTIMECONSTANT",2.8*ns);
	fPS->SetMaterialPropertiesTable(mpPS);
	fPS->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

	//--------------------------------------------------
	//  OpticalSurface - Si
	//--------------------------------------------------
	G4double refl_Si[nEnt]; std::fill_n(refl_Si,nEnt,0);
	G4double eff_Si[nEnt] = 
	{
		0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.10,
		0.11, 0.13, 0.15, 0.17, 0.19, 0.20, 0.22, 0.23,
		0.24, 0.25, 0.24, 0.23, 0.21, 0.20, 0.17, 0.14, 0.10
	};
	G4MaterialPropertiesTable* mpSi = new G4MaterialPropertiesTable();
	mpSi -> AddProperty("REFLECTIVITY",opEn,refl_Si,nEnt);
	mpSi -> AddProperty("EFFICIENCY",opEn,eff_Si,nEnt);
	SkinSurfSi = new G4OpticalSurface("SkinSurfSi",glisur,polished,dielectric_metal);
	SkinSurfSi -> SetMaterialPropertiesTable(mpSi);
	//--------------------------------------------------
	//  OpticalSurface(PS to Si) 
	//--------------------------------------------------
	G4double eff_surf_Si[nEnt]; std::fill_n(eff_surf_Si,nEnt,1.0);
	G4double ref_surf_Si[nEnt]; std::fill_n(ref_surf_Si,nEnt,0.0); 
	G4MaterialPropertiesTable* mpSurf_Si = new G4MaterialPropertiesTable();
	mpSurf_Si -> AddProperty("TRANSMITTANCE",opEn,eff_surf_Si,nEnt);
	mpSurf_Si -> AddProperty("REFLECTIVITY",opEn,ref_surf_Si,nEnt);
	BorderSurfSi = new G4OpticalSurface("BorderSurfSi",glisur,polished,dielectric_dielectric);
	BorderSurfSi -> SetMaterialPropertiesTable(mpSurf_Si);
	//--------------------------------------------------
	//  OpticalSurface(PS to Air) 
	//--------------------------------------------------
	G4double eff_surf_Air[nEnt]; std::fill_n(eff_surf_Air,nEnt,0.0);
	G4double ref_surf_Air[nEnt]; std::fill_n(ref_surf_Air,nEnt,1.0); 
	G4MaterialPropertiesTable* mpSurf_Air = new G4MaterialPropertiesTable();
	mpSurf_Air -> AddProperty("TRANSMITTANCE",opEn,eff_surf_Air,nEnt);
	mpSurf_Air -> AddProperty("REFLECTIVITY",opEn,ref_surf_Air,nEnt);
	BordersurfAir = new G4Opticalsurf_Airace("BorderSurfAir",glisur,polished,dielectric_metal);
	BordersurfAir -> SetMaterialPropertiesTable(mpsurf_Air);
}
