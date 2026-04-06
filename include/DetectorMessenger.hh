#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithADoubleAndUnit;

class DetectorMessenger : public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction*);
    ~DetectorMessenger();

    void SetNewValue(G4UIcommand*, G4String) override;

  private:
    DetectorConstruction* fDetector;

    G4UIdirectory*              fDir;
    G4UIcmdWithADoubleAndUnit*  fNiThicknessCmd;
    G4UIcmdWithADoubleAndUnit*  fSicThicknessCmd;
    G4UIcmdWithADoubleAndUnit*  fSourceHalfZCmd;
    G4UIcommand*                fPrintCmd;
};

#endif
