#include "OpDetectorConstruction.hh"
#include "OpEventAction.hh"

//#include "OpPhotonDetHit.hh"
//#include "OpRun.hh"
#include "OpRunAction.hh"
//#include "OpTrajectory.hh"

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
	fCount_i = 0;
}

OpEventAction::~OpEventAction() 
{
}

void OpEventAction::BeginOfEventAction(const G4Event*)
{
}

void OpEventAction::EndOfEventAction(const G4Event* evt)
{
	std::cout << "EventAction::GetCount "<< GetCount() << std::endl;
	std::cout << "EventAction::GetCount_i "<< GetCount_i() << std::endl;
//	// Get Hits from the detector if any
//	G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
//	if(!HCE)
//	{
//		G4ExceptionDescription msg;
//		msg << "No hits collection of this event found." << G4endl;
//		G4Exception ("OpEventAction::EndOfEventAction()","OptionCode001",JustWarning,msg);
//		return;
//	}
//	G4int totSDNum = HCE -> GetNumberOfCollections();
//	std::cout << totSDNum << std::endl;
//
//	for(int a=0;a<totSDNum; a++)
//	{
//		OpPhotonDetHitsCollection* HC = 0;
////		if(HCE)
////		{
////			G4cout << "yjkim " << fCollID[a] << G4endl;	// to check fCollID
////			if(fCollID[a]>=0) HC = (OpPhotonDetHitsCollection*)(HCE->GetHC(fCollID[a]));
////		}
//
//		if(HC)
//		{
//			G4int MPPCs = HC -> entries();
//			for(G4int b=0; b<MPPCs; b++)
//			{
//				fillHits((*HC)[b]);
//			}
//		}
//	}
//
//	for(int a=0; a<evt->GetNumberOfPrimaryVertex(); a++)
//	{
//		G4PrimaryVertex* vtx = evt->GetPrimaryVertex(a);
//		for(int b=0; b<vtx->GetNumberOfParticle(); b++)
//		{
//			G4PrimaryParticle* ptc = vtx -> GetPrimary(b);
//			fillPtcs(vtx,ptc);
//		}
//	}
////	evtNum = OpPrimaryGeneratorAction::Evt;
//
//	G4SDManager* SDman = G4SDManager::GetSDMpointer();
//	G4String colName   = "PhotonDetHitCollection";
//	fMPPCCollID        = SDman->GetCollectionID(colName);
//
//  OpPhotonDetHitsCollection* mppcHC = nullptr;
//
//  // Get the hit collections
//  if(HCE)
//  {
//    if(fMPPCCollID >= 0)
//    {
//      mppcHC = (OpPhotonDetHitsCollection*) (HCE->GetHC(fMPPCCollID));
//    }
//  }
//
//  // Get hit information about photons that reached the detector in this event
//  G4int n_hit = 0;
//  if(mppcHC)
//  {
//    n_hit = mppcHC->entries();
//  }


	frunAction -> fillcount(fCount);
	frunAction -> fillcount_i(fCount_i);
	fCount = 0;
	fCount_i = 0;
	frunAction -> update();
	frunAction -> clear();

}

//void OpEventAction::fillHits(OpPhotonDetHit* hit)
//{
//	fCount += hit->GetPhotonCount();
//	fPhotonDetID.push_back(hit->GetOpPhotonDetID());
//	fPhotonDetPos.push_back(hit->GetOpPhotonDetPos());
//}
void OpEventAction::fillPtcs(G4PrimaryVertex* vtx, G4PrimaryParticle* ptc)
{
//	Gen_E.push_back(ptc -> GetTotalEnergy());
//	Gen_p.push_back(make_tuple(ptc->GetPx(), ptc->GetPy(), ptc->GetPz()));
//	Gen_v.push_back(make_tuple(vtx->GetX0(), vtx->GetY0(), vtx->GetZ0()));
//	Gen_t.push_back(vtx->GetT0());
//	Gen_pdgID.push_back(ptc->GetPDGcode()); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//G4int OpEventAction::GetEventNo()
//{
//  return fpEventManager->GetConstCurrentEvent()->GetEventID();
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//void OpEventAction::SetEventVerbose(G4int level) { fVerboseLevel = level; }
