#include "PhysicsList.hh"

#include "G4EmLivermorePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"

#include "G4ProductionCutsTable.hh"
#include "G4SystemOfUnits.hh"

PhysicsList ::PhysicsList()
{
    RegisterPhysics(new G4EmLivermorePhysics());

    RegisterPhysics(new G4EmExtraPhysics());

    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());

    RegisterPhysics(new G4StoppingPhysics());

    RegisterPhysics(new G4IonPhysics());

    RegisterPhysics(new G4NeutronTrackingCut());

    SetCutValue(0.01 * um, "e-");
    SetCutValue(0.1 * um, "e+");
    SetCutValue(0.1 * um, "gamma");
    SetCutValue(0.1 * um, "proton");
}

PhysicsList ::~PhysicsList()
{

}