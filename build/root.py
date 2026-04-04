import ROOT
import numpy as np
import matplotlib.pyplot as plt

# 打开文件
f = ROOT.TFile("DepositionData.root")
h = f.Get("Edep_vs_depth")

# 读取数据
nbins = h.GetNbinsX()

x = []
y = []

for i in range(1, nbins + 1):
    x.append(h.GetBinCenter(i))     # 深度
    y.append(h.GetBinContent(i))    # 能量沉积

x = np.array(x)
y = np.array(y)

# 画图
plt.figure(figsize=(8,6))

plt.plot(x, y, linewidth=1.5)

plt.xlabel("Depth (um)", fontsize=14)
plt.ylabel("Energy Deposition", fontsize=14)
plt.title("Energy Deposition vs Depth (SiC)", fontsize=16)

plt.grid(alpha=0.3)
plt.tight_layout()

plt.show()