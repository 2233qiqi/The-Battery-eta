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
    if (fRunAction)
    {
        fRunAction->AddTotalParticles();
    }

    fGPS->GeneratePrimaryVertex(event);

    for (G4int iv = 0; iv < event->GetNumberOfPrimaryVertex(); ++iv)
    {
        G4PrimaryVertex* vertex = event->GetPrimaryVertex(iv);
        if (!vertex) continue;
        for (G4int ip = 0; ip < vertex->GetNumberOfParticle(); ++ip)
        {
            G4PrimaryParticle* particle = vertex->GetPrimary(ip);
            if (particle && fRunAction)
            {
                fRunAction->AddInitialParticleEnergy(particle->GetKineticEnergy());
            }
        }
    }
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fGPS;
}

