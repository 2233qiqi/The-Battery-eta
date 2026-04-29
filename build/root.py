import ROOT
import numpy as np
import matplotlib.pyplot as plt

# ===== Plot style =====
plt.rcParams.update({
    "font.family": "serif",
    "font.size": 12,
    "axes.linewidth": 1.2,
    "xtick.direction": "in",
    "ytick.direction": "in",
    "xtick.top": True,
    "ytick.right": True
})

# ===== Open ROOT =====
f = ROOT.TFile("DepositionData.root")

if not f or f.IsZombie():
    raise RuntimeError("Cannot open ROOT file")

print("\n===== ROOT content =====")
f.ls()
print("========================\n")

# ===== Get histogram =====
h = f.Get("Edep_vs_depth")

if not h or not hasattr(h, "GetNbinsX"):
    raise RuntimeError("Edep_vs_depth not found or invalid")

# ===== Convert to numpy =====
nbins = h.GetNbinsX()

x = np.array([h.GetBinCenter(i) for i in range(1, nbins + 1)])
y = np.array([h.GetBinContent(i) for i in range(1, nbins + 1)])
bw = h.GetBinWidth(1)

# ===== Basic physics quantities =====
total_edep = np.sum(y)
mean_depth = np.average(x, weights=y) if total_edep > 0 else 0

print("=" * 45)
print(" SiC Energy Deposition Analysis")
print("=" * 45)
print(f"Total energy deposition : {total_edep:.4e} MeV")
print(f"Mean deposition depth  : {mean_depth:.3f} μm")
print("=" * 45)

# ===============================
# Figure 1: Energy deposition vs depth
# ===============================
plt.figure(figsize=(6,5))

plt.bar(
    x - bw/2, y,
    width=bw,
    edgecolor='black',
    linewidth=0.5
)

plt.xlabel("Depth in SiC (μm)")
plt.ylabel("Energy Deposition (MeV)")

plt.tight_layout()
plt.savefig("fig_profile.png", dpi=300)

# ===============================
# Figure 2: Normalized profile
# ===============================
plt.figure(figsize=(6,5))

y_norm = y / np.max(y) if np.max(y) > 0 else y

plt.bar(
    x - bw/2, y_norm,
    width=bw,
    edgecolor='black',
    linewidth=0.5
)

plt.xlabel("Depth in SiC (μm)")
plt.ylabel("Normalized Energy Deposition")

plt.tight_layout()
plt.savefig("fig_normalized.png", dpi=300)

# ===============================
# Figure 3: CDF
# ===============================
plt.figure(figsize=(6,5))

cdf = np.cumsum(y)
cdf = cdf / cdf[-1] if cdf[-1] > 0 else cdf

plt.plot(
    x, cdf,
    marker='o',
    linewidth=1.2,
    markersize=4
)

plt.xlabel("Depth in SiC (μm)")
plt.ylabel("Cumulative Fraction")
plt.ylim(0, 1.05)

plt.tight_layout()
plt.savefig("fig_cdf.png", dpi=300)

plt.show()

f.Close()