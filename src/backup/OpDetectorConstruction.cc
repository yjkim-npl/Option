#include "OpDetectorConstruction.hh"

#include "G4Colour.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

OpDetectorConstruction::OpDetectorConstruction()
: G4VUserDetectorConstruction()
{
	DefineMaterials();
}

OpDetectorConstruction::~OpDetectorConstruction()
{
}

G4VPhysicalVolume* OpDetectorConstruction::Construct()
{

	G4bool checkOverlaps = true;

	// World
	G4double world_XY = 1000*mm;
	G4double world_Z  = 1000*mm;

	G4Box* solidWorld = new G4Box("World",0.5*world_XY, 0.5*world_XY, 0.5*world_Z);
    
	G4LogicalVolume* logicWorld =                         
		new G4LogicalVolume(solidWorld,fAir,"World");
                                 
	G4VPhysicalVolume* physWorld = 
		new G4PVPlacement(0,G4ThreeVector(),logicWorld,"World", 0, false, 0,checkOverlaps);  
    
	// Env
	G4double env_XY = 500*mm;
	G4double env_Z  = 500*mm;

	G4Box* solidEnV = new G4Box("EnV",0.5*env_XY, 0.5*env_XY, 0.5*env_Z);
    
	G4LogicalVolume* logicEnV =                         
		new G4LogicalVolume(solidEnV,fAir,"EnV");
                                 
	G4VPhysicalVolume* physEnV = 
		new G4PVPlacement(0,G4ThreeVector(),logicEnV,"EnV", logicWorld, false, 0,checkOverlaps);  
    
	// Scintillator
	G4double SC_XY = 210*mm;
//	G4double SC_Z = 0.2*mm;
	G4double SC_Z = 100*mm;

	G4Box* solidSC = new G4Box("SC",0.5*SC_XY,0.5*SC_XY,0.5*SC_Z);
	G4LogicalVolume* logicSC = 
		new G4LogicalVolume(solidSC,fFP,"SC");

	G4VisAttributes* fVis_SC = new G4VisAttributes(G4Colour(G4Colour::Brown()));
	fVis_SC -> SetVisibility(true);
	fVis_SC -> SetForceWireframe(true);
	logicSC -> SetVisAttributes(fVis_SC);

	new G4PVPlacement(0,G4ThreeVector(),logicSC,"SC",logicWorld,false,0,checkOverlaps);

	return physWorld;
}

void OpDetectorConstruction::DefineMaterials()
{
	// Materials
	G4NistManager* nist = G4NistManager::Instance();
	nist -> FindOrBuildMaterial("G4_Galactic");	// vac
	nist -> FindOrBuildMaterial("G4_AIR");		// air

	// Vaccum
	fVac = G4Material::GetMaterial("G4_Galactic");
	// Air
	fAir = G4Material::GetMaterial("G4_AIR");

	G4Element* H = new G4Element("Hydrogen"	,"H",1.,1.01*g/mole);
	G4Element* C = new G4Element("Carbon"	,"C",6.,12.01*g/mole);
	G4Element* N = new G4Element("Nitrogen"	,"N",7.,14.01*g/mole);
	G4Element* O = new G4Element("Oxygen"	,"O",8.,16.00*g/mole);
	G4Element* F = new G4Element("Fluorine"	,"F",9.,18.9984*g/mole);

	// Polystyrene
	fPS = new G4Material("Polystyrene",1.05*g/cm3,2);
	fPS -> AddElement(C,8);
	fPS -> AddElement(H,8);

	// Silicon
	fSi = new G4Material("Silicon",14.,28.09*g/mole,2.33*g/cm3);

	// Fluorinated polymer
	fFP = new G4Material("FluorinatedPolymer",1.43*g/cm3,2);
	fFP -> AddElement(C,2);
	fFP -> AddElement(F,2);

	// Material Table :: Apply Optical properties
	G4MaterialPropertiesTable* mpAir;
	G4MaterialPropertiesTable* mpPS;
	G4MaterialPropertiesTable* mpSi;
	G4MaterialPropertiesTable* mpFP;
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
	fAir -> SetMaterialPropertiesTable(mpAir);

	G4double RI_FP[nEn]; std::fill_n(RI_FP,nEn, 1.42);
	mpFP = new G4MaterialPropertiesTable();
	mpFP -> AddProperty("RINDEX",opEn,RI_FP,nEn);
	fFP -> SetMaterialPropertiesTable(mpFP);

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
	fPS->SetMaterialPropertiesTable(mpPS);
	fPS->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

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
