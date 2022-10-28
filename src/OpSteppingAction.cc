#include "OpSteppingAction.hh"

#include "OpDetectorConstruction.hh"
#include "OpEventAction.hh"
#include "OpRunAction.hh"
//#include "OpPhotonDetSD.hh"
//#include "OpUserTrackInformation.hh"

#include "G4ios.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpticalPhoton.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Run.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4UImanager.hh"
#include "G4VPhysicalVolume.hh"


OpSteppingAction::OpSteppingAction(OpEventAction* event, OpRunAction* run)
  : fEventAction(event),
	fRunAction(run)
{
}
OpSteppingAction::~OpSteppingAction()
{}


void OpSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();
  G4ParticleDefinition* particle = track -> GetDefinition();

  G4StepPoint* prevPoint  = step->GetPreStepPoint();
  G4Material* prevMaterial = prevPoint -> GetMaterial();
	  
  G4StepPoint* postPoint = step->GetPostStepPoint();
  G4Material* postMaterial = postPoint -> GetMaterial();
  if(particle != G4OpticalPhoton::OpticalPhotonDefinition()) return;

  if(postMaterial->GetName().find("Silicon") != std::string::npos)
  {
//	if(prevMaterial->GetName().find("Polystyrene") != std::string::npos)
	if(prevMaterial->GetName().find("Glass") != std::string::npos)
	{
		fEventAction -> photon_count();
	  // wavlen
	  G4double energy = track -> GetTotalEnergy();
	  G4double wav = ETowav(energy);
	  fRunAction -> fillWavlen(wav);

	  // time
	  G4double time = postPoint -> GetGlobalTime();
	  fRunAction -> fillTime(time);

	  // edep

	  // DetID
	  G4int detID = postPoint -> GetPhysicalVolume() -> GetCopyNo();
	  fRunAction -> fillDetID(detID);

	  // final position : Vxyz
	  G4ThreeVector pos = postPoint -> GetPosition();
	  fRunAction -> fillVxyz(pos.x(),pos.y(),pos.z());
	  // initial & final E : E_if
	  // initial momentum Pi
	  // final momentum Pf
	  G4ThreeVector mom = track -> GetMomentum();
	  fRunAction -> fillPf(mom.x(),mom.y(),mom.z());
	}
  }


//		 std::cout << "optical photon reached at Si" << std::endl;
//  G4int pdgID = particle -> GetPDGEncoding();
//  OpUserTrackInformation* trackInformation =
//    (OpUserTrackInformation*) track->GetUserInformation();


//  G4VPhysicalVolume* prePV  = prevPoint->GetPhysicalVolume();
//  G4LogicalVolume* preLV    = prePV -> GetLogicalVolume();
//  G4VPhysicalVolume* postPV = postPoint->GetPhysicalVolume();
//  G4LogicalVolume* postLV   = postPV -> GetLogicalVolume();
//  G4TouchableHandle theTouchable = prevPoint -> GetTouchableHandle();
//
//  G4String prePVname  = " ";
//  G4String postPVname = " ";
//
//  if(postPoint -> GetStepStatus() == fWorldBoundary)
//  {
//	  leak_E = track -> GetTotalEnergy();
//	  G4double px = track -> GetMomentum().x();
//	  G4double py = track -> GetMomentum().y();
//	  G4double pz = track -> GetMomentum().z();
//	  leak_p = make_tuple(px,py,pz);
//	  G4double vx = prevPoint -> GetPosition().x();
//	  G4double vy = prevPoint -> GetPosition().y();
//	  G4double vz = prevPoint -> GetPosition().z();
//	  leak_v = make_tuple(vx,vy,vz);

