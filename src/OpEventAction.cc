#include "OpEventAction.hh"

#include "OpPhotonDetHit.hh"
#include "OpRun.hh"
#include "OpRunAction.hh"
#include "OpTrajectory.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VVisManager.hh"
#include "Randomize.hh"

// Purpose: Accumulates statistics regarding hits
//          in the PhotonDet detector

OpEventAction::OpEventAction() : G4UserEventAction()
{
	G4RunManager::GetRunManager() -> SetPrintProgress(1);
}

OpEventAction::~OpEventAction() { }

void OpEventAction::BeginOfEventAction(const G4Event*)
{
	clear();
	G4SDManager* sdManager = G4SDmanager::GetSDMpointer();
	for(int a=0; a<OpDetectorConstruction::NofY; a++)
	{
		fCollID.push_back(sdManager->GetCollectionID("MPPCleft_"+std::to_string(a)));
		fCollID.push_back(sdManager->GetCollectionID("MPPCright_"+std::to_string(a)));
	}
//	fEventData = 

//  fNTIR        = 0;
//  fNExiting    = 0;
//  fEscapedEnd  = 0;
//  fEscapedMid  = 0;
//  fBounce      = 0;
//  fOpBounce   = 0;
//  fClad1Bounce = 0;
//  fClad2Bounce = 0;
//  fReflected   = 0;
//  fDetected    = 0;
//  fEscaped     = 0;
//  fMirror      = 0;
}

void OpEventAction::EndOfEventAction(const G4Event* evt)
{
	// Get Hits from the detector if any
	G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
	if(!HCE)
	{
		G4ExceptionDescription msg;
		msg << "No hits collection of this event found." << G4endl;
		G4Exception ("OpEventAction::EndOfEventAction()","OptionCode001",JustWarning,msg);
		return;
	}
	G4int totSDNum = HCE -> GetNumberOfCollections();

	for(int a=0;a<totSDNum; a++)
	{
		OpPhotonDetHitCollection* HC = 0;
		if(HCE)
		{
			G4cout << fCollID[a] << G4endl;	// to check fCollID
			if(fCollID[a]>=0) HC = (OpPhotonDetHitCollection*)(HCE->GetHC(fCollID[a]));
		}

		if(HC)
		{
			G4int MPPCs = HC -> entries();
			for(G4int b=0; b<MPPCs; b++)
			{
				fillHits((*HC)[b]);
			}
		}
	}

	for(int a=0; a<evt->GetNumberOfPrimaryVertex(); a++)
	{
		G4PrimaryVertex* vtx = evt->GetPrimaryVertex(a);
		for(int b=0; b<vtx->GetNumberOfParticle(); b++)
		{
			G4PrimaryParticle* ptc = vtx -> GetPrimary(b);
			fillPtcs(vtx,ptc);
		}
	}
//	evtNum = OpPrimaryGeneratorAction::Evt;

	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	G4String colName   = "PhotonDetHitCollection";
	fMPPCCollID        = SDman->GetCollectionID(colName);

  OpPhotonDetHitsCollection* mppcHC = nullptr;

  // Get the hit collections
  if(HCE)
  {
    if(fMPPCCollID >= 0)
    {
      mppcHC = (OpPhotonDetHitsCollection*) (HCE->GetHC(fMPPCCollID));
    }
  }

  // Get hit information about photons that reached the detector in this event
  G4int n_hit = 0;
  if(mppcHC)
  {
    n_hit = mppcHC->entries();
  }

  if(fVerboseLevel > 1)
  {
    G4cout << "-------------------------------------" << G4endl
           << " In this event, number of:" << G4endl
           << "  TIR:           " << fNTIR << G4endl
           << "  Exiting:       " << fNExiting << G4endl
           << "  Escaped Mid:   " << fEscapedMid << G4endl
           << "  Escaped End:   " << fEscapedEnd << G4endl
           << "  Bounced:       " << fBounce << G4endl
           << "  Op Bounce:    " << fOpBounce << G4endl
           << "  Clad1 Bounce:  " << fClad1Bounce << G4endl
           << "  Clad2 Bounce:  " << fClad2Bounce << G4endl
           << "  Reflected:     " << fReflected << G4endl
           << "  Detected:      " << fDetected << G4endl
           << "  Escaped:       " << fEscaped << G4endl
           << "  Mirror:        " << fMirror << G4endl
           << "  Detector hit:  " << n_hit << G4endl;
  }

  OpRun* run = static_cast<OpRun*>(
    G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  run->AddTIR(fNTIR);
  run->AddExiting(fNExiting);
  run->AddEscapedEnd(fEscapedEnd);
  run->AddEscapedMid(fEscapedMid);
  run->AddBounce(fBounce);
  run->AddClad1Bounce(fClad1Bounce);
  run->AddClad2Bounce(fClad2Bounce);
  run->AddReflected(fReflected);
  run->AddEscaped(fEscaped);
  run->AddMirror(fMirror);
  run->AddDetectorHits(n_hit);
}

void fillHits(OpPhotonDetHit* hit)
{
	count.push_back(hit->GetPhotonCount());
	fPhotonDetID.push_back(hit->GetOpPhotonDetID());
	fPhotonDetPos.push_back(hit->GetOpPhotonDetPos());
}
void fillPtcs(G4PrimaryVertex* vtx, G4PrimaryParticle* ptc)
{
	Gen_E.push_back(ptc -> GetTotalEnergy());
	Gen_p.push_back(make_tuple(ptc->GetPx(), ptc->GetPy(), ptc->GetPz()));
	Gen_v.push_back(make_tuple(vtx->GetX0(), vtx->GetY0(), vtx->GetZ0()));
	Gen_t.push_back(vtx->GetT0());
	Gen_pdgID.push_back(ptc->GenPDGcode()); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int OpEventAction::GetEventNo()
{
  return fpEventManager->GetConstCurrentEvent()->GetEventID();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpEventAction::SetEventVerbose(G4int level) { fVerboseLevel = level; }
