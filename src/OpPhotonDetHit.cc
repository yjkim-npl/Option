#include "OpPhotonDetHit.hh"

G4ThreadLocal G4Allocator<OpPhotonDetHit>* OpPhotonDetHitAllocator = nullptr;

OpPhotonDetHit::OpPhotonDetHit()
 :G4VHit(),fPhotons(0),fOpPhotonDetPos(G4ThreeVector(0,0,0)),fOpPhotonDetID(0)
{}

OpPhotonDetHit::~OpPhotonDetHit() {}

OpPhotonDetHit::OpPhotonDetHit(const OpPhotonDetHit& right)
  : G4VHit()
{*this = right;}

const OpPhotonDetHit& OpPhotonDetHit::operator=(const OpPhotonDetHit& right)
{
	fPhotons = right.fPhotons;
	fOpPhotonDetPos = right.fOpPhotonDetPos;
	fOpPhotonDetID = right.fOpPhotonDetID;
	fPhotonPos = right.fPhotonPos;
	fWavlen = right.fWavlen;
	fTime = right.fTime;
	return *this;
}

G4bool OpPhotonDetHit::operator==(const OpPhotonDetHit& right) const
{
  return fOpPhotonDetPos == right.fOpPhotonDetPos &&
	     fOpPhotonDetID  == right.fOpPhotonDetID;
}

void OpPhotonDetHit::Draw() {}

void OpPhotonDetHit::Print() {}
