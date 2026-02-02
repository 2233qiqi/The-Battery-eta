#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class DetectorConstruction;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
    DetectorConstruction();
    ~DetectorConstruction() override;
    virtual G4VPhysicalVolume* Construct();
    G4double NiThnickness()const 
    {
        return Ni_Z;
    }


    private:
    void DefineMaterials();
    G4LogicalVolume* fLogicGeCrystal; 
    G4double Ni_Z;
};

#endif


