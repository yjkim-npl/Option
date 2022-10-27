#include "OpDetectorConstruction.hh"
#include "OpPhotonDetSD.hh"

#include "G4Colour.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4SDManager.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include <utility>
#include <iostream>
#include <memory>
#include <vector>
#include <stdio.h>

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
	//--------------------------------------------------
	// World
	//--------------------------------------------------

	fWorldSizeX = 1000 *mm;
	fWorldSizeY = 1000 *mm;
	fWorldSizeZ = 1000 *mm;
	G4VSolid* solidWorld =
    	new G4Box("World", fWorldSizeX/2, fWorldSizeY/2, fWorldSizeZ/2);

	G4LogicalVolume* logicWorld =
//    	new G4LogicalVolume(solidWorld, FindMaterial("G4_Galactic"), "World");
    	new G4LogicalVolume(solidWorld, fVac, "World");

	G4VPhysicalVolume* phyWorld =
    	new G4PVPlacement(0, G4ThreeVector(0,0,0), logicWorld, "World", 0, false, 0, false);
	//--------------------------------------------------
	// Envelope
	//--------------------------------------------------

	fEnvX = 0.9*fWorldSizeX;
	fEnvY = 0.9*fWorldSizeY;
	fEnvZ = 0.9*fWorldSizeZ;
	G4VSolid* solidEnv =
	    new G4Box("Envelope",fEnvX/2,fEnvY/2,fEnvZ/2);
	G4LogicalVolume* logicEnv =
//    	new G4LogicalVolume(solidEnv,FindMaterial("G4_AIR"),"Envelope");
    	new G4LogicalVolume(solidEnv,fAir,"Envelope");
	G4VPhysicalVolume* phyEnv =
    	new G4PVPlacement(0,G4ThreeVector(0,0,0),logicEnv,"Envelope",logicWorld,false,1,false);

	//--------------------------------------------------
	// Scintillator (SC)
	//--------------------------------------------------

	fSCX = 100 *mm; // default : 210
	fSCY = 100 *mm; // default : 210
	fSCZ = 10 *mm;   // default : 0.2
	G4VSolid* solidScintillator =
    	new G4Box("Scintillator",fSCX/2,fSCY/2,fSCZ/2);

	G4LogicalVolume* logicScintillator = new G4LogicalVolume(
//    	solidScintillator, FindMaterial("Polystyrene"), "Scintillator");
    	solidScintillator, fPS, "Scintillator");

	G4VPhysicalVolume* physScintillator = new G4PVPlacement(
    	    0, G4ThreeVector(), logicScintillator, "Scintillator", logicEnv, false, 2,false);

	G4VisAttributes* visS = new G4VisAttributes();
	visS -> SetColor(G4Color::Cyan());
	visS -> SetForceWireframe(true);
	logicScintillator -> SetVisAttributes(visS);

	//--------------------------------------------------
	// MPPC(SensitiveDetector)
	//--------------------------------------------------

	G4SDManager* SDman = G4SDManager::GetSDMpointer();

	fMPPCX = 10*mm;
	fMPPCY = 10*mm;
	fMPPCZ = 10*mm;

	G4VSolid* solidMPPC =
    	new G4Box("MPPC",fMPPCX/2,fMPPCY/2,fMPPCZ/2);
	const int NofArrX = (int)fSCX/fMPPCX;
	NofY = (int)fSCY/fMPPCY;
	G4VPhysicalVolume* phyMPPC_left[NofY];
	G4VPhysicalVolume* phyMPPC_right[NofY];
	for(int a=0; a<NofY; a++)
	{
		G4LogicalVolume* logicMPPCL =
    		new G4LogicalVolume(solidMPPC,fSi,"MPPCL"+to_string(100+a));
		G4LogicalVolume* logicMPPCR =
    		new G4LogicalVolume(solidMPPC,fSi,"MPPCR"+to_string(200+a));
    	vec_logicMPPCL.push_back(logicMPPCL);
    	vec_logicMPPCR.push_back(logicMPPCR);

		G4VisAttributes* visMPPC = new G4VisAttributes();
		visMPPC -> SetColor(G4Color::Brown());
		logicMPPCL -> SetVisAttributes(visMPPC);
		logicMPPCR -> SetVisAttributes(visMPPC);

	    G4double posX1 = -(fSCX/2+fMPPCX/2);
    	G4double posX2 =  (fSCX/2+fMPPCX/2);
	    G4double posY  = -fSCY/2+(a+0.5)*fMPPCY;
    	G4double posZ  = 0;
	    G4ThreeVector pos1(posX1,posY,posZ);
    	G4ThreeVector pos2(posX2,posY,posZ);
	    G4int MPPCID1 = 100 + a;    // left side
    	G4int MPPCID2 = 200 + a;    // right side
		G4String MPPC1str = "MPPC_"+to_string(MPPCID1);
		G4String MPPC2str = "MPPC_"+to_string(MPPCID2);
	    phyMPPC_left[a] = new G4PVPlacement(
    	        0,pos1,logicMPPCL,MPPC1str,logicEnv,false,MPPCID1,false);
	    phyMPPC_right[a] = new G4PVPlacement(
    	        0,pos2,logicMPPCR,MPPC2str,logicEnv,false,MPPCID2,false);
		new G4LogicalSkinSurface("surfMPPC",logicMPPCL,SkinSurfSi);
		new G4LogicalSkinSurface("surfMPPC",logicMPPCR,SkinSurfSi);
	}

	//-------------------------------------------------
	// Applying Surface
	//--------------------------------------------------
	new G4LogicalBorderSurface("surfPStoAir",physScintillator,phyEnv,BorderSurfAir);
	new G4LogicalBorderSurface("surfAirtoPS",phyEnv,physScintillator,BorderSurfAir);
	for(int a=0; a<NofY; a++)
	{
		G4String left = "surfMPPCtoPSleft_"+to_string(100+a);
		G4String right = "surfMPPCtoPSright_"+to_string(200+a);
    	new G4LogicalBorderSurface(
        	    left,physScintillator,phyMPPC_left[a],
	            BorderSurfSi);
    	new G4LogicalBorderSurface(
        	    right,physScintillator,phyMPPC_right[a],
            	BorderSurfSi);
	}

	return phyWorld;
}

