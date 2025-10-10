#include <iostream>
#include <fstream>
#include <string>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4NuclideTable.hh"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"

int main(int argc, char **argv)
{
    
    std::ofstream logfile("run.log");
    G4cout.rdbuf(logfile.rdbuf());


    G4UIExecutive *ui = nullptr;
    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    auto *runManager = new G4RunManager;

    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new ActionInitialization());

    runManager->Initialize();

    G4VisManager *visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    G4UImanager *uiManager = G4UImanager::GetUIpointer();

    if (argc == 2)
    {
        int particle_count = std::stoi(argv[1]);
        std::cout << "Running in batch mode with " << particle_count << " particles..." << std::endl;

        uiManager->ApplyCommand("/control/execute run_particle.mac");

        G4String beamCmd = "/run/beamOn " + std::to_string(particle_count);
        uiManager->ApplyCommand(beamCmd);
    }
    else
    {
        uiManager->ApplyCommand("/control/execute run.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;

    return 0;
}
