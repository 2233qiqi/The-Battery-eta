#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class DetectorMessenger;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
    DetectorConstruction();
    ~DetectorConstruction() override;
    G4VPhysicalVolume* Construct() override;

    // Getters
    G4double GetNiThickness() const { return Ni_Z; }
    G4double GetSicThickness() const { return Sic_Z; }
    G4double GetSourceHalfZ() const { return fSourceHalfZ; }

    // Setters (called by messenger or directly in code)
    void SetNiThickness(G4double val);
    void SetSicThickness(G4double val);
    void SetSourceHalfZ(G4double val);

    void UpdateGeometry();  // rebuild geometry after parameter change

    private:
    void DefineMaterials();
    DetectorMessenger* fMessenger;
    G4LogicalVolume* fLogicGeCrystal;
    G4double Ni_Z;
    G4double Sic_Z;
    G4double fSourceHalfZ;  // source half-z, synced with Ni thickness
};

#endif


