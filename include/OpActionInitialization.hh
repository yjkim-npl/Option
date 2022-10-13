#ifndef OpActionInitialization_h
#define OpActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class OpDetectorConstruction;

class OpActionInitialization : public G4VUserActionInitialization
{
 public:
  OpActionInitialization(OpDetectorConstruction*);
  ~OpActionInitialization();

  void BuildForMaster() const override;
  void Build() const override;

 private:
  OpDetectorConstruction* fDetector;
};

#endif
