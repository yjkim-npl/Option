#ifndef OpMaterials_h
#define OpMaterials_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"

class G4Material;
class G4NistManager;

class OpMaterials
{
 public:
  ~OpMaterials();
  static OpMaterials* GetInstance();
  G4Material* GetMaterial(const G4String);
  G4OpticalSurface* GetOpticalSurface(const G4String);

 private:
  OpMaterials();
  void CreateMaterials();

  static OpMaterials* fInstance;
  G4NistManager* fNistMan;
  G4Material* fVac;
  G4Material* fAir;
//  G4Material* fPMMA;
//  G4Material* fPethylene;
//  G4Material* fFPethylene;
  G4Material* fPS;
  G4Material* fSi;
  G4OpticalSurface* SkinSurfSi;
  G4OpticalSurface* BorderSurfSi;
  G4OpticalSurface* BorderSurfAir;
//  G4Material* fCoating;
};

#endif /*OpMaterials_h*/
