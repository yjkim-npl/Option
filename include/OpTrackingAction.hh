#ifndef OpTrackingAction_h
#define OpTrackingAction_h 1

#include "OpEventAction.hh"
#include "OpRunAction.hh"

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"
#include "globals.hh"

class OpTrackingAction : public G4UserTrackingAction
{
	public:
		OpTrackingAction(OpRunAction*,OpEventAction*);
		virtual ~OpTrackingAction() {}

		virtual void PreUserTrackingAction(const G4Track* track);
//		G4int GetCount() {return count_i;}
	private:
		OpEventAction* fEventAction;
		OpRunAction* fRunAction;
};
#endif
