#ifndef OpHit_h
#define OpHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class OpHit : public G4VHit
{
	public:
		OpHit();
		OpHit(const OpHit &right);
		virtual ~OpHit();

		const OpHit& operator==(const OpHit &right);
		G4bool operator == (constOpHit &right) const;

		inline void *operator new(size_t);
		inline void operator delete(void* aHit);

		void photonCount() { fPhotons++;}
		G4int GetPhotonCount() const { return fPhotons;}

	private:
		G4int fPhotons;
};

typedef G4THitsCollection<OpHit> OpHitsCollection;
extern G4ThreadLocal G4Allocator<OpHit>* OpHitAllocator;

inline void* OpHit::operator new(size_t)
{
	if(!OpHitAllocator) OpHitAllocator = new G4Allocator<OpHit>;
	return (void*)OpHitAllocator->MallocSingle();
}
inline void OpHit::operator delete(void* aHit)
{
	OpHitAllocator -> FreeSingle((OpHit*)aHit);
}
#endif
