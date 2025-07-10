#include "DetectorConstruction.hh"
#include "RadialEnergyDepositSD.hh"
#include "SingleParticleSD.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVReplica.hh"

DetectorConstruction::DetectorConstruction() {}
/* : G4VUserDetectorConstruction(),
   fScoringVolume(NULL),
   logicSic_mother(NULL),
   logicSiC_layer(NULL),
   logicNi63_mother(NULL),
   logicNi63_layer(NULL) {
   };
*/
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
    G4Material *MixNi = new G4Material("Ni", 8.9 * g / cm3, 1);
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
    auto *solidWorld = new G4Box("SoildWorld", worldX, worldY, worldZ);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldmat, "LogicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "PhysWorld", 0, false, 0, checkOverlaps);
    G4VisAttributes *worldVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 0.3));
    worldVisAtt->SetVisibility(false);
    logicWorld->SetVisAttributes(worldVisAtt);

    // SIC不分层
    G4double sicX = 10 * um, sicY = 10 * um, sicZ = 5 * um;
    G4Box *solidSic = new G4Box("SolidSic", sicX, sicY, sicZ);
    G4LogicalVolume *logicSic = new G4LogicalVolume(solidSic, SiC, "LogicSic");
    G4VPhysicalVolume *physSic = new G4PVPlacement(0, G4ThreeVector(0. * um, 0. * um, 5.0 * um), logicSic, "PhysSic", logicWorld, false, 0, checkOverlaps);

    G4VisAttributes *sicVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3, 0.6));
    sicVisAtt->SetVisibility(true);
    logicSic->SetVisAttributes(sicVisAtt);

    /* // SIC分层
     G4double sicX = 10 * um, sicY = 10 * um, sic_full_Z = 10 * um;

     G4Box *solidSic_mother_box = new G4Box("SolidSic_Mother", 0.5 * sicX, 0.5 * sicY, 0.5 * sic_full_Z);
     logicSic_mother = new G4LogicalVolume(solidSic_mother_box, SiC, "LogicSic_Mother");

     new G4PVPlacement(0, G4ThreeVector(0. * um, 0., 10 * um), logicSic_mother, "PhysSic_Mother", logicWorld, false, 0, checkOverlaps);
     G4int numberOfSiCLayers = 200;
     G4double siCLayerThickness = sic_full_Z / numberOfSiCLayers;

     G4Box *solidSiC_layer_box = new G4Box("SolidSiC_Layer", 0.5 * sicX, 0.5 * sicY, 0.5 * siCLayerThickness);

     logicSiC_layer = new G4LogicalVolume(solidSiC_layer_box, SiC, "LogicSiC_Layer");

     new G4PVReplica("PhysSiC_Layer", logicSiC_layer, logicSic_mother, kZAxis, numberOfSiCLayers, siCLayerThickness);
     G4VisAttributes *sicMotherVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.05));
     sicMotherVisAtt->SetVisibility(true);
     logicSic_mother->SetVisAttributes(sicMotherVisAtt);

     G4VisAttributes *sicLayerVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.4));
     sicLayerVisAtt->SetVisibility(true);
     sicLayerVisAtt->SetForceWireframe(true); // 显示线框
     logicSiC_layer->SetVisAttributes(sicLayerVisAtt);
 */
    // Ni不分层
    G4int NumLayersNi63 = 20;
    G4double layerSpacing = 0.25 * um;
    G4double Ni63X = 10 * um, Ni63Y = 10 * um, Ni63Z = 1.0 * um;
    G4double totalNi63Z = NumLayersNi63 * layerSpacing;

    auto *solidNi63 = new G4Box("SolidNi63", Ni63X, Ni63Y, Ni63Z);
    auto *logicalNi63 = new G4LogicalVolume(solidNi63, MixNi, "LogicNi63");
    G4VPhysicalVolume *physNi63 = new G4PVPlacement(0, G4ThreeVector(0., 0., -5.5 * um), logicalNi63, "PhysNi63", logicWorld, false, 0, checkOverlaps);

    auto *niVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.7));
    niVisAtt->SetVisibility(true);

    logicalNi63->SetVisAttributes(niVisAtt);

    fScoringVolume = logicalNi63;

    /*  // Ni分层
      G4int NumLayersNi63 = 20;
      G4double niLayerThickness = 0.25 * um;
      G4double Ni63X = 10 * um, Ni63Y = 10 * um;
      G4double ni_full_Z = NumLayersNi63 * niLayerThickness;

      G4Box *solidNi63_mother_box = new G4Box("SolidNi63_Mother", 0.5 * Ni63X, 0.5 * Ni63Y, 0.5 * ni_full_Z);
      logicNi63_mother = new G4LogicalVolume(solidNi63_mother_box, MixNi, "LogicNi63_Mother");

      new G4PVPlacement(0, G4ThreeVector(0., 0., -2.0 * um), logicNi63_mother, "PhysNi63_Mother", logicWorld, false, 0, checkOverlaps);

      G4Box *solidNi63_layer_box = new G4Box("SolidNi63_Layer", 0.5 * Ni63X, 0.5 * Ni63Y, 0.5 * niLayerThickness);
      logicNi63_layer = new G4LogicalVolume(solidNi63_layer_box, MixNi, "LogicNi63_Layer");

      new G4PVReplica("PhysNi63_Layer", logicNi63_layer, logicNi63_mother, kZAxis, NumLayersNi63, niLayerThickness);

      G4VisAttributes *niMotherVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.5));
      niMotherVisAtt->SetVisibility(true);
      logicNi63_mother->SetVisAttributes(niMotherVisAtt);

      G4VisAttributes *niLayerVisAtt = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0, 0.5));
      niLayerVisAtt->SetVisibility(true);
      niLayerVisAtt->SetForceWireframe(true); // 显示线框
      logicNi63_layer->SetVisAttributes(niLayerVisAtt);

    fScoringVolume = logicNi63_layer;
*/

    return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{

    auto SdManager = G4SDManager::GetSDMpointer();

    G4String NiSdName = "Ni63_SelfAbsorption_SD";

    if (!SdManager->FindSensitiveDetector(NiSdName, false))
    {
        G4cout << "Constructing Sensitive Detector: " << NiSdName << G4endl;

        auto *niSd = new SingleParticleSD(NiSdName,
                                          800,
                                          10 * um,
                                          "ni63_self_absorption.root");

        SdManager->AddNewDetector(niSd);

        SetSensitiveDetector("LogicNi63", niSd, true);
    }

    G4String SicSdName = "SiC_EscapedParticles_SD";

    if (!SdManager->FindSensitiveDetector(SicSdName, false))
    {
        G4cout << "Constructing Sensitive Detector: " << SicSdName << G4endl;

        auto *sicSd = new SingleParticleSD(SicSdName,
                                           200,
                                           10. * um,
                                           "sic_escaped_particles.root");

        SdManager->AddNewDetector(sicSd);

        SetSensitiveDetector("LogicSic", sicSd, true);
    }
}