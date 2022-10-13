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
