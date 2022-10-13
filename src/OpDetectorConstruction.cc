#include "OpDetectorConstruction.hh"

#include "OpMaterials.hh"
#include "OpPhotonDetSD.hh"

#include "globals.hh"
#include "G4Box.hh"
#include "G4EllipticalTube.hh"
#include "G4ios.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"
#include "G4PhysicalConstants.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"

OpDetectorConstruction::OpDetectorConstruction()
  : G4VUserDetectorConstruction()
	,NofY(0)
//	fMaterials(nullptr)
//  , fLogicHole(nullptr)
//  , fLogicWorld(nullptr)
//  , fPhysiWorld(nullptr)
//  , fPhysiHole(nullptr)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpDetectorConstruction::~OpDetectorConstruction()
{
  if(fMaterials)
    delete fMaterials;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* OpDetectorConstruction::Construct()
{
  if(fPhysiWorld)
  {
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    G4LogicalSkinSurface::CleanSurfaceTable();
    G4LogicalBorderSurface::CleanSurfaceTable();
  }

  fMaterials = OpMaterials::GetInstance();
  UpdateGeometryParameters();

  return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* OpDetectorConstruction::ConstructDetector()
{
	G4VisAttributes* visatt = new G4VisAttributes();
	//--------------------------------------------------
	// World
	//--------------------------------------------------

	fWorldSizeX = 1000 *mm;
	fWorldSizeY = 1000 *mm;
	fWorldSizeZ = 1000 *mm;
	G4VSolid* solidWorld =
    	new G4Box("World", fWorldSizeX/2, fWorldSizeY/2, fWorldSizeZ/2);

	G4LogicalVolume* logicWorld =
    	new G4LogicalVolume(solidWorld, FindMaterial("G4_Galactic"), "World");

	G4VPhysicalVolume* phyWorld =
    	new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, false);

	//--------------------------------------------------
	// Extrusion
	//--------------------------------------------------

//	G4VSolid* solidExtrusion = new G4Box("Extrusion", GetBarBase() / 2.,
//                                       GetBarBase() / 2., GetBarLength() / 2.);
//
//	G4LogicalVolume* logicExtrusion =
//	    new G4LogicalVolume(solidExtrusion, FindMaterial("Coating"), "Extrusion");
//
//	G4OpticalSurface* TiO2Surface = new G4OpticalSurface(
//    	"TiO2Surface", glisur, ground, dielectric_metal, fExtrusionPolish);
//
//	G4MaterialPropertiesTable* TiO2SurfaceProperty =
//	    new G4MaterialPropertiesTable();
//
//	std::vector<G4double> p_TiO2 = { 2.00 * eV, 3.47 * eV };
//
//	std::vector<G4double> refl_TiO2 = { fExtrusionReflectivity,
//                                      fExtrusionReflectivity };
//	std::vector<G4double> effi_TiO2 = { 0., 0. };
//
//	TiO2SurfaceProperty->AddProperty("REFLECTIVITY", p_TiO2, refl_TiO2);
//	TiO2SurfaceProperty->AddProperty("EFFICIENCY", p_TiO2, effi_TiO2);
//
//	TiO2Surface->SetMaterialPropertiesTable(TiO2SurfaceProperty);
//
//	new G4PVPlacement(0, G4ThreeVector(), logicExtrusion, "Extrusion",
//                    fLogicWorld, false, 0);
//
//	new G4LogicalSkinSurface("TiO2Surface", logicExtrusion, TiO2Surface);

	//--------------------------------------------------
	// Envelope
	//--------------------------------------------------

	fEnvX = 0.9*fWorldSizeX;
	fEnvY = 0.9*fWorldSizeY;
	fEnvZ = 0.9*fWorldSizeZ;
	G4VSolid* solidEnv = 
		new G4Box("Envelope",fEnvX/2,fEnvY/2,fEnvZ/2);
	G4LogicalVolume* logicEnv = 
		new G4LogicalVolume(solidEnv,FindMaterial("G4_AIR"),"Envelope");
	G4VPhysicalVolume* phyEnv = 
		new G4PVPlacement(0,G4ThreeVector(),logicEnv,"Envelope",logicWorld,false,1,false);

	//--------------------------------------------------
	// Scintillator (SC)
	//--------------------------------------------------

	fSCX = 100 *mm;	// default : 210
	fSCY = 100 *mm;	// default : 210
	fSCZ = 1 *mm;	// default : 0.2
	G4VSolid* solidScintillator =
		new G4Box("Scintillator",fSCX/2,fSCY/2,fSCZ/2);

	G4LogicalVolume* logicScintillator = new G4LogicalVolume(
    	solidScintillator, FindMaterial("Polystyrene"), "Scintillator");

	G4VPhysicalVolume* physScintillator = new G4PVPlacement(
			0, G4ThreeVector(), logicScintillator, "Scintillator", logicEnv, false, 2,false);

//  if(GetCoatingRadius() > 0.)
//  {
//    G4VSolid* solidScintside =
//      new G4Box("SideOfBar",
//                GetBarBase() / 2. - GetCoatingThickness() - GetCoatingRadius(),
//                GetCoatingRadius() / 2., GetBarLength() / 2.);
//
//    G4VSolid* solidScintcrnr =
//      new G4Tubs("CrnrOfBar", 0.0 * cm, GetCoatingRadius(), GetBarLength() / 2.,
//                 0. * deg, 90. * deg);
//
//    G4LogicalVolume* logicScintSide = new G4LogicalVolume(
//      solidScintside, FindMaterial("Polystyrene"), "SideOfBar");
//
//    G4LogicalVolume* logicScintCrnr = new G4LogicalVolume(
//      solidScintcrnr, FindMaterial("Polystyrene"), "CrnrOfBar");
//
//    G4double pos =
//      GetBarBase() / 2. - GetCoatingThickness() - GetCoatingRadius() / 2.;
//
//    new G4PVPlacement(nullptr, G4ThreeVector(0., -pos, 0.), logicScintSide,
//                      "SideOfBar", logicExtrusion, false, 0);
//
//    new G4PVPlacement(nullptr, G4ThreeVector(0., pos, 0.), logicScintSide,
//                      "SideOfBar", logicExtrusion, false, 1);
//
//    G4RotationMatrix* rot1 = new G4RotationMatrix();
//    *rot1                  = StringToRotationMatrix("Z90");
//    *rot1                  = rot1->inverse();
//    if(*rot1 == G4RotationMatrix())
//    {
//      rot1 = nullptr;
//    }
//
//    new G4PVPlacement(rot1, G4ThreeVector(pos, 0., 0.), logicScintSide,
//                      "SideOfBar", logicExtrusion, false, 2);
//
//    new G4PVPlacement(rot1, G4ThreeVector(-pos, 0., 0.), logicScintSide,
//                      "SideOfBar", logicExtrusion, false, 3);
//
//    pos = GetBarBase() / 2. - GetCoatingThickness() - GetCoatingRadius();
//
//    new G4PVPlacement(nullptr, G4ThreeVector(pos, pos, 0.), logicScintCrnr,
//                      "CrnrOfBar", logicExtrusion, false, 0);
//
//    new G4PVPlacement(rot1, G4ThreeVector(-pos, pos, 0.), logicScintCrnr,
//                      "CrnrOfBar", logicExtrusion, false, 1);
//
//    G4RotationMatrix* rot2 = new G4RotationMatrix();
//    *rot2                  = StringToRotationMatrix("Z180");
//    *rot2                  = rot2->inverse();
//    if(*rot2 == G4RotationMatrix())
//    {
//      rot2 = nullptr;
//    }
//
//    new G4PVPlacement(rot2, G4ThreeVector(-pos, -pos, 0.), logicScintCrnr,
//                      "CrnrOfBar", logicExtrusion, false, 2);
//
//    G4RotationMatrix* rot3 = new G4RotationMatrix();
//    *rot3                  = StringToRotationMatrix("Z270");
//    *rot3                  = rot3->inverse();
//    if(*rot3 == G4RotationMatrix())
//    {
//      rot3 = nullptr;
//    }
//
//    new G4PVPlacement(rot3, G4ThreeVector(pos, -pos, 0.), logicScintCrnr,
//                      "CrnrOfBar", logicExtrusion, false, 3);
//  }
//
//  if(GetFiberRadius() < GetHoleRadius())
//  {
//    G4VSolid* solidHole = new G4Tubs(
//      "Hole", 0., GetHoleRadius(), GetHoleLength() / 2., 0. * deg, 360. * deg);
//
//    fLogicHole = new G4LogicalVolume(solidHole, FindMaterial("G4_AIR"), "Hole");
//
//    fPhysiHole = new G4PVPlacement(0, G4ThreeVector(), fLogicHole, "Hole",
//                                   logicScintillator, false, 0);
//  }
 
	//--------------------------------------------------
	// MPPC(SensitiveDetector)
	//--------------------------------------------------
	fMPPCX = 0.1*mm;
	fMPPCY = 1*mm;
	fMPPCZ = 1*mm;

	G4VSolid* solidMPPC = 
		new G4Box("MPPC",fMPPCX,fMPPCY,fMPPCZ);
	G4LogicalVolume* logicMPPC =
		new G4LogicalVolume(solidMPPC,FindMaterial("G4_Si"),"MPPC");
	const int NofArrX = (int)fSCX/fMPPCX;
	NofY = (int)fSCY/fMPPCY;
	G4VPhysicalVolume* phyMPPC_left[NofY]; 
	G4VPhysicalVolume* phyMPPC_right[NofY]; 
	for(int a=0; a<NofY; a++)
	{
		vec_logicMPPC.push_back(logicMPPC);
		G4double posX1 = -(fSCX/2+fMPPCX/2);
		G4double posX2 =  (fSCX/2+fMPPCX/2);
		G4double posY  = -fSCY/2+(a+0.5)*fMPPCY;
		G4double posZ  = 0;
		G4ThreeVector pos1(posX1,posY,posZ);
		G4ThreeVector pos2(posX2,posY,posZ);
		G4int MPPCID1 = 100 + a;	// left side
		G4int MPPCID2 = 200 + a;	// right side
		phyMPPC_left[a] = new G4PVPlacement(
				0,pos1,logicMPPC,Form("MPPC_%d",MPPCID1),logicEnv,false,MPPCID1,false);
		phyMPPC_right[a] = new G4PVPlacement(
				0,pos2,logicMPPC,Form("MPPC_%d",MPPCID2),logicEnv,false,MPPCID2,false);
	}

	//--------------------------------------------------
	// Applying Surface 
	//--------------------------------------------------
	new G4LogicalSkinSurface("surfMPPC",logicMPPC,FindSurface("SkinSurfSi"));
	new G4LogicalBorderSurface("surfPStoAir",phyScintillator,phyEnv,FindSurface("BorderSurfAir"));
	for(int a=0; a<NofY; a++)
	{
		new G4LogicalBorderSurface(
				Form("surfMPPCtoPSleft_%d",a),phyScintillator,phyMPPC_left[a],
				FindSurface("BorderSurfSi"));
		new G4LogicalBorderSurface(
				Form("surfMPPCtoPSright_%d",a),phyScintillator,phyMPPC_right[a]
				FindSurface("BorderSurfSi"));
	}
  //--------------------------------------------------
  // Fiber
  //--------------------------------------------------

//  ConstructFiber();

  return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//void OpDetectorConstruction::ConstructFiber()
//{
//  if(!(fLogicHole) || !(fPhysiHole))
//  {
//    G4ExceptionDescription ed;
//    ed << "The Fiber Hole has not been constructed";
//    G4Exception("OpDetectorConstruction::ConstructFiber", "", FatalException,
//                ed);
//  }
//
//  // Pointers to the most recently constructed volume
//  G4LogicalVolume* logicPlacement   = fLogicHole;
//  G4VPhysicalVolume* physiPlacement = fPhysiHole;
//
//  //--------------------------------------------------
//  // Fiber Construction
//  //--------------------------------------------------
//
//  // Boundary Surface Properties
//  G4OpticalSurface* opSurface = nullptr;
//
//  if(fSurfaceRoughness < 1.)
//    opSurface = new G4OpticalSurface("RoughSurface", glisur, ground,
//                                     dielectric_dielectric, fSurfaceRoughness);
//
//  G4LogicalVolume* logicClad1   = nullptr;
//  G4LogicalVolume* logicClad2   = nullptr;
//  G4VPhysicalVolume* physiClad1 = nullptr;
//  G4VPhysicalVolume* physiClad2 = nullptr;
//
//  // Determine the number of cladding layers to be built
//  switch(fNumOfCladLayers)
//  {
//    case 2:
//
//      //--------------------------------------------------
//      // Cladding 2
//      //--------------------------------------------------
//
//      G4VSolid* solidClad2;
//
//      if(fXYRatio == 1.)
//        solidClad2 = new G4Tubs("Clad2", 0., fClad2RX, fClad2Z, 0., twopi);
//      else
//        solidClad2 = new G4EllipticalTube("Clad2", fClad2RX, fClad2RY, fClad2Z);
//
//      logicClad2 =
//        new G4LogicalVolume(solidClad2, FindMaterial("FPethylene"), "Clad2");
//
//      physiClad2 =
//        new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, fOpfiberOrigin),
//                          logicClad2, "Clad2", logicPlacement, false, 0);
//
//      // Place the rough surface only if needed
//      if(opSurface)
//      {
//        new G4LogicalBorderSurface("surfaceClad2Out", physiClad2,
//                                   physiPlacement, opSurface);
//        new G4LogicalBorderSurface("surfaceClad2In", physiPlacement, physiClad2,
//                                   opSurface);
//      }
//
//      logicPlacement = logicClad2;
//      physiPlacement = physiClad2;
//      break;
//
//    case 1:
//
//      //--------------------------------------------------
//      // Cladding 1
//      //--------------------------------------------------
//
//      G4VSolid* solidClad1;
//
//      if(fXYRatio == 1.)
//        solidClad1 = new G4Tubs("Clad1", 0., fClad1RX, fClad1Z, 0., twopi);
//      else
//        solidClad1 = new G4EllipticalTube("Clad1", fClad1RX, fClad1RY, fClad1Z);
//
//      logicClad1 =
//        new G4LogicalVolume(solidClad1, FindMaterial("Pethylene"), "Clad1");
//
//      physiClad1 =
//        new G4PVPlacement(0, G4ThreeVector(0., 0., fOpfiberOrigin), logicClad1,
//                          "Clad1", logicPlacement, false, 0);
//
//      // Place the rough surface only if needed
//      if(opSurface)
//      {
//        new G4LogicalBorderSurface("surfaceClad1Out", physiClad1,
//                                   physiPlacement, opSurface);
//
//        new G4LogicalBorderSurface("surfaceClad1In", physiPlacement, physiClad1,
//                                   opSurface);
//      }
//
//      logicPlacement = logicClad1;
//      physiPlacement = physiClad1;
//      break;
//
//    default:
//
//      //--------------------------------------------------
//      // Op Fiber
//      //--------------------------------------------------
//
//      G4VSolid* solidOpfiber;
//
//      if(fXYRatio == 1.)
//      {
//        solidOpfiber =
//          new G4Tubs("OpFiber", 0., fOpfiberRX, fOpfiberZ, 0., twopi);
//      }
//      else
//      {
//        solidOpfiber = new G4EllipticalTube("OpFiber", fOpfiberRX,
//                                             fOpfiberRY, fOpfiberZ);
//      }
//
//      G4LogicalVolume* logicOpfiber =
//        new G4LogicalVolume(solidOpfiber, FindMaterial("PMMA"), "OpFiber");
//
//      logicOpfiber->SetUserLimits(
//        new G4UserLimits(DBL_MAX, DBL_MAX, 10. * ms));
//
//      G4VPhysicalVolume* physiOpfiber =
//        new G4PVPlacement(0, G4ThreeVector(0., 0., fOpfiberOrigin),
//                          logicOpfiber, "OpFiber", logicPlacement, false, 0);
//
//      // Place the rough surface only if needed
//      if(opSurface)
//      {
//        new G4LogicalBorderSurface("surfaceOpOut", physiOpfiber,
//                                   physiPlacement, opSurface);
//
//        new G4LogicalBorderSurface("surfaceOpIn", physiPlacement,
//                                   physiOpfiber, opSurface);
//      }
//  }
//
//  //--------------------------------------------------
//  // Mirror for reflection at one of the end
//  //--------------------------------------------------
//
//  // Place the mirror only if the user wants the mirror
//  if(fMirrorToggle)
//  {
//    G4VSolid* solidMirror =
//      new G4Box("Mirror", fMirrorRmax, fMirrorRmax, fMirrorZ);
//
//    G4LogicalVolume* logicMirror =
//      new G4LogicalVolume(solidMirror, FindMaterial("G4_Al"), "Mirror");
//
//    G4OpticalSurface* mirrorSurface = new G4OpticalSurface(
//      "MirrorSurface", glisur, ground, dielectric_metal, fMirrorPolish);
//
//    G4MaterialPropertiesTable* mirrorSurfaceProperty =
//      new G4MaterialPropertiesTable();
//
//    std::vector<G4double> p_mirror    = { 2.00 * eV, 3.47 * eV };
//    std::vector<G4double> refl_mirror = { fMirrorReflectivity,
//                                          fMirrorReflectivity };
//    std::vector<G4double> effi_mirror = { 0., 0. };
//
//    mirrorSurfaceProperty->AddProperty("REFLECTIVITY", p_mirror, refl_mirror);
//    mirrorSurfaceProperty->AddProperty("EFFICIENCY", p_mirror, effi_mirror);
//
//    mirrorSurface->SetMaterialPropertiesTable(mirrorSurfaceProperty);
//
//    new G4PVPlacement(0, G4ThreeVector(0., 0., fMirrorOrigin), logicMirror,
//                      "Mirror", fLogicWorld, false, 0);
//
//    new G4LogicalSkinSurface("MirrorSurface", logicMirror, mirrorSurface);
//  }
//
//  //--------------------------------------------------
//  // Coupling at the read-out end
//  //--------------------------------------------------
//
//  // Clear Fiber (Coupling Layer)
//  G4VSolid* solidCouple = new G4Box("Couple", fCoupleRX, fCoupleRY, fCoupleZ);
//
//  G4LogicalVolume* logicCouple =
//    new G4LogicalVolume(solidCouple, FindMaterial("G4_AIR"), "Couple");
//
//  new G4PVPlacement(0, G4ThreeVector(0., 0., fCoupleOrigin), logicCouple,
//                    "Couple", fLogicWorld, false, 0);
//
//  //--------------------------------------------------
//  // A logical layer in front of PhotonDet
//  //--------------------------------------------------
//
//  // Purpose: Preventing direct dielectric to metal contact
//
//  // Check for valid placement of PhotonDet
//  if(fMPPCTheta > std::atan(fMPPCDist / fMPPCHalfL))
//  {
//    fMPPCTheta   = 0.;
//    fMPPCOriginX = std::sin(fMPPCTheta) * (fMPPCDist + fClrfiberZ);
//    fMPPCOriginZ = -fCoupleZ + std::cos(fMPPCTheta) * (fMPPCDist + fClrfiberZ);
//    G4cerr << "Invalid alignment.  Alignment Reset to 0" << G4endl;
//  }
//
//  // Clear Fiber (Coupling Layer)
//  G4VSolid* solidClrfiber;
//
//  if(fMPPCShape == "Square")
//  {
//    solidClrfiber =
//      new G4Box("ClearFiber", fClrfiberHalfL, fClrfiberHalfL, fClrfiberZ);
//  }
//  else
//  {
//    solidClrfiber =
//      new G4Tubs("ClearFiber", 0., fClrfiberHalfL, fClrfiberZ, 0., twopi);
//  }
//
//  G4LogicalVolume* logicClrfiber =
//    new G4LogicalVolume(solidClrfiber, FindMaterial("G4_AIR"), "ClearFiber");
//
//  new G4PVPlacement(new G4RotationMatrix(CLHEP::HepRotationY(-fMPPCTheta)),
//                    G4ThreeVector(fMPPCOriginX, 0.0, fMPPCOriginZ),
//                    logicClrfiber, "ClearFiber", logicCouple, false, 0);
//
//  //--------------------------------------------------
//  // PhotonDet (Sensitive Detector)
//  //--------------------------------------------------
//
//  // Physical Construction
//  G4VSolid* solidPhotonDet = nullptr;
//
//  if(fMPPCShape == "Square")
//    solidPhotonDet = new G4Box("PhotonDet", fMPPCHalfL, fMPPCHalfL, fMPPCZ);
//  else
//    solidPhotonDet = new G4Tubs("PhotonDet", 0., fMPPCHalfL, fMPPCZ, 0., twopi);
//
//  G4LogicalVolume* logicPhotonDet =
//    new G4LogicalVolume(solidPhotonDet, FindMaterial("G4_Al"), "PhotonDet_LV");
//
//  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicPhotonDet, "PhotonDet",
//                    logicClrfiber, false, 0);
//
//  // PhotonDet Surface Properties
//  G4OpticalSurface* photonDetSurface = new G4OpticalSurface(
//    "PhotonDetSurface", glisur, ground, dielectric_metal, fMPPCPolish);
//
//  G4MaterialPropertiesTable* photonDetSurfaceProperty =
//    new G4MaterialPropertiesTable();
//
//  std::vector<G4double> p_mppc    = { 2.00 * eV, 3.47 * eV };
//  std::vector<G4double> refl_mppc = { fMPPCReflectivity, fMPPCReflectivity };
//  std::vector<G4double> effi_mppc = { 1., 1. };
//
//  photonDetSurfaceProperty->AddProperty("REFLECTIVITY", p_mppc, refl_mppc);
//  photonDetSurfaceProperty->AddProperty("EFFICIENCY", p_mppc, effi_mppc);
//
//  photonDetSurface->SetMaterialPropertiesTable(photonDetSurfaceProperty);
//
//  new G4LogicalSkinSurface("PhotonDetSurface", logicPhotonDet,
//                           photonDetSurface);
//}


void OpDetectorConstruction::ConstructSDandField()
{
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	for(int a=0; a<NofY; a++)
	{
		G4String SDName_L = Form("MPPCleft_%d",a);
		G4String SDName_R = Form("MPPCright_%d",a);
		G4String HCName_L = Form("MPPCleftHC_%d",a);
		G4String HCName_R = Form("MPPCrightHC_%d",a);
		G4int MPPCID_L = 100 + a;
		G4int MPPCID_R = 200 + a;
		OpPhotonDetSD* mppcSD_L = new OpPhotonDetSD(SDName_L,HCName_L,MPPCID_L);
		OpPhotonDetSD* mppcSD_R = new OpPhotonDetSD(SDName_R,HCName_R,MPPCID_R);
		SDman -> AddNewDetector(mppcSD_L);
		SDman -> AddNewDetector(mppcSD_R);
		vec_logicMPPC[a] -> SetSensitiveDetector(mppcSD_L);
		vec_logicMPPC[100+a] -> SetSensitiveDetector(mppcSD_R);
	}

//  if(!fmppcSD.Get())
//  {
//    G4String mppcSDName    = "PhotonDetSD";
//    OpPhotonDetSD* mppcSD = new OpPhotonDetSD(mppcSDName,"MPPC_HC",);
//    G4SDManager::GetSDMpointer()->AddNewDetector(mppcSD);
//    fmppcSD.Put(mppcSD);
//  }
//  SetSensitiveDetector("PhotonDet_LV", fmppcSD.Get(), true);
}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::UpdateGeometryParameters()
//{
//  fOpfiberRX = fXYRatio * fOpfiberRY;
//
//  fClad1RX = fOpfiberRX + 0.03 * fOpfiberRX;
//  fClad1RY = fOpfiberRY + 0.03 * fOpfiberRY;
//  fClad1Z  = fOpfiberZ;
//
//  fClad2RX = fClad1RX + 0.03 * fOpfiberRX;
//  fClad2RY = fClad1RY + 0.03 * fOpfiberRY;
//  fClad2Z  = fOpfiberZ;
//
//  fWorldSizeX = fClad2RX + fMPPCDist + fMPPCHalfL + 1. * cm;
//  fWorldSizeY = fClad2RY + fMPPCDist + fMPPCHalfL + 1. * cm;
//  fWorldSizeZ = fOpfiberZ + fMPPCDist + fMPPCHalfL + 1. * cm;
//
//  fCoupleRX = fWorldSizeX;
//  fCoupleRY = fWorldSizeY;
//  fCoupleZ  = (fWorldSizeZ - fOpfiberZ) / 2.;
//
//  fClrfiberHalfL = fMPPCHalfL;
//
//  fMirrorRmax = fClad2RY;
//
//  fCoupleOrigin = fOpfiberOrigin + fOpfiberZ + fCoupleZ;
//  fMirrorOrigin = fOpfiberOrigin - fOpfiberZ - fMirrorZ;
//  fMPPCOriginX  = std::sin(fMPPCTheta) * (fMPPCDist + fClrfiberZ);
//  fMPPCOriginZ  = -fCoupleZ + std::cos(fMPPCTheta) * (fMPPCDist + fClrfiberZ);
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4RotationMatrix OpDetectorConstruction::StringToRotationMatrix(
//  G4String rotation)
//{
//  // We apply successive rotations OF THE OBJECT around the FIXED
//  // axes of the parent's local coordinates; rotations are applied
//  // left-to-right (rotation="r1,r2,r3" => r1 then r2 then r3).
//
//  G4RotationMatrix rot;
//
//  unsigned int place = 0;
//
//  while(place < rotation.size())
//  {
//    G4double angle;
//    char* p;
//
//    const G4String tmpstring = rotation.substr(place + 1);
//    angle                    = strtod(tmpstring.c_str(), &p) * deg;
//
//    if(!p || (*p != (char) ',' && *p != (char) '\0'))
//    {
//      G4cerr << "Invalid rotation specification: " << rotation.c_str()
//             << G4endl;
//      return rot;
//    }
//
//    G4RotationMatrix thisRotation;
//
//    switch(rotation.substr(place, 1).c_str()[0])
//    {
//      case 'X':
//      case 'x':
//        thisRotation = G4RotationMatrix(CLHEP::HepRotationX(angle));
//        break;
//      case 'Y':
//      case 'y':
//        thisRotation = G4RotationMatrix(CLHEP::HepRotationY(angle));
//        break;
//      case 'Z':
//      case 'z':
//        thisRotation = G4RotationMatrix(CLHEP::HepRotationZ(angle));
//        break;
//      default:
//        G4cerr << " Invalid rotation specification: " << rotation << G4endl;
//        return rot;
//    }
//
//    rot   = thisRotation * rot;
//    place = rotation.find(',', place);
//    if(place > rotation.size())
//      break;
//    ++place;
//  }
//
//  return rot;
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetPhotonDetGeometry(G4String shape)
//// Set the Geometry of the PhotonDet detector
//// Pre:  shape must be either "Circle" and "Square"
//{
//  if(shape == "Circle" || shape == "Square")
//    fMPPCShape = shape;
//  G4RunManager::GetRunManager()->ReinitializeGeometry();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetNumberOfCladding(G4int num)
//// Set the number of claddings
//// Pre: 0 <= num <= 2
//{
//  fNumOfCladLayers = num;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetOpLength(G4double length)
//// Set the TOTAL length of the Op fiber
//{
//  fOpfiberZ = length;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetOpRadius(G4double radius)
//// Set the Y radius of Op fiber
//{
//  fOpfiberRY = radius;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetClad1Radius(G4double radius)
//// Set the Y radius of Cladding 1
//{
//  fClad1RY = radius;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetClad2Radius(G4double radius)
//// Set the Y radius of Cladding 2
//{
//  fClad2RY = radius;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetPhotonDetHalfLength(G4double halfL)
//// Set the half length of the PhotonDet detector
//// The half length will be the radius if PhotonDet is circular
//{
//  fMPPCHalfL = halfL;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetGap(G4double gap)
//// Set the distance between fiber end and PhotonDet
//{
//  fMPPCDist = gap;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetPhotonDetAlignment(G4double theta)
//// Set the Aligment of PhotonDet with respect to the z axis
//// If theta is 0 deg, then the detector is perfectly aligned
//// PhotonDet will be deviated by theta from z axis
//// facing towards the center of the fiber
//{
//  fMPPCTheta = theta;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetSurfaceRoughness(G4double roughness)
//// Set the Surface Roughness between Cladding 1 and Op fiber
//// Pre: 0 < roughness <= 1
//{
//  fSurfaceRoughness = roughness;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetMirrorPolish(G4double polish)
//// Set the Polish of the mirror, polish of 1 is a perfect mirror surface
//// Pre: 0 < polish <= 1
//{
//  fMirrorPolish = polish;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetMirrorReflectivity(G4double reflectivity)
//// Set the Reflectivity of the mirror, reflectivity of 1 is a perfect mirror
//// Pre: 0 < reflectivity <= 1
//{
//  fMirrorReflectivity = reflectivity;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetPhotonDetPolish(G4double polish)
//// Set the Polish of the PhotonDet, polish of 1 is a perfect mirror surface
//// Pre: 0 < polish <= 1
//{
//  fMPPCPolish = polish;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetPhotonDetReflectivity(G4double reflectivity)
//// Set the Reflectivity of the PhotonDet, reflectivity of 1 is a perfect mirror
//// Pre: 0 < reflectivity <= 1
//{
//  fMPPCReflectivity = reflectivity;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetMirror(G4bool flag)
//// Toggle to place the mirror or not at one end (-z end) of the fiber
//// True means place the mirror, false means otherwise
//{
//  fMirrorToggle = flag;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetXYRatio(G4double r)
//// Set the ratio of the x and y radius of the ellipse (x/y)
//// a ratio of 1 would produce a circle
//{
//  fXYRatio = r;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetBarLength(G4double length)
//// Set the length of the scintillator bar
//{
//  fBarLength = length;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetBarBase(G4double side)
//// Set the side of the scintillator bar
//{
//  fBarBase = side;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetHoleRadius(G4double radius)
//// Set the radius of the fiber hole
//{
//  fHoleRadius = radius;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetCoatingThickness(G4double thick)
//// Set thickness of the coating on the bars
//{
//  fCoatingThickness = thick;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpDetectorConstruction::SetCoatingRadius(G4double radius)
//// Set inner radius of the corner bar coating
//{
//  fCoatingRadius = radius;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4double OpDetectorConstruction::GetOpFiberLength() { return fOpfiberZ; }
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4double OpDetectorConstruction::GetBarLength() { return fBarLength; }
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4double OpDetectorConstruction::GetBarBase() { return fBarBase; }
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4double OpDetectorConstruction::GetHoleRadius() { return fHoleRadius; }
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4double OpDetectorConstruction::GetHoleLength() { return fHoleLength; }
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4double OpDetectorConstruction::GetFiberRadius() { return GetOpFiberRMax(); }
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4double OpDetectorConstruction::GetCoatingThickness()
//{
//  return fCoatingThickness;
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4double OpDetectorConstruction::GetCoatingRadius() { return fCoatingRadius; }
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4double OpDetectorConstruction::GetOpFiberEnd()
//{
//  return fOpfiberOrigin + fOpfiberZ;
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4double OpDetectorConstruction::GetOpFiberRMax()
//{
//  if(fNumOfCladLayers == 2)
//    return fClad2RY;
//  if(fNumOfCladLayers == 1)
//    return fClad1RY;
//  return fOpfiberRY;
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4double OpDetectorConstruction::GetSurfaceRoughness()
//{
//  return fSurfaceRoughness;
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//// Return True if the fiber construction is ideal
//G4bool OpDetectorConstruction::IsPerfectFiber()
//{
//  return fSurfaceRoughness == 1. && fXYRatio == 1. &&
//         (!fMirrorToggle || (fMirrorPolish == 1. && fMirrorReflectivity == 1.));
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4Material* OpDetectorConstruction::FindMaterial(G4String name)
//{
//  G4Material* material = G4Material::GetMaterial(name, true);
//  return material;
//}
