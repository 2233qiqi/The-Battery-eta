#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "DetectorConstruction.hh"
#include "G4VisAttributes.hh"

DetectorConstruction :: DetectorConstruction() :G4VUserDetectorConstruction()
{

}

DetectorConstruction :: ~DetectorConstruction()
{

}

G4VPhysicalVolume* DetectorConstruction ::Construct()
{

    G4bool checkOverlaps = true;

    G4NistManager* nist = G4NistManager::Instance();
    auto *WorldMat = nist ->FindOrBuildMaterial("G4_AIR");
    auto *NiMat = nist ->FindOrBuildMaterial("G4_Ni");
    auto *SiMat = nist ->FindOrBuildElement("Si");
    auto *CMat = nist -> FindOrBuildElement("C");

    //SIC matrial
    G4double density = 3.21 * g/cm3;
    G4int natoms;
    G4Material *Sic = new G4Material("SiliconCarbide", density, 2);
    Sic->AddElement(SiMat, 1); 
    Sic->AddElement(CMat, 1);

    //World
    G4double World_X = 10*cm;
    G4double World_Y = 10*cm;
    G4double World_Z = 10*cm;

    G4Box* SolidWolrd  = new G4Box("World",World_X/2,World_Y/2,World_Z/2);
    G4LogicalVolume* LogicalWorld = new G4LogicalVolume(SolidWolrd,WorldMat,"LogicalWorld");
    G4VPhysicalVolume* PhysicalWorld = new G4PVPlacement(0,G4ThreeVector(0,0,0),LogicalWorld,"PhysicalWorld",NULL,false,0,true);

    //Ni
    G4double Ni_X = 1*cm;
    G4double Ni_Y = 1*cm;
    G4double Ni_Z = 0.5*um;

    G4Box* SolidNi = new G4Box ("Ni",Ni_X/2,Ni_Y/2,Ni_Z/2);
    G4LogicalVolume* LogicalNi = new G4LogicalVolume (SolidNi,NiMat,"LogicalNi");
    G4VPhysicalVolume* PhysicalNi  = new G4PVPlacement(0,G4ThreeVector(0,0,0),LogicalNi,"PhysicalNi",LogicalWorld,false,0,true);

    //Sic
    G4double Sic_X = 1*cm;
    G4double Sic_Y = 1*cm;
    G4double Sic_Z = 310.3*um;

    G4Box *SolidSic = new G4Box ("Sic",Sic_X/2,Sic_Y/2,Sic_Z/2);
    G4LogicalVolume* LogicalVolumeSic = new G4LogicalVolume(SolidSic,Sic,"LogicalSic");
    G4VPhysicalVolume *PhysicalSic = new G4PVPlacement (0,G4ThreeVector(0,0,(Ni_Z/2+Sic_Z/2)),LogicalVolumeSic,"PhysicalSic",LogicalWorld,false,0,true);

    //可视化
    G4VisAttributes* Ni_VisAtt = new G4VisAttributes(G4Color(1.0,0.0,0.0,0.7));
    Ni_VisAtt ->SetForceSolid(true);
    LogicalNi ->SetVisAttributes(Ni_VisAtt);
    G4VisAttributes* Sic_VisAtt = new G4VisAttributes(G4Color(0.0,1.0,0.0,0.7));
    Sic_VisAtt ->SetForceSolid(true);
    LogicalVolumeSic ->SetVisAttributes(Sic_VisAtt);
    
    return PhysicalWorld;

}


















