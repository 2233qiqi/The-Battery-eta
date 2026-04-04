import ROOT
import numpy as np
import matplotlib.pyplot as plt

plt.rcParams.update({
    "font.family": "serif",
    "font.size": 12,
    "axes.linewidth": 1.2,
    "xtick.direction": "in",
    "ytick.direction": "in",
    "xtick.top": True,
    "ytick.right": True
})

f = ROOT.TFile("DepositionData.root")

h_depth = f.Get("Edep_vs_depth")
h_ni = f.Get("Edep_Ni")

nbins_depth = h_depth.GetNbinsX()
nbins_ni = h_ni.GetNbinsX()

x_depth = np.array([h_depth.GetBinCenter(i) for i in range(1, nbins_depth + 1)])
y_depth = np.array([h_depth.GetBinContent(i) for i in range(1, nbins_depth + 1)])

x_ni = np.array([h_ni.GetBinCenter(i) for i in range(1, nbins_ni + 1)])
y_ni = np.array([h_ni.GetBinContent(i) for i in range(1, nbins_ni + 1)])

W_SiC = 3.28e-6
W_Ni = 3.8e-6

total_edep_sic = np.sum(y_depth)
total_edep_ni = np.sum(y_ni)

print("=" * 40)
print("Betavoltaic Energy Deposition Analysis")
print("=" * 40)
print(f"SiC total energy deposition: {total_edep_sic:.4e} MeV")
print(f"Ni total energy loss:        {total_edep_ni:.4e} MeV")
print(f"Mean depth (SiC):            {np.average(x_depth, weights=y_depth):.3f} μm")
print("=" * 40)

# ===== Figure 1: Deposition profiles =====
fig, ax = plt.subplots(figsize=(6,5))

ax.plot(
    x_depth, y_depth,
    color='black',
    linewidth=1.2,
    marker='s',
    markersize=4,
    label='SiC'
)

ax.plot(
    x_ni, y_ni,
    color='black',
    linestyle='--',
    linewidth=1.2,
    marker='o',
    markersize=4,
    label='Ni'
)

ax.set_xlabel(r"Depth / Energy (arb. unit)")
ax.set_ylabel(r"Energy Deposition (MeV)")
ax.legend(frameon=False)

plt.tight_layout()
plt.savefig("fig_profile.png", dpi=300)

# ===== Figure 2: Normalized profile =====
plt.figure(figsize=(6,5))

y_norm = y_depth / np.max(y_depth) if np.max(y_depth) > 0 else y_depth

plt.plot(
    x_depth, y_norm,
    color='black',
    linewidth=1.3,
    marker='s',
    markersize=4
)

plt.xlabel(r"Depth in SiC ($\mu$m)")
plt.ylabel(r"Normalized Energy Deposition")

plt.tight_layout()
plt.savefig("fig_normalized.png", dpi=300)

# ===== Figure 3: CDF =====
plt.figure(figsize=(6,5))

cdf = np.cumsum(y_depth)
cdf = cdf / cdf[-1] if cdf[-1] > 0 else cdf

plt.plot(
    x_depth, cdf,
    color='black',
    linewidth=1.3,
    marker='o',
    markersize=4
)

plt.xlabel(r"Depth in SiC ($\mu$m)")
plt.ylabel(r"Cumulative Fraction")

plt.ylim(0, 1.05)

plt.tight_layout()
plt.savefig("fig_cdf.png", dpi=300)

plt.show()

f.Close()