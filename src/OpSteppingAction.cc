#include "OpSteppingAction.hh"

#include "OpDetectorConstruction.hh"
#include "OpEventAction.hh"
#include "OpPhotonDetSD.hh"
#include "OpUserTrackInformation.hh"

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

// Purpose: Save relevant information into User Track Information

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpSteppingAction::OpSteppingAction(OpDetectorConstruction* detector,
                                     OpEventAction* event)
  : fDetector(detector)
  , fEventAction(event)
{
	leak_E = 0. *MeV;
//	leak_p = make_tuple(0,0,0)
//
//  fCounterEnd  = 0;
//  fCounterMid  = 0;
//  fBounceLimit = 100000;
//
//  fOpProcess = nullptr;
//
//  ResetCounters();
}


void OpSteppingAction::UserSteppingAction(const G4Step* theStep)
{
  G4Track* theTrack = theStep->GetTrack();
  G4ParticleDefinition* particle = track -> GetDefinition();
  if(particle == G4OpticalPhoton::OpticalPhotonDefinition()) return;
  G4int pdgID = particle -> GetPDGEncoding();
//  OpUserTrackInformation* trackInformation =
//    (OpUserTrackInformation*) theTrack->GetUserInformation();

  G4StepPoint* thePrePoint  = theStep->GetPreStepPoint();
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();

  G4VPhysicalVolume* thePrePV  = thePrePoint->GetPhysicalVolume();
  G4LogicalVolume* thePreLV    = thePrePV -> GetLogicalVolume();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
  G4LogicalVolume* thePostLV   = thePostPV -> GetLogicalVolume();
  G4TouchableHandle theTouchable = thePrePoint -> GetTouchableHandle();

  G4String thePrePVname  = " ";
  G4String thePostPVname = " ";

  if(thePostPoint -> GetStepStatus() == fWorldBoundary)
  {
	  leak_E = theTrack -> GetTotalEnergy();
	  G4double px = theTrack -> GetMomentum().x();
	  G4double py = theTrack -> GetMomentum().y();
	  G4double pz = theTrack -> GetMomentum().z();
	  leak_p = make_tuple(px,py,pz);
	  G4double vx = thePrePoint -> GetPosition().x();
	  G4double vy = thePrePoint -> GetPosition().y();
	  G4double vz = thePrePoint -> GetPosition().z();
	  leak_v = make_tuple(vx,vy,vz);

	  fEventAction -> fillLeaks(leak_E,leak_p,leak_v);
  }
//  G4String matName = preVol -> GetMaterial() -> GetName();
//  if(matName == "G4_Galactic" || matName == "Air") return;
//
//  if(thePostPV)
//  {
//    thePrePVname  = thePrePV->GetName();
//    thePostPVname = thePostPV->GetName();
//  }
//
//  // Recording data for start
//  // static const G4ThreeVector ZHat = G4ThreeVector(0.0,0.0,1.0);
//  if(theTrack->GetParentID() == 0)
//  {
//    // This is a primary track
//    if(theTrack->GetCurrentStepNumber() == 1)
//    {
//      //        G4double x  = theTrack->GetVertexPosition().x();
//      //        G4double y  = theTrack->GetVertexPosition().y();
//      //        G4double z  = theTrack->GetVertexPosition().z();
//      //        G4double pz = theTrack->GetVertexMomentumDirection().z();
//      //        G4double fInitTheta =
//      //                         theTrack->GetVertexMomentumDirection().angle(ZHat);
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
//    G4double px = theTrack->GetVertexMomentumDirection().x();
//    G4double py = theTrack->GetVertexMomentumDirection().y();
//    G4double x  = theTrack->GetPosition().x();
//    G4double y  = theTrack->GetPosition().y();
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
//  if(!thePostPV && trackInformation->IsStatus(EscapedFromReadOut))
//  {
//    G4cout << "SteppingAction: status = EscapedFromReadOut" << G4endl;
//    fEventAction->AddEscaped();
//    // UpdateHistogramSuccess(thePostPoint,theTrack);
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
//      if(thePostPVname == "OpFiber" || thePostPVname == "Clad1" ||
//         thePostPVname == "Clad2")
//      {
//        if(trackInformation->IsStatus(OutsideOfFiber))
//          trackInformation->AddStatusFlag(InsideOfFiber);
//
//        // Set the Exit flag when the photon refracted out of the fiber
//      }
//      else if(trackInformation->IsStatus(InsideOfFiber))
//      {
//        // EscapedFromReadOut if the z position is the same as fiber's end
//        if(theTrack->GetPosition().z() == fDetector->GetOpFiberEnd())
//        {
//          trackInformation->AddStatusFlag(EscapedFromReadOut);
//          fCounterEnd++;
//          fEventAction->AddEscapedEnd();
//        }
//        else  // Escaped from side
//        {
//          trackInformation->AddStatusFlag(EscapedFromSide);
//          trackInformation->SetExitPosition(theTrack->GetPosition());
//          //  UpdateHistogramEscape(thePostPoint,theTrack);
//
//          fCounterMid++;
//          fEventAction->AddEscapedMid();
//          ResetCounters();
//        }
//
//        trackInformation->AddStatusFlag(OutsideOfFiber);
//        trackInformation->SetExitPosition(theTrack->GetPosition());
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
//        theTrack->SetTrackStatus(fStopAndKill);
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
//      if(thePrePVname == "OpFiber")
//      {
//        fCounterOpBounce++;
//        fEventAction->AddOpBounce();
//      }
//      else if(thePrePVname == "Clad1")
//      {
//        fCounterClad1Bounce++;
//        fEventAction->AddClad1Bounce();
//      }
//      else if(thePrePVname == "Clad2")
//      {
//        fCounterClad2Bounce++;
//        fEventAction->AddClad1Bounce();
//      }
//
//      // Determine if the photon has reflected off the read-out end
//      if(theTrack->GetPosition().z() == fDetector->GetOpFiberEnd())
//      {
//        if(!trackInformation->IsStatus(ReflectedAtReadOut) &&
//           trackInformation->IsStatus(InsideOfFiber))
//        {
//          trackInformation->AddStatusFlag(ReflectedAtReadOut);
//
//          if(fDetector->IsPerfectFiber() &&
//             theStatus == TotalInternalReflection)
//          {
//            theTrack->SetTrackStatus(fStopAndKill);
//            trackInformation->AddStatusFlag(murderee);
//            // UpdateHistogramReflect(thePostPoint,theTrack);
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
//      if(thePostPVname == "Mirror")
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
//      if(thePostPVname == "PhotonDet")
//      {
//        // G4cout << "Detection" << G4endl;
//        fEventAction->AddDetected();
//        G4SDManager* SDman = G4SDManager::GetSDMpointer();
//        G4String SDname    = "Op/PhotonDet";
//        OpPhotonDetSD* mppcSD =
//          (OpPhotonDetSD*) SDman->FindSensitiveDetector(SDname);
//
//        if(mppcSD)
//          mppcSD->ProcessHits_boundary(theStep, nullptr);
//
//        // Record Photons that escaped at the end
//        // if (trackInformation->IsStatus(EscapedFromReadOut))
//        //                    UpdateHistogramSuccess(thePostPoint,theTrack);
//
//        // Stop Tracking when it hits the detector's surface
//        ResetCounters();
//        theTrack->SetTrackStatus(fStopAndKill);
//        return;
//      }
//      break;
//
//    default:
//      break;
//  }
//
//  // Check for absorbed photons
//  if(theTrack->GetTrackStatus() != fAlive &&
//     trackInformation->IsStatus(InsideOfFiber))
//  {
//    // UpdateHistogramAbsorb(thePostPoint,theTrack);
//    ResetCounters();
//    return;
//  }
}
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//OpSteppingAction::~OpSteppingAction() {  }
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void OpSteppingAction::SetBounceLimit(G4int i) { fBounceLimit = i; }
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4int OpSteppingAction::GetNumberOfBounces() { return fCounterBounce; }
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4int OpSteppingAction::GetNumberOfClad1Bounces()
//{
//  return fCounterClad1Bounce;
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4int OpSteppingAction::GetNumberOfClad2Bounces()
//{
//  return fCounterClad2Bounce;
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4int OpSteppingAction::GetNumberOfOpBounces() { return fCounterOpBounce; }
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//G4int OpSteppingAction::ResetSuccessCounter()
//{
//  G4int temp  = fCounterEnd;
//  fCounterEnd = 0;
//  return temp;
//}
//
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
