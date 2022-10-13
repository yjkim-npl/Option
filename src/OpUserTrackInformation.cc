#include "OpUserTrackInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpUserTrackInformation::OpUserTrackInformation() { fStatus = undefined; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpUserTrackInformation::~OpUserTrackInformation() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool OpUserTrackInformation::AddStatusFlag(TrackStatus s)
// Try adding a status flag and return if it is successful or not
// Cannot Add Undefine or a flag that conflicts with another flag
// Return true if the addition of flag is successful, false otherwise
{
  switch(s)
  {
    case left:
    case right:

      // Allow the user to set left or right
      // only if the track is undefined
      if(fStatus == undefined)
        return fStatus |= s;

      return false;

    case EscapedFromSide:
    case EscapedFromReadOut:

      // Allow the user to set escaped flag
      // only if the photon hasn't exited the fiber yet

      if((fStatus == undefined) || (fStatus & OutsideOfFiber))
        return false;

      return fStatus |= s;

    case ReflectedAtMirror:
    case ReflectedAtReadOut:
    case murderee:

      return fStatus |= s;

    case InsideOfFiber:

      return (fStatus = (fStatus & ~(EscapedFromSide + EscapedFromReadOut +
                                     OutsideOfFiber)) |
                        s);

    case OutsideOfFiber:

      return (fStatus = (fStatus & ~InsideOfFiber) | s);

    default:

      return false;
  }
}
