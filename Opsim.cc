#include "OpDetectorConstruction.hh"
#include "OpActionInitialization.hh"
#include "OpParameterContainer.hh"

#include "G4RunManagerFactory.hh"
#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4OpticalPhysics.hh"
#include "FTFP_BERT.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"


int main(int argc,char** argv)
{
  G4UIExecutive* ui = 0;
  if ( argc <= 2 ) {
    ui = new G4UIExecutive(argc, argv);
  }
  G4String parName = "OpParameter.conf";
  if(argc >2) parName = argv[1];
  G4int seed = 0;
  G4String filename;
  if(argc >3)  	seed = atoi(argv[3]);
  if(argc >4) 	filename = argv[4];
  G4RunManager* runManager = new G4RunManager;

  OpParameterContainer* parC = new OpParameterContainer(parName);

  runManager->SetUserInitialization(new OpDetectorConstruction(parC));

  // Physics list
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  opticalPhysics -> Configure(kCerenkov, true);
  opticalPhysics -> Configure(kScintillation, true);
//  opticalPhysics -> SetTrackSecondariesFirst(kCerenkov, true);
//  opticalPhysics -> SetTrackSecondariesFirst(kScintillation,true);
  physicsList -> RegisterPhysics(opticalPhysics);
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserInitialization(new OpActionInitialization(parC));
  
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if ( argc != 1 ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = parC -> GetParString("geant_mac");;
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }
  
  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
