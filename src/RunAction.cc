#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction(): G4UserRunAction(),fEnteredSiC(0),fTotalParticles(0),fTotalEnergyNi(0.),fTotalEnergySiC(0.)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetFileName("DepositionData");
    analysisManager->SetDefaultFileType("root");

    analysisManager->CreateH1("Edep_vs_depth", "Energy deposition vs depth in SiC", 100, 0.0, 310.3*um);
    
    analysisManager->CreateH1("Edep_Ni", "Energy loss in Ni layer", 100, 0.0, 3.0*MeV);
    
    analysisManager->CreateNtuple("EventData", "Event summary data");
    analysisManager->CreateNtupleIColumn("EventID");
    analysisManager->CreateNtupleIColumn("ParticleCrossedNi");
    analysisManager->CreateNtupleIColumn("EnteredSiC");
    analysisManager->CreateNtupleIColumn("CarriersPair");           // 电子-空穴对数
    analysisManager->CreateNtupleIColumn("EhPairsFromNi");          // Ni层产生的对数
    analysisManager->CreateNtupleIColumn("EhPairsFromSiC");         // SiC层产生的对数
    analysisManager->CreateNtupleDColumn("EnergyNi");               // Ni层能量损失(MeV)
    analysisManager->CreateNtupleDColumn("EnergySiC");              // SiC层能量沉积(MeV)
    analysisManager->CreateNtupleDColumn("TotalInitialEnergy");     // 初始粒子能量(MeV)
    analysisManager->FinishNtuple();
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile();
    
    fEnteredSiC = 0;
    fTotalParticles = 0;
    fTotalEnergyNi = 0.;
    fTotalEnergySiC = 0.;
}

void RunAction::AddEnteredEvent()
{
    fEnteredSiC++;
}

void RunAction::AddEnergyInNi(G4double edep)
{
    fTotalEnergyNi += edep;
}

void RunAction::AddEnergyInSiC(G4double edep)
{
    fTotalEnergySiC += edep;
}

void RunAction::AddTotalParticles()
{
    fTotalParticles++;
}

void RunAction::EndOfRunAction(const G4Run*)
{
    
    G4cout << "=====================================" << G4endl;
    G4cout << " 运行统计结果 " << G4endl;
    G4cout << "=====================================" << G4endl;
    G4cout << " 总粒子数:                 " << fTotalParticles << G4endl;
    G4cout << " 进入SiC的粒子数:          " << fEnteredSiC << G4endl;
    
    if (fTotalParticles > 0)
    {
        G4cout << " 进入效率:                 " << (G4double)fEnteredSiC/fTotalParticles*100. << " %" << G4endl;
    }
    
    G4cout << " Ni层总能量损失:           " << fTotalEnergyNi/MeV << " MeV" << G4endl;
    G4cout << " SiC层总能量沉积:          " << fTotalEnergySiC/MeV << " MeV" << G4endl;
    G4cout << "=====================================" << G4endl;
   
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}