void OpDetectorConstruction::ConstructSDandField()
{
	std::cout << "yjkim" << std::endl;
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	std::cout << NofY << std::endl;
	for(int a=0; a<NofY; a++)
	{
		G4String SDName_L = "MPPCleft_"+to_string(100+a);
		G4String SDName_R = "MPPCright_"+to_string(200+a);
		G4String HCName_L = "MPPCleftHC_"+to_string(100+a);
		G4String HCName_R = "MPPCrightHC_"+to_string(200+a);
		G4int MPPCID_L = 100 + a;
		G4int MPPCID_R = 200 + a;
		G4VSensitiveDetector* mppcSD_L = new OpPhotonDetSD(SDName_L,HCName_L,MPPCID_L);
		G4VSensitiveDetector* mppcSD_R = new OpPhotonDetSD(SDName_R,HCName_R,MPPCID_R);
//		OpPhotonDetSD* mppcSD_L = new OpPhotonDetSD(SDName_L,HCName_L,MPPCID_L);
//		OpPhotonDetSD* mppcSD_R = new OpPhotonDetSD(SDName_R,HCName_R,MPPCID_R);
		SDman -> AddNewDetector(mppcSD_L);
		SDman -> AddNewDetector(mppcSD_R);
		vec_logicMPPCL[a] -> SetSensitiveDetector(mppcSD_L);
		vec_logicMPPCR[a] -> SetSensitiveDetector(mppcSD_R);
	}
		std::cout << "yjkim3" << std::endl;
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
	mpPS ->AddConstProperty("RESOLUTIONSCALE",1.0);
	mpPS ->AddConstProperty("FASTTIMECONSTANT",2.8*ns);
	fPS->SetMaterialPropertiesTable(mpPS);
	fPS->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
	//--------------------------------------------------
	//  OpticalSurface - Si
	//--------------------------------------------------
	G4double refl_Si[nEn]; std::fill_n(refl_Si,nEn,0);
	G4double eff_Si[nEn] =
	{
	    0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.10,
	    0.11, 0.13, 0.15, 0.17, 0.19, 0.20, 0.22, 0.23,
	    0.24, 0.25, 0.24, 0.23, 0.21, 0.20, 0.17, 0.14, 0.10
	};
	mpSi = new G4MaterialPropertiesTable();
	mpSi -> AddProperty("REFLECTIVITY",opEn,refl_Si,nEn);
	mpSi -> AddProperty("EFFICIENCY",opEn,eff_Si,nEn);
	SkinSurfSi = new G4OpticalSurface("SkinSurfSi",glisur,polished,dielectric_metal);
	SkinSurfSi -> SetMaterialPropertiesTable(mpSi);
	//--------------------------------------------------
	//  OpticalSurface(PS to Si)
	//--------------------------------------------------
	G4double eff_surf_Si[nEn]; std::fill_n(eff_surf_Si,nEn,1.0);
	G4double ref_surf_Si[nEn]; std::fill_n(ref_surf_Si,nEn,0.0);
	G4MaterialPropertiesTable* mpSurf_Si = new G4MaterialPropertiesTable();
	mpSurf_Si -> AddProperty("TRANSMITTANCE",opEn,eff_surf_Si,nEn);
	mpSurf_Si -> AddProperty("REFLECTIVITY",opEn,ref_surf_Si,nEn);
	BorderSurfSi = new G4OpticalSurface("BorderSurfSi",glisur,polished,dielectric_dielectric);
	BorderSurfSi -> SetMaterialPropertiesTable(mpSurf_Si);
	//--------------------------------------------------
	//  OpticalSurface(PS to Air)
	//--------------------------------------------------
	G4double eff_surf_Air[nEn]; std::fill_n(eff_surf_Air,nEn,0.0);
	G4double ref_surf_Air[nEn]; std::fill_n(ref_surf_Air,nEn,1.0);
	G4MaterialPropertiesTable* mpSurf_Air = new G4MaterialPropertiesTable();
	mpSurf_Air -> AddProperty("TRANSMITTANCE",opEn,eff_surf_Air,nEn);
	mpSurf_Air -> AddProperty("REFLECTIVITY",opEn,ref_surf_Air,nEn);
	BorderSurfAir = new G4OpticalSurface("BorderSurfAir",glisur,polished,dielectric_metal);
	BorderSurfAir -> SetMaterialPropertiesTable(mpSurf_Air);
}
