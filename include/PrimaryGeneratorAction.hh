#ifndef PRIMARYGENERATOR_HH
#define PRIMARYGENERATOR_HH

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class RunAction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction(RunAction* runAction = nullptr);
  virtual ~PrimaryGeneratorAction();
  virtual void GeneratePrimaries(G4Event *);

private:
  G4ParticleGun *fParticleGun;
  G4ParticleTable *particleTable;
  G4ParticleDefinition *particleDefinition;
  G4GeneralParticleSource *fGPS;
  RunAction* fRunAction;
};

#endif