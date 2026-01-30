#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "DetectorConstruction.hh"

DetectorConstruction :: DetectorConstruction() :G4VUserDetectorConstruction()
{

}

DetectorConstruction :: ~DetectorConstruction()
{

}

G4VPhysicalVolume* DetectorConstruction ::Construct()
{
    G4NistManager* nist = G4NistManager::Instance();
    auto *WorldMat = nist ->FindOrBuildMaterial("G4_AIR");
    auto *NiMat = nist ->FindOrBuildMaterial("G4_Ni");
    auto *SiMat = nist ->FindOrBuildElement("Si");
    auto *CMat = nist -> FindOrBuildElement("C");

    //SIC matrial
    G4double density = 3.21 * g/cm3;
    G4int ncomponents = 2;
    G4int natoms;
    G4Material *Sic = new G4Material("SiliconCarbide", density, ncomponents);
    Sic ->AddElement(SiMat, natoms=1);
    Sic ->AddElement(CMat,natoms=1);

    //World
    G4double World_X = 10*cm;
    G4double World_Y = 10*cm;
    G4double World_Z = 10*cm;

    G4Box* SolidWolrd  = new G4Box("World",World_X/2,World_Y/2,World_Z/2);
    G4LogicalVolume* G4LogicalWorld = new G4LogicalVolume(SolidWolrd,WorldMat,"LogicalWorld");
    G4VPhysicalVolume*G4VPhysicalVolume = new G4PVPlacement(0,G4ThreeVector(0,0,0),"PhysicalWorld",G4LogicalWorld,NULL,false,0,true);

    //Ni
    G4double Ni_X = 1*cm;
    G4double Ni_Y = 1*cm;
    G4double Ni_Z = 0.143*um;















}