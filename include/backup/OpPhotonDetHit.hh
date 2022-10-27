#ifndef OpPhotonDetHit_h
#define OpPhotonDetHit_h 1

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"
#include "G4VPhysicalVolume.hh"

#include <vector>
#include <tuple>
using namespace std;

class OpPhotonDetHit : public G4VHit
{
	public:
		OpPhotonDetHit();
//		OpPhotonDetHit(G4ThreeVector pExit, G4ThreeVector pArrive, G4double pTime);
		~OpPhotonDetHit();

		OpPhotonDetHit(const OpPhotonDetHit& right);
		const OpPhotonDetHit& operator=(const OpPhotonDetHit& right);

		G4bool operator==(const OpPhotonDetHit& right) const;

		inline void* operator new(size_t);
		inline void operator delete(void* aHit);

		void Draw();
		void Print();

		void photonCount() {fPhotons++;}
		G4int GetPhotonCount() {return fPhotons;}

//		void SetOpPhotonDetPos(G4double posX,G4double posY, G4double posZ)
		void SetOpPhotonDetPos(G4ThreeVector opdetpos)
			{fOpPhotonDetPos = opdetpos;}
//			{fOpPhotonDetPos = make_tuple(posX,posY,posZ);}
//		tuple<G4double,G4double,G4double> GetOpPhotonDetPos() {return fOpPhotonDetPos;}
		G4ThreeVector GetOpPhotonDetPos() {return fOpPhotonDetPos;}

		void SetOpPhotonDetID(G4int ID) {fOpPhotonDetID = ID;}
		G4int GetOpPhotonDetID() {return fOpPhotonDetID;}

		void SetOpPhotonPos(G4double x, G4double y, G4double z)
		{
			fPhotonPos.push_back(make_tuple(x,y,z));
		}
		tuple<G4double,G4double,G4double> GetOpPhotonPos(G4int n) {return fPhotonPos[n];}

		void SetWavlen(G4double wavlen) {fWavlen.push_back(wavlen);}
		G4double GetWavlen(G4int n) {return fWavlen[n];}

		void SetTime(G4double time) {fTime.push_back(time);}
		G4double GetTime(G4int n) {return fTime[n];}
	

	private:
		int fPhotons;
//		tuple<G4double,G4double,G4double> fOpPhotonDetPos;
		G4ThreeVector fOpPhotonDetPos;
		G4int fOpPhotonDetID;
		vector<tuple<G4double,G4double,G4double>> fPhotonPos;	// temporary
		vector<G4double> fWavlen;
		vector<G4double> fTime;
};

//--------------------------------------------------
// Type Definitions
//--------------------------------------------------

typedef G4THitsCollection<OpPhotonDetHit> OpPhotonDetHitsCollection;

extern G4ThreadLocal G4Allocator<OpPhotonDetHit>* OpPhotonDetHitAllocator;

//--------------------------------------------------
// Operator Overloads
//--------------------------------------------------

inline void* OpPhotonDetHit::operator new(size_t)
{
  if(!OpPhotonDetHitAllocator)
    OpPhotonDetHitAllocator = new G4Allocator<OpPhotonDetHit>;
  return (void*) OpPhotonDetHitAllocator->MallocSingle();
}

inline void OpPhotonDetHit::operator delete(void* aHit)
{
  OpPhotonDetHitAllocator->FreeSingle((OpPhotonDetHit*) aHit);
}

#endif
