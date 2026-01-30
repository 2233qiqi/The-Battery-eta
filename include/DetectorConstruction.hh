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


    private:
    void DefineMaterials();
    G4LogicalVolume* fLogicGeCrystal; 
};


