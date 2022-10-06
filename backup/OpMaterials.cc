#include "OpMaterials.hh"
#include "G4SystemOfUnits.hh"

#include <stdio.h>
#include <algorithm>
#include <cmath>

OpMaterials* OpMaterials::fInstance = 0;

OpMaterials::OpMaterials()
{
	fNistMan = G4NistManager::Instance();
	CreateMaterials();
}

OpMaterials::~OpMaterials() {}

OpMaterials* OpMaterials::GetInstance()
{
	if(fInstance ==0) fInstance = new OpMaterials();
	return fInstance;
}

G4Material* OpMaterials::GetMaterial(const G4String matName)
{
	G4Material* mat = fNistMan -> FindOrBuildMaterial(matName);

	if(!mat) mat = G4Material::GetMaterial(matName);
	if(!mat)
	{
		std::ostringstream o;
		o << "Material " << matName << " not found!";
		G4Exception("OpMaterials::GetMaterial","",FatalException,o.str().c_str());
	}
	return mat;
}

G4OpticalSurface* OpMaterials::GetOpticalSurface(const G4String surfName)
{
	if(surfName == "SiPMSurf") return fSiPMSurf;
	else{
		std::ostringstream o;
		o << "OptialcSurface " << surfName << " not found!";
		G4Exception("OpMaterials::GetOpticalSurface","",FatalException,o.str().c_str());
	}
	return nullptr;
}

