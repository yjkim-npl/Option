#include "OpRunManager.hh"
#include "OpPrimarayGeneratorAction.hh"
#include "OpEventAction.hh"
#include "OpSteppingAction.hh"

#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "globals.hh"

#include "TObjString.h"
#include "TSystem.h"

OpRunManager::OpRunManager()
	:G4RunManager()
{}

OpRunManager::~OpRunManager()
{}

void OpRunManager::Initialize()
{
	if(GetUserPrimaryGeneratorAction() == nullptr) 	SetUserAction(new OpPrimaryGeneratorAction());
	if(GetUserEventAction() == nullptr)				SetUserAction(new OpEventAction(this));
	if(GetUserSteppingAction() == nullptr)			SetUserAction(new OpSteppingAction(this));

//	G4String SDNames = 
	G4RunManager::Initialize();

	TString filename = "Option.root";
	SetOutputFile(filename);

	auto procName = G4ProcessTable::GetProcessTable() -> GetNameList();
	G4cout << "####################" << G4endl;
	G4cout << "Create Process Table" << G4endl;
	for(auto name:*procNames)
	{
		G4cout<< name << G4endl;
	}
}

void OpRunManager::Run(G4int argc, char**argv, const G4String &type)
{
	G4UImanager* uiman = G4UImanager::GetUIpointer();
	TString command("/control/execute ");

	G4String filename = "vis.mac";
	G4VisManager* visman = new G4VisExecutive;
	visman -> Initialize();
	G4UIExecutive* uiexc = new G4UIExecutive(argc,argv,type);
	G4cout << "Initializing Geant4 run with viewr macro" << G4endl;
	uiman -> ApplyCommand(command+filename);
	uiexe -> SessionStart();

	delete uiexe;
	delete visman;

//	WriteToFile(fProcessTable);
	EndOfRun();
}

void OpRunManager::BeamOn(G4int numEvents, const char* macroFile, G4int numSelect)
{
	if(numEvents <=0) {fakeRun = true;}

