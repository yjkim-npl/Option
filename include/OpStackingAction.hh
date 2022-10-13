#ifndef OpStackingAction_h
#define OpStackingAction_h 1

#include "G4Types.hh"
#include "G4UserStackingAction.hh"

class OpStackingAction : public G4UserStackingAction
{
 public:
  OpStackingAction();
  ~OpStackingAction();

  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*) override;
  void NewStage() override;
  void PrepareNewEvent() override;

 private:
  G4int fPhotonCounter;
};

#endif
