#! /usr/bin/perl

use Cwd;
$maindir = getcwd();

@files_i = 
(
"WLSActionInitialization.",
"WLSDetectorConstruction.",
"WLSDetectorMessenger.",
"WLSEventAction.",
"WLSEventActionMessenger.",
"WLSMaterials.",
"WLSPhotonDetHit.",
"WLSPhotonDetSD.",
"WLSPrimaryGeneratorAction.",
"WLSPrimaryGeneratorMessenger.",
"WLSRun.",
"WLSRunAction.",
"WLSStackingAction.",
"WLSSteppingAction.",
"WLSSteppingActionMessenger.",
"WLSTrackingAction.",
"WLSTrajectory.",
"WLSTrajectoryPoint.",
"WLSUserTrackInformation.",
);
$nfiles = @files_i;
@files_f = 
(
"OpActionInitialization.",
"OpDetectorConstruction.",
"OpDetectorMessenger.",
"OpEventAction.",
"OpEventActionMessenger.",
"OpMaterials.",
"OpPhotonDetHit.",
"OpPhotonDetSD.",
"OpPrimaryGeneratorAction.",
"OpPrimaryGeneratorMessenger.",
"OpRun.",
"OpRunAction.",
"OpStackingAction.",
"OpSteppingAction.",
"OpSteppingActionMessenger.",
"OpTrackingAction.",
"OpTrajectory.",
"OpTrajectoryPoint.",
"OpUserTrackInformation.",
);

for ($a=0; $a<$nfiles; $a++)
{
	$path_file_i = "src/$files_i[$a]cc";
	$path_file_i_d = "include/$files_i[$a]hh";
	$path_file_f = "src/$files_f[$a]cc";
	$path_file_f_d = "include/$files_f[$a]hh";

	$cmd1 = "mv $path_file_i $path_file_f";
	system $cmd1;
	$cmd2 = "mv $path_file_i_d $path_file_f_d";
	system $cmd2;
}


