#ifndef OpDetectorConstruction_h
#define OpDetectorConstruction_h 1

#include "OpPhotonDetSD.hh"
#include "OpPhotonDetHit.hh"
#include "OpMaterials.hh"

#include "globals.hh"
#include "G4Cache.hh"
#include "G4RotationMatrix.hh"
#include "G4VUserDetectorConstruction.hh"

class G4Box;
class G4EllipticalTube;
class G4LogicalVolume;
class G4Material;
class G4Tubs;
class G4VPhysicalVolume;

class OpDetectorConstruction : public G4VUserDetectorConstruction
{
 public:
  OpDetectorConstruction();
  ~OpDetectorConstruction();

  G4VPhysicalVolume* Construct() override;
  G4VPhysicalVolume* ConstructDetector();

  void ConstructSDandField() override;
  static int NofY;

  // Set Material Commands for World and Opfiber
//  void SetWorldMaterial(G4String);
//  void SetOpFiberMaterial(G4String);
//  void SetCoupleMaterial(G4String);
//
//  void SetPhotonDetGeometry(G4String);
//  void SetNumberOfCladding(G4int);  // Maximum 2 claddings
//
//  void SetOpLength(G4double);  // Total length of Op fiber
//  void SetOpRadius(G4double);
//  void SetClad1Radius(G4double);
//  void SetClad2Radius(G4double);
//  void SetPhotonDetHalfLength(G4double);
//  void SetGap(G4double);
//  void SetPhotonDetAlignment(G4double);
//  // Set the ratio of x and y (x/y) radius of the ellipse
//  void SetXYRatio(G4double);
//  // Set the Roughness in between each layer
//  void SetSurfaceRoughness(G4double);
//  // Set the reflectivity of the mirror
//  void SetMirrorReflectivity(G4double);
//  // Set the polish of the mirror
//  void SetMirrorPolish(G4double);
//  // Set the reflectivity of the mirror
//  void SetPhotonDetReflectivity(G4double);
//  // Set the polish of the mirror
//  void SetPhotonDetPolish(G4double);
//
//  void SetMirror(G4bool);
//
//  void SetBarLength(G4double);
//  void SetBarBase(G4double);
//  void SetHoleRadius(G4double);
//  void SetCoatingThickness(G4double);
//  void SetCoatingRadius(G4double);
//
//  G4double GetOpFiberLength();
//  G4double GetOpFiberEnd();
//  G4double GetOpFiberRMax();
//  G4double GetSurfaceRoughness();
//  G4bool IsPerfectFiber();
//
//  G4double GetBarLength();
//  G4double GetBarBase();
//  G4double GetHoleRadius();
//  G4double GetHoleLength();
//  G4double GetFiberRadius();
//
//  G4double GetCoatingThickness();
//  G4double GetCoatingRadius();
//
//  // StringToRotationMatrix() converts a string "X90,Y45" into a
//  // G4RotationMatrix.
//  // This is an active rotation, in that the object is first rotated
//  // around the parent's X axis by 90 degrees, then the object is
//  // further rotated around the parent's Y axis by 45 degrees.
//  // The return value points to a G4RotationMatrix on the heap, so
//  // it is persistent. Angles are in degrees, can have decimals,
//  // and can be negative. Axes are X, Y, Z.
//
//  static G4RotationMatrix StringToRotationMatrix(G4String rotation);
//
  G4Material* FindMaterial(G4String matName)
  	{return fMaterials -> GetMaterial(matName);}
  G4OpticalSurface* FindSurface(G4String surfName)
  	{return fMaterial* -> GetOpticalSurface(surfName);}

 private:
  OpMaterials* fMaterials;

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
  G4double fMPPCY;
  G4double fMPPCZ;

  vector<G4LogicalVolume*> vec_logicMPPC;
//  void ConstructFiber();

//  void UpdateGeometryParameters();

//  iG4Cache<OpPhotonDetSD*> fmppcSD;
};

#endif
