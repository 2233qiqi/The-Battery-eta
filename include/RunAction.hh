#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "globals.hh" 

class RunAction : public G4UserRunAction 
{
public:
    RunAction();
    virtual ~RunAction() override;

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void AddEnteredEvent();
    void AddEnergyInNi(G4double edep);
    void AddEnergyInSiC(G4double edep);
    void AddTotalParticles();
    void AddInitialParticleEnergy(G4double energy);

private:
    G4int fEnteredSiC; 
    G4int fTotalParticles;
    G4double fTotalEnergyNi;
    G4double fTotalEnergySiC;
    G4double fTotalInitialEnergy;
};

#endif