//	  fEventAction -> fillLeaks(leak_E,leak_p,leak_v);
//  }
// G4String matName = preVol -> GetMaterial() -> GetName();
//  if(matName == "G4_Galactic" || matName == "Air") return;
//
//  if(postPV)
//  {
//    prePVname  = prePV->GetName();
//    postPVname = postPV->GetName();
//  }
//
//  // Recording data for start
//  // static const G4ThreeVector ZHat = G4ThreeVector(0.0,0.0,1.0);
//  if(track->GetParentID() == 0)
//  {
//    // This is a primary track
//    if(track->GetCurrentStepNumber() == 1)
//    {
//      //        G4double x  = track->GetVertexPosition().x();
//      //        G4double y  = track->GetVertexPosition().y();
//      //        G4double z  = track->GetVertexPosition().z();
//      //        G4double pz = track->GetVertexMomentumDirection().z();
//      //        G4double fInitTheta =
//      //                         track->GetVertexMomentumDirection().angle(ZHat);
//    }
//  }
//
//  // Retrieve the status of the photon
//  G4OpBoundaryProcessStatus theStatus = Undefined;
//
//  static G4ThreadLocal G4ProcessManager* OpManager =
//    G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
//
//  if(OpManager)
//  {
//    G4int nproc = OpManager->GetPostStepProcessVector()->entries();
//    G4ProcessVector* fPostStepDoItVector =
//      OpManager->GetPostStepProcessVector(typeDoIt);
//
//    for(G4int i = 0; i < nproc; ++i)
//    {
//      G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
//      fOpProcess = dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
//      if(fOpProcess)
//      {
//        theStatus = fOpProcess->GetStatus();
//        break;
//      }
//    }
//  }
//
//  // Find the skewness of the ray at first change of boundary
//  if(fInitGamma == -1 &&
//     (theStatus == TotalInternalReflection || theStatus == FresnelReflection ||
//      theStatus == FresnelRefraction) &&
//     trackInformation->IsStatus(InsideOfFiber))
//  {
//    G4double px = track->GetVertexMomentumDirection().x();
//    G4double py = track->GetVertexMomentumDirection().y();
//    G4double x  = track->GetPosition().x();
//    G4double y  = track->GetPosition().y();
//
//    fInitGamma = x * px + y * py;
//
//    fInitGamma =
//      fInitGamma / std::sqrt(px * px + py * py) / std::sqrt(x * x + y * y);
//
//    fInitGamma = std::acos(fInitGamma * rad);
//
//    if(fInitGamma / deg > 90.0)
//    {
//      fInitGamma = 180 * deg - fInitGamma;
//    }
//  }
//  // Record Photons that missed the photon detector but escaped from readout
//  if(!postPV && trackInformation->IsStatus(EscapedFromReadOut))
//  {
//    G4cout << "SteppingAction: status = EscapedFromReadOut" << G4endl;
//    fEventAction->AddEscaped();
//    // UpdateHistogramSuccess(postPoint,track);
//    ResetCounters();
//
//    return;
//  }
//
//  // Assumed photons are originated at the fiber OR
//  // the fiber is the first material the photon hits
//  switch(theStatus)
//  {
//    // Exiting the fiber
//    case FresnelRefraction:
//    case SameMaterial:
//      fEventAction->AddExiting();
//
//      if(postPVname == "OpFiber" || postPVname == "Clad1" ||
//         postPVname == "Clad2")
//      {
//        if(trackInformation->IsStatus(OutsideOfFiber))
//          trackInformation->AddStatusFlag(InsideOfFiber);
//
//        // Set the Exit flag when the photon refracted out of the fiber
//      }
//      else if(trackInformation->IsStatus(InsideOfFiber))
//      {
//        // EscapedFromReadOut if the z position is the same as fiber's end
//        if(track->GetPosition().z() == fDetector->GetOpFiberEnd())
//        {
//          trackInformation->AddStatusFlag(EscapedFromReadOut);
//          fCounterEnd++;
//          fEventAction->AddEscapedEnd();
//        }
//        else  // Escaped from side
//        {
//          trackInformation->AddStatusFlag(EscapedFromSide);
//          trackInformation->SetExitPosition(track->GetPosition());
//          //  UpdateHistogramEscape(postPoint,track);
//
//          fCounterMid++;
//          fEventAction->AddEscapedMid();
//          ResetCounters();
//        }
//
//        trackInformation->AddStatusFlag(OutsideOfFiber);
//        trackInformation->SetExitPosition(track->GetPosition());
//      }
//
//      return;
//
//    // Internal Reflections
//    case TotalInternalReflection:
//
//      fEventAction->AddTIR();
//
//      // Kill the track if it's number of bounces exceeded the limit
//      if(fBounceLimit > 0 && fCounterBounce >= fBounceLimit)
//      {
//        track->SetTrackStatus(fStopAndKill);
//        trackInformation->AddStatusFlag(murderee);
//        ResetCounters();
//        G4cout << "\n Bounce Limit Exceeded" << G4endl;
//        return;
//      }
//      break;
//
//    case FresnelReflection:
//
//      fCounterBounce++;
//      fEventAction->AddBounce();
//
//      if(prePVname == "OpFiber")
//      {
//        fCounterOpBounce++;
//        fEventAction->AddOpBounce();
//      }
//      else if(prePVname == "Clad1")
//      {
//        fCounterClad1Bounce++;
//        fEventAction->AddClad1Bounce();
//      }
//      else if(prePVname == "Clad2")
//      {
//        fCounterClad2Bounce++;
//        fEventAction->AddClad1Bounce();
//      }
//
//      // Determine if the photon has reflected off the read-out end
//      if(track->GetPosition().z() == fDetector->GetOpFiberEnd())
//      {
//        if(!trackInformation->IsStatus(ReflectedAtReadOut) &&
//           trackInformation->IsStatus(InsideOfFiber))
//        {
//          trackInformation->AddStatusFlag(ReflectedAtReadOut);
//
//          if(fDetector->IsPerfectFiber() &&
//             theStatus == TotalInternalReflection)
//          {
//            track->SetTrackStatus(fStopAndKill);
//            trackInformation->AddStatusFlag(murderee);
//            // UpdateHistogramReflect(postPoint,track);
//            ResetCounters();
//            return;
//          }
//        }
//      }
//      return;
//
//    // Reflection of the mirror
//    case LambertianReflection:
//    case LobeReflection:
//    case SpikeReflection:
//
//      fEventAction->AddReflected();
//      // Check if it hits the mirror
//      if(postPVname == "Mirror")
//      {
//        trackInformation->AddStatusFlag(ReflectedAtMirror);
//        fEventAction->AddMirror();
//      }
//      return;
//
//    // Detected by a detector
//    case Detection:
//
//      // Check if the photon hits the detector and process the hit if it does
//      if(postPVname == "PhotonDet")
//      {
//        // G4cout << "Detection" << G4endl;
//        fEventAction->AddDetected();
//        G4SDManager* SDman = G4SDManager::GetSDMpointer();
//        G4String SDname    = "Op/PhotonDet";
//        OpPhotonDetSD* mppcSD =
//          (OpPhotonDetSD*) SDman->FindSensitiveDetector(SDname);
//
//        if(mppcSD)
//          mppcSD->ProcessHits_boundary(step, nullptr);
//
//        // Record Photons that escaped at the end
//        // if (trackInformation->IsStatus(EscapedFromReadOut))
//        //                    UpdateHistogramSuccess(postPoint,track);
//
//        // Stop Tracking when it hits the detector's surface
//        ResetCounters();
//        track->SetTrackStatus(fStopAndKill);
//        return;
//      }
//      break;
//
//    default:
//      break;
//  }
//
//  // Check for absorbed photons
//  if(track->GetTrackStatus() != fAlive &&
//     trackInformation->IsStatus(InsideOfFiber))
//  {
//    // UpdateHistogramAbsorb(postPoint,track);
//    ResetCounters();
//    return;
//  }
}
