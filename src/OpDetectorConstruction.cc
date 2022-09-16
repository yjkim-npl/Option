#include "OpDetectorConstruction.hh"
#include "OpParameterContainer.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4VSolid.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4String.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

OpDetectorConstruction::OpDetectorConstruction() : G4VUserDetectorConstruction() {}

OpDetectorConstruction::~OpDetectorConstruction() {}

G4VPhysicalVolume* OpDetectorConstruction::Construct()
{
	G4NistManager* nist = G4NistManager::Instance();

	//World
	G4Material* world_mat = nist -> FindOrBuildMaterial("G4_AIR");
	G4double world_size = 1000 *cm;
	G4Box* solidWorld = new G4Box("solidWorld",0.5*world_size,0.5*world_size,0.5*world_size);
	G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,world_mat,"logicWorld");
	G4VPhysicalVolume* physWorld = 
		new G4PVPlacement(0,G4ThreeVector(0,0,0),logicWorld,"World",0,false,0,true);

	// detector
	G4Material* det_mat = nist -> FindOrBuildMaterial("G4_Pb");
	G4double det_size = 20*mm;
	G4Box* solidDet = new G4Box("solidDet",0.5*det_size,0.5*det_size,0.5*det_size);
	G4LogicalVolume* logicDet = new G4LogicalVolume(solidDet,det_mat,"logicDet");
	new G4PVPlacement(0, G4ThreeVector(0,0,0),logicDet,"Det",logicWorld,false,1,true);
	return physWorld;
}
