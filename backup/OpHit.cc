#include "OpHit.hh"

G4ThreadLocal G4Allocator<OpHit>* OpHitAllocator = 0;

OpHit::OpHit()
: G4VHit(),
  fPhotons(0)
{}

OpHit::OpHit(const OpHit &right)
: G4VHit()
{
	fPhotons = right.fPhotons;
	return *this;
}

OpHit::~OpHit()
{}

const OpHit& OpHit::operator=(const OpHit &right)
{
	fPhotons = right.fPhotons;
	return this;
}

G4bool OpHit::operator==(const OpHit &right) const
{
	return (fPhotons == right.fPhotons);
}
