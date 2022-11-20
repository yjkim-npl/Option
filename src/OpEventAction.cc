#include "OpDetectorConstruction.hh"
#include "OpEventAction.hh"

//#include "OpPhotonDetHit.hh"
//#include "OpRun.hh"
#include "OpRunAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VVisManager.hh"
#include "Randomize.hh"
#include "g4root.hh"


OpEventAction::OpEventAction(OpRunAction* runAction, OpDetectorConstruction* det) 
	:G4UserEventAction(),
	frunAction(runAction),
	fDetector(det)
{
	G4RunManager::GetRunManager() -> SetPrintProgress(1);
	fCount = 0;
	fCount_Scint = 0;
	fCount_air = 0;
}

OpEventAction::~OpEventAction() 
{
//	delete frunAction;
}

void OpEventAction::BeginOfEventAction(const G4Event*)
{
	fCount = 0;
	fCount_Scint = 0;
	fCount_air = 0;

	trackID.clear();
	trackID_scint.clear();
	trackID_air.clear();
}

void OpEventAction::EndOfEventAction(const G4Event* evt)
{
//	std::cout << "EventAction::GetCount "<< GetCount() << std::endl;
//	std::cout << "EventAction::GetCount "<< GetCount() << std::endl;
//	std::cout << "EventAction::GetCount_i "<< GetCount_i() << std::endl;

	frunAction -> fillcount(fCount, SiPM);
	frunAction -> fillcount(fCount_Scint, Scintillator);
	frunAction -> fillcount(fCount_air,Air);
	fCount = 0;
	fCount_Scint = 0;
	fCount_air = 0;
	frunAction -> update();
	frunAction -> clear();

}

void OpEventAction::photon_count(G4int ID, G4int OPT)
{
	if(OPT == SiPM){
		fCount++;
	}else if (OPT == Air){
		if(!Exist(trackID_air,ID)){
			trackID_air.push_back(ID);
			fCount_air++;
		}
	}else if (OPT == Scintillator){
		fCount_Scint++;
	}else{
		return;
	}
}

G4int OpEventAction::GetCount(G4int OPT)
{
	if(OPT == SiPM){
		return fCount;
	}else if (OPT == Air){
		return fCount_air;
	}else if (OPT == Scintillator){
		return fCount_Scint;
	}else{
		return 0;
	}
}

void OpEventAction::fillPtcs(G4PrimaryVertex* vtx, G4PrimaryParticle* ptc)
{
}

bool OpEventAction::Exist(vector<G4int> list, int id)
{
	if(std::find(list.begin(),list.end(),id) != list.end()) return true;
	else return false;
}
