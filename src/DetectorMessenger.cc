#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIparameter.hh"
#include "G4UImanager.hh"
#include <sstream>

DetectorMessenger::DetectorMessenger(DetectorConstruction* detector)
: fDetector(detector)
{
    fDir = new G4UIdirectory("/detector/");
    fDir->SetGuidance("Detector construction control.");

    fNiThicknessCmd = new G4UIcmdWithADoubleAndUnit("/detector/setNiThickness", this);
    fNiThicknessCmd->SetGuidance("Set the Ni layer thickness.");
    fNiThicknessCmd->SetParameterName("NiThickness", false);
    fNiThicknessCmd->SetDefaultUnit("um");
    fNiThicknessCmd->SetRange("NiThickness>0.");

    fSicThicknessCmd = new G4UIcmdWithADoubleAndUnit("/detector/setSicThickness", this);
    fSicThicknessCmd->SetGuidance("Set the SiC layer thickness.");
    fSicThicknessCmd->SetParameterName("SicThickness", false);
    fSicThicknessCmd->SetDefaultUnit("um");
    fSicThicknessCmd->SetRange("SicThickness>0.");

    fSourceHalfZCmd = new G4UIcmdWithADoubleAndUnit("/detector/setSourceHalfZ", this);
    fSourceHalfZCmd->SetGuidance("Set the source half-z (should match Ni thickness/2 for surface source).");
    fSourceHalfZCmd->SetParameterName("SourceHalfZ", false);
    fSourceHalfZCmd->SetDefaultUnit("um");
    fSourceHalfZCmd->SetRange("SourceHalfZ>0.");

    fPrintCmd = new G4UIcommand("/detector/print", this);
    fPrintCmd->SetGuidance("Print current detector geometry parameters.");
}

DetectorMessenger::~DetectorMessenger()
{
    delete fNiThicknessCmd;
    delete fSicThicknessCmd;
    delete fSourceHalfZCmd;
    delete fPrintCmd;
    delete fDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    G4UImanager* UI = G4UImanager::GetUIpointer();

    if (command == fNiThicknessCmd)
    {
        G4double val = fNiThicknessCmd->GetNewDoubleValue(newValue);
        fDetector->SetNiThickness(val);
        std::ostringstream cmd;
        cmd << "/gps/pos/halfz " << val/um << " um";
        UI->ApplyCommand(cmd.str());
    }
    else if (command == fSicThicknessCmd)
    {
        G4double val = fSicThicknessCmd->GetNewDoubleValue(newValue);
        fDetector->SetSicThickness(val);
    }
    else if (command == fSourceHalfZCmd)
    {
        G4double val = fSourceHalfZCmd->GetNewDoubleValue(newValue);
        fDetector->SetSourceHalfZ(val);
        std::ostringstream cmd;
        cmd << "/gps/pos/halfz " << val/um << " um";
        UI->ApplyCommand(cmd.str());
    }
    
}
