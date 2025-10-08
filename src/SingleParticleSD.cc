#include "SingleParticleSD.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"

#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include <cmath>
#include <vector>
#include <mutex>

SingleParticleSD::SingleParticleSD(const G4String &name, G4int nbins, G4double maxDist, const G4String &filename)
    : G4VSensitiveDetector(name),
      fNbins(nbins),
      fMaxDist(maxDist),
      fBinWidth(maxDist > 0 ? maxDist / nbins : 0.0),
      fOutputFilename(filename),
      fRunEdepSum(nbins, 0.0),
      fRunEdepSum2(nbins, 0.0),
      fRunHitCounts(nbins, 0),
      fEventEdepSum(nbins, 0.0),
      fEventEdepSum2(nbins, 0.0),
      fEventHitCounts(nbins, 0),
      fMeanEdep(nbins, 0.0),
      fStdError(nbins, 0.0)
{
  
}

SingleParticleSD::~SingleParticleSD()
{
    if (G4Threading::IsMasterThread())
    {
        CalculateStatistics();
        SaveToROOT();
    }
}

void SingleParticleSD::Initialize(G4HCofThisEvent *) { ClearEventData(); }

void SingleParticleSD::ClearEventData()
{
    std::fill(fEventEdepSum.begin(), fEventEdepSum.end(), 0.0);
    std::fill(fEventEdepSum2.begin(), fEventEdepSum2.end(), 0.0);
    std::fill(fEventHitCounts.begin(), fEventHitCounts.end(), 0);
}

G4bool SingleParticleSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.0)
        return false;

    auto touchable = step->GetPreStepPoint()->GetTouchable();
    G4ThreeVector localPosition = touchable->GetHistory()->GetTopTransform().TransformPoint(step->GetPreStepPoint()->GetPosition());
    G4double depth = localPosition.z() + fMaxDist / 2.0;

    if (depth < 0.0 || depth >= fMaxDist)
        return false;

    G4int bin_idx = static_cast<int>(depth / fBinWidth);
    if (bin_idx >= 0 && bin_idx < fNbins)
    {
        fEventEdepSum[bin_idx] += edep;
        fEventEdepSum2[bin_idx] += edep * edep;
        fEventHitCounts[bin_idx]++;
    }
    return true;
}

void SingleParticleSD::EndOfEvent(G4HCofThisEvent *)
{
    std::lock_guard<std::mutex> lock(fMutex);
    for (G4int i = 0; i < fNbins; ++i)
    {
        fRunEdepSum[i] += fEventEdepSum[i];
        fRunEdepSum2[i] += fEventEdepSum2[i];
        fRunHitCounts[i] += fEventHitCounts[i];
    }
}

void SingleParticleSD::CalculateStatistics()
{
    fTotalEdepAllEvents = 0.0;
    for (G4int i = 0; i < fNbins; ++i)
    {
        if (fRunHitCounts[i] > 0)
        {
            fMeanEdep[i] = fRunEdepSum[i] / fRunHitCounts[i];
            if (fRunHitCounts[i] > 1)
            {
                G4double variance = (fRunEdepSum2[i] - fRunEdepSum[i] * fMeanEdep[i]) / (fRunHitCounts[i] - 1);
                fStdError[i] = std::sqrt(std::abs(variance) / fRunHitCounts[i]);
            }
        }
        fTotalEdepAllEvents += fRunEdepSum[i];
    }
   
    G4cout<<std::setw(20)<<"Total Energy Deposited:"<<fTotalEdepAllEvents <<" Mev"<<G4endl;
   
}

void SingleParticleSD::SaveToROOT()
{
    TFile file(fOutputFilename.c_str(), "RECREATE");
    if (!file.IsOpen())
        return;

    std::vector<G4double> x_depth(fNbins);
    std::vector<G4double> y_total_edep(fNbins);

    for (G4int i = 0; i < fNbins; ++i)
    {
        x_depth[i] = (i + 0.5) * fBinWidth / um;
        y_total_edep[i] = fRunEdepSum[i] / keV;
    }

    TGraph *graph = new TGraph(fNbins, x_depth.data(), y_total_edep.data());
    graph->SetName("TotalEnergyDepositVsDepth");
    graph->SetTitle("Depth vs Energy Deposited;Depth [um];Total Energy Deposited [keV]");
    graph->SetMarkerStyle(20);
    graph->SetMarkerColor(kBlue);
    graph->SetLineColor(kBlue);
    graph->Write();

    TCanvas canvas("canvas", "Depth-Dose Curve", 800, 600);
    graph->Draw("APL");
    canvas.SaveAs("Energy_vs_Depth.png");

    file.Close();
}
