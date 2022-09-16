#ifndef OPDETECTORCONSTRUCTION_HH
#define OPDETECTORCONSTRUCTION_HH 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class OpDetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		OpDetectorConstruction();
		virtual ~OpDetectorConstruction();

		virtual G4VPhysicalVolume* Construct();
};
#endif