void OpMaterials::CreateMaterials()
{
	fNistMan -> FindOrBuildMaterial("G4_Galactic");	// vac
	fNistMan -> FindOrBuildMaterial("G4_AIR");		// air
	fVac = G4Material::GetMaterial("G4_Galactic");
	fAir = G4Material::GetMaterial("G4_AIR");

	G4String symbol;
	G4double a,z,density;
	G4int ncomp, natom;

	G4Element* H = new G4Element("Hydrogen"	,symbol="H", z=1.,	a=1.01*g/mole);
	G4Element* C = new G4Element("Carbon"	,symbol="C", z=6.,	a=12.01*g/mole);
	G4Element* N = new G4Element("Nitrogen"	,symbol="N", z=7.,	a=14.01*g/mole);
	G4Element* O = new G4Element("Oxygen"	,symbol="O", z=8.,	a=16.00*g/mole);
	G4Element* F = new G4Element("Fluorine"	,symbol="F", z=9.,	a=18.9984*g/mole);

	fCu = new G4Material("Copper"	, z=29., a=63.546*g/mole, density=8.96*g/cm3);
	fSi = new G4Material("Silicon"	, z=14., a=28.09*g/mole , density=2.33*g/cm3);
	
	fFluoPoly = new G4Material("FluorinatedPolymer",density=1.43*g/cm3,ncomp=2);
	fFluoPoly->AddElement(C,2);
	fFluoPoly->AddElement(F,2);

	fPS = new G4Material("Polystyrene",density=1.05*g/cm3,ncomp=2);
	fPS->AddElement(C,8);
	fPS->AddElement(H,8);

	fPMMA = new G4Material("PMMA",density=1.19*g/cm3,ncomp=3);
	fPMMA->AddElement(C,5);
	fPMMA->AddElement(H,8);
	fPMMA->AddElement(O,2);

	fAcryl = new G4Material("Acrylic",density=1.19*g/cm3,ncomp=3);
	fAcryl->AddElement(C,5);
	fAcryl->AddElement(H,8);
	fAcryl->AddElement(O,2);

	G4MaterialPropertiesTable* mpAir;
	G4MaterialPropertiesTable* mpFluoPoly;
	G4MaterialPropertiesTable* mpPS;
	G4MaterialPropertiesTable* mpSi;

	G4double opEn[] = // from 900nm to 300nm with 25nm step
	{
		1.37760*eV, 1.41696*eV, 1.45864*eV, 1.50284*eV, 1.54980*eV, 1.59980*eV,
		1.65312*eV, 1.71013*eV, 1.77120*eV, 1.83680*eV, 1.90745*eV, 1.98375*eV,
		2.06640*eV, 2.15625*eV, 2.25426*eV, 2.36160*eV, 2.47968*eV, 2.61019*eV,
		2.75520*eV, 2.91728*eV, 3.09960*eV, 3.30625*eV, 3.54241*eV, 3.81490*eV,
		4.13281*eV
	};
	const G4int nEn = sizeof(opEn)/sizeof(opEn[0]);

	G4double RI_Air[nEn]; std::fill_n(RI_Air,nEn,1.0);
	mpAir = new G4MaterialPropertiesTable();
	mpAir ->AddProperty("RINDEX",opEn,RI_Air,nEn);
//	fAir -> SetMaterialPropertiesTable(mpAir);

	G4double RI_FluoPoly[nEn]; std::fill_n(RI_FluoPoly,nEn,1.42);
	mpFluoPoly = new G4MaterialPropertiesTable();
	mpFluoPoly ->AddProperty("RINDEX",opEn,RI_FluoPoly,nEn);
	fFluoPoly -> SetMaterialPropertiesTable(mpFluoPoly);

	G4double RI_PS[nEn] = 
	{
		1.57483, 1.57568, 1.57644, 1.57726, 1.57817, 1.57916,
		1.58026, 1.58148, 1.58284, 1.58435, 1.58605, 1.58796,
		1.59013, 1.59328, 1.59621, 1.59960, 1.60251, 1.60824, 
		1.61229, 1.62032, 1.62858, 1.63886, 1.65191, 1.66888, 
		1.69165
	};
	G4double AbsLen_PS[nEn] = 
	{
		2.714*m, 3.102*m, 3.619*m, 4.343*m, 5.791*m, 7.896*m, 4.343*m, 7.896*m,
		5.429*m, 36.19*m, 17.37*m, 36.19*m, 5.429*m, 28.95*m, 21.71*m, 14.48*m,
		12.41*m, 8.686*m, 7.238*m, 1.200*m, 0.200*m, 0.500*m, 0.200*m, 0.100*m, 0.100*m
	};
	G4double scintFast_PS[nEn]=
	{
		0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
		0.00, 0.00, 0.00, 0.00, 0.00, 0.03, 0.07, 0.13,
		0.33, 0.63, 1.00, 0.50, 0.00, 0.00, 0.00, 0.00, 0.00
	};

	mpPS = new G4MaterialPropertiesTable();
	mpPS ->AddProperty("RINDEX",opEn,RI_PS,nEn);
	mpPS ->AddProperty("ABSLENGTH",opEn,AbsLen_PS,nEn);
	mpPS ->AddProperty("FASTCOMPONENT",opEn,scintFast_PS,nEn);
	mpPS ->AddConstProperty("SCINTILLATIONYIELD",10./keV);
	mpPS ->AddConstProperty("RESOLUIONSCALE",1.0);
	mpPS ->AddConstProperty("FASTTIMECONSTANT",2.8*ns);
//	fPS->SetMaterialPropertiesTable(mpPS);
//	fPS->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

	G4double refl_Si[nEn]; std::fill_n(refl_Si,nEn,0.);
	G4double eff_Si[nEn] = 
	{
		0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.10,
		0.11, 0.13, 0.15, 0.17, 0.19, 0.20, 0.22, 0.23,
		0.24, 0.25, 0.24, 0.23, 0.21, 0.20, 0.17, 0.14, 0.10
	};
	mpSi = new G4MaterialPropertiesTable();
	mpSi ->AddProperty("REFLECTIVITY",opEn,refl_Si,nEn);
	mpSi ->AddProperty("EFFICIENCY",opEn,eff_Si,nEn);

	fSiPMSurf = new G4OpticalSurface("SiPMSurf",glisur,polished,dielectric_metal);
	fSiPMSurf ->SetMaterialPropertiesTable(mpSi);
}
