#ifndef OpTrackingAction_h
#define OpTrackingAction_h 1

#include "G4UserTrackingAction.hh"

class OpTrackingAction : public G4UserTrackingAction
{
 public:
  OpTrackingAction(){};
  ~OpTrackingAction(){};

  void PreUserTrackingAction(const G4Track*) override;
};

#endif
