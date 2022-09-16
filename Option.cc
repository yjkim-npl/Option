#include "OpDetectorConstruction.hh"
#include "OpPrimaryGeneratorAction.hh"

#include "G4EmStandardPhysics_option4.hh"
#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4RunManagerFactory.hh"
#include "G4RunManager.hh"

#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

int main(int argc, char** argv)
{
	G4UIExecutive* ui = nullptr;
	if(argc == 1)	// the case of command './Option'
	{
		ui = new G4UIExecutive(argc,argv);
	}
	auto runManager = G4RunManagerFactory::CreateRunManager();

	G4int seed = 123;	
	if(argc > 2)	// the case of command './Option <arguments>'
	{
		seed = atoi(argv[argc-1]);	// the last arguments is considered as seed
	}

	//Set random seed
	G4Random::setTheSeed(seed);

	//Detector construction
	OpDetectorConstruction* detector = new OpDetectorConstruction();
	runManager -> SetUserInitialization(detector);

	//Physics list
	G4VModularPhysicsList* physicsList = new FTFP_BERT;
	physicsList -> ReplacePhysics(new G4EmStandardPhysics_option4());
	G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
	physicsList -> RegisterPhysics(opticalPhysics);
	runManager -> SetUserInitialization(physicsList);

	//User action initialization
//	runManager -> SetUserInitialization(new OpActionInitialization(detector));
	runManager -> SetUserAction(new OpPrimaryGeneratorAction());

	//Initialize visualization
	G4VisManager* visManager = new G4VisExecutive;
	visManager -> Initialize();

	//Get the pointer to the User Interface
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if(ui)
	{
		if(ui->IsGUI())
			UImanager->ApplyCommand("/control/execute vis.mac");
		ui->SessionStart();
		delete ui;
	}else{
		G4String command = "/control/execute ";
		G4String fileName = argv[1];	// the first argument is considered as macro
		UImanager -> ApplyCommand(command + fileName);
	}

	delete visManager;
	delete runManager;
	return 0;
}
