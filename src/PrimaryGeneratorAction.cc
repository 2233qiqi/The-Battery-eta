#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"

#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"    
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4Exception.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(RunAction* runAction) 
    : G4VUserPrimaryGeneratorAction(),
      fRunAction(runAction)
{
    fGPS = new G4GeneralParticleSource();
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
    // 统计粒子数
    if (fRunAction)
    {
        fRunAction->AddTotalParticles();
    }
    
    fGPS->GeneratePrimaryVertex(event);
    
    // 获取粒子初始能量
    if (event->GetNumberOfPrimaryVertex() > 0)
    {
        G4PrimaryParticle* particle = event->GetPrimaryVertex(0)->GetPrimary(0);
        if (particle && fRunAction)
        {
            G4double kineticEnergy = particle->GetKineticEnergy();
            fRunAction->AddInitialParticleEnergy(kineticEnergy);
        }
    }
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fGPS;
}

