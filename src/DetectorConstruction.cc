#include "DetectorConstruction.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVReplica.hh"

DetectorConstruction::DetectorConstruction() {

}

DetectorConstruction::~DetectorConstruction() {

};

G4VPhysicalVolume *DetectorConstruction::Construct()
{

    G4bool checkOverlaps = true;

    // 63Ni
    G4Isotope *isoNi63 = new G4Isotope("Ni63", 28, 63, 62.9296 * g / mole);
    G4Isotope *isoNi64 = new G4Isotope("Ni64", 28, 64, 63.92797 * g / mole);
    G4Element *iso_elNi = new G4Element("CustomNickel", "Ni", 2);
    iso_elNi->AddIsotope(isoNi63, 20. * perCent);
    iso_elNi->AddIsotope(isoNi64, 80. * perCent);
    G4Material *MixNi = new G4Material("Ni", 8.902 * g / cm3, 1);
    MixNi->AddElement(iso_elNi, 100. * perCent);

    // SiC
    G4Element *elSi = new G4Element("Silicon", "Si", 14, 28.09 * g / mole);
    G4Element *elC = new G4Element("Carbon", "C", 6, 12.01 * g / mole);
    
    G4Material *SiC = new G4Material("SiliconCarbide", 3.21 * g / cm3, 2);
    SiC->AddElement(elSi, 1);
    SiC->AddElement(elC, 1);

    // construct
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *worldmat = nist->FindOrBuildMaterial("G4_Galactic");

    // World
    G4double worldX = 100 * um, worldY = 100 * um, worldZ = 100 * um;
    G4Box *solidWorld = new G4Box("SoildWorld", worldX, worldY, worldZ);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldmat, "LogicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "PhysWorld", 0, false, 0, checkOverlaps);
    G4VisAttributes *worldVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 0.3));
    worldVisAtt->SetVisibility(false);
    logicWorld->SetVisAttributes(worldVisAtt);

    // SIC
    G4double sicX = 10 * um, sicY = 10 * um, sicZ = 10 * um;
    G4Box *solidSic = new G4Box("SolidSic", sicX, sicY, sicZ);
    G4LogicalVolume *logicSic = new G4LogicalVolume(solidSic, SiC, "LogicSic");
    G4VPhysicalVolume *physSic = new G4PVPlacement(0, G4ThreeVector(0. * um, 0. * um, 10.0 * um), logicSic, "PhysSic", logicWorld, false, 0, checkOverlaps);

    G4VisAttributes *sicVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.7));
    sicVisAtt->SetVisibility(true);
    logicSic->SetVisAttributes(sicVisAtt);

    logicSic->SetUserLimits(new G4UserLimits(1 * nm));

   
    // Ni
    G4int NumLayersNi63 = 20;
    G4double layerSpacing = 0.25 * um;
    G4double Ni63X = 10 * um, Ni63Y = 10 * um, Ni63Z = 0.25 * um;
    G4double totalNi63Z = NumLayersNi63 * layerSpacing;

    auto *solidNi63 = new G4Box("SolidNi63", Ni63X, Ni63Y, Ni63Z);
    auto *logicalNi63 = new G4LogicalVolume(solidNi63, MixNi, "LogicNi63");
    G4VPhysicalVolume *physNi63 = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.25 * um), logicalNi63, "PhysNi63", logicWorld, false, 0, checkOverlaps);

    auto *niVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.7));
    niVisAtt->SetVisibility(true);

    logicalNi63->SetVisAttributes(niVisAtt);

    logicalNi63->SetUserLimits(new G4UserLimits(1 * nm));

    fScoringVolume = logicalNi63;

    

    return physWorld;
}
