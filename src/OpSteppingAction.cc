#include "OpSteppingAction.hh"
#include "OpEventAction.hh"
#include "OpDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalPhoton.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"


OpSteppingAction::OpSteppingAction(OpEventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{
	fOpProcess = nullptr;
}


OpSteppingAction::~OpSteppingAction()
{}


void OpSteppingAction::UserSteppingAction(const G4Step* step)
{
	if(step -> GetTrack() -> GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) return;
	G4OpBoundaryProcessStatus theStatus = Undefined;

	static G4ThreadLocal G4ProcessManager* OpManager = 
		G4OpticalPhoton::OpticalPhoton() -> GetProcessManager();

//	if(OpManager)
//	{
//		G4int nproc = OpManager -> GetPostStepProcessVector() -> entries();
//		G4ProcessVector* fPostStepDoItVector = 
//			OpManager -> GetPostStepProcessVector(typeDoIt);

//	if( step -> GetTrack() -> GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
//		std::cout << "optical photon was detected" << std::endl;

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
      
  // check if we are in scoring volume
//  if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);  
}
