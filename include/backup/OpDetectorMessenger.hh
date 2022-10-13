#ifndef OpDetectorMessenger_h
#define OpDetectorMessenger_h 1

#include "OpDetectorConstruction.hh"

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;

class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithoutParameter;

class OpDetectorMessenger : public G4UImessenger
{
 public:
  OpDetectorMessenger(OpDetectorConstruction*);
  ~OpDetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String) override;

 private:
  OpDetectorConstruction* fDetector;

  G4UIdirectory* fDetDir;

  G4UIcmdWithAString* fSetPhotonDetGeometryCmd;
  G4UIcmdWithAnInteger* fSetNumOfCladLayersCmd;
  G4UIcmdWithADoubleAndUnit* fSetOpLengthCmd;
  G4UIcmdWithADoubleAndUnit* fSetOpRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fSetClad1RadiusCmd;
  G4UIcmdWithADoubleAndUnit* fSetClad2RadiusCmd;
  G4UIcmdWithADoubleAndUnit* fSetPhotonDetHalfLengthCmd;
  G4UIcmdWithADoubleAndUnit* fSetGapCmd;
  G4UIcmdWithADoubleAndUnit* fSetPhotonDetAlignmentCmd;
  G4UIcmdWithADouble* fSetXYRatioCmd;
  G4UIcmdWithADouble* fSetSurfaceRoughnessCmd;
  G4UIcmdWithADouble* fSetMirrorPolishCmd;
  G4UIcmdWithADouble* fSetMirrorReflectivityCmd;
  G4UIcmdWithADouble* fSetPhotonDetPolishCmd;
  G4UIcmdWithADouble* fSetPhotonDetReflectivityCmd;
  G4UIcmdWithABool* fSetMirrorCmd;
  G4UIcmdWithADoubleAndUnit* fSetBarLengthCmd;
  G4UIcmdWithADoubleAndUnit* fSetBarBaseCmd;
  G4UIcmdWithADoubleAndUnit* fSetHoleRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fSetCoatingThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fSetCoatingRadiusCmd;
};

#endif
