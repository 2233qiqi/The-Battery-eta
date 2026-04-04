import ROOT
import numpy as np
import matplotlib.pyplot as plt

# ===== 读 ROOT 数据 =====
f = ROOT.TFile("DepositionData.root")
h = f.Get("Edep_vs_depth")

nbins = h.GetNbinsX()
x, y = [], []

for i in range(1, nbins + 1):
    x.append(h.GetBinCenter(i))
    y.append(h.GetBinContent(i))

x = np.array(x)
y = np.array(y)

# ===== 画图 =====
plt.figure(figsize=(6,5))

# 数据点 + 连线（关键）
plt.plot(
    x, y,
    color='black',
    linewidth=1.2,
    marker='s',           # 方块点
    markersize=4,
    markerfacecolor='black'
)

# 坐标轴标签（论文风格）
plt.xlabel("Source Thickness (μm)", fontsize=14)
plt.ylabel("Power Density (μW·cm$^{-2}$)", fontsize=14)

# 坐标轴范围（根据你数据调）
plt.xlim(0, 0.011)

# 坐标轴刻度风格（关键）
plt.tick_params(
    direction='in',      # 向内刻度
    top=True,
    right=True,
    width=1.2,
    length=6,
    labelsize=12
)

# 坐标轴边框加粗
ax = plt.gca()
for spine in ax.spines.values():
    spine.set_linewidth(1.2)

# 网格（可选，论文一般不用）
# plt.grid(False)

plt.tight_layout()

# 保存高质量图片
plt.savefig("result.png", dpi=300)

plt.show()