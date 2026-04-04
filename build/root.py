import ROOT
import numpy as np
import matplotlib.pyplot as plt

# ===== 读 ROOT 数据 =====
f = ROOT.TFile("DepositionData.root")

# 获取直方图
h_depth = f.Get("Edep_vs_depth")      # SiC中的能量沉积分布
h_ni_loss = f.Get("Edep_Ni")          # Ni层中的能量损失

# 获取Ntuple用于详细分析
tree = f.Get("EventData")

# 统计基本信息
nbins_depth = h_depth.GetNbinsX()
nbins_ni = h_ni_loss.GetNbinsX()

x_depth, y_depth = [], []
for i in range(1, nbins_depth + 1):
    x_depth.append(h_depth.GetBinCenter(i))
    y_depth.append(h_depth.GetBinContent(i))

x_ni, y_ni = [], []
for i in range(1, nbins_ni + 1):
    x_ni.append(h_ni_loss.GetBinCenter(i))
    y_ni.append(h_ni_loss.GetBinContent(i))

x_depth = np.array(x_depth)
y_depth = np.array(y_depth)
x_ni = np.array(x_ni)
y_ni = np.array(y_ni)

# ===== 计算关键参数 =====
W_SiC = 3.28e-6  # SiC的平均电子-空穴对产生能量 (MeV)
W_Ni = 3.8e-6    # Ni的平均电子-空穴对产生能量 (MeV) - 近似值

total_edep_sic = np.sum(y_depth)      # SiC中的总能量沉积
total_edep_ni = np.sum(y_ni)          # Ni中的总能量损失

# 产生的电子-空穴对数
total_eh_pairs_sic = total_edep_sic / W_SiC
total_eh_pairs_ni = total_edep_ni / W_Ni

print("=" * 50)
print("  Betavoltaic 电池 - 能量沉积分析")
print("=" * 50)
print(f"\nSiC层能量沉积统计:")
print(f"  总能量沉积 (SiC):        {total_edep_sic:.6e} MeV")
print(f"  产生的电子-空穴对数:     {total_eh_pairs_sic:.2e}")
print(f"  平均能量沉积深度:       {np.average(x_depth, weights=y_depth):.3f} μm")

print(f"\nNi层能量损失统计:")
print(f"  总能量损失 (Ni):         {total_edep_ni:.6e} MeV")
print(f"  产生的电子-空穴对数:     {total_eh_pairs_ni:.2e}")
print(f"  平均能量损失:            {np.average(x_ni, weights=y_ni) if total_edep_ni > 0 else 0:.6e} MeV")

print("\n" + "=" * 50)

# ===== 绘制能量沉积深度分布图 =====
fig, axes = plt.subplots(2, 1, figsize=(8, 10))

# 图1: SiC中的能量沉积 vs 深度
ax1 = axes[0]
ax1.plot(
    x_depth*1e4, y_depth,           # 转换为nm显示
    color='black',
    linewidth=1.2,
    marker='s',
    markersize=4,
    markerfacecolor='black',
    label='SiC层能量沉积'
)
ax1.set_xlabel("Depth in SiC (μm)", fontsize=12)
ax1.set_ylabel("Energy Deposition (MeV)", fontsize=12)
ax1.set_title("Energy Deposition Profile in SiC", fontsize=13, fontweight='bold')
ax1.tick_params(
    direction='in',
    top=True,
    right=True,
    width=1.2,
    length=6,
    labelsize=11
)
for spine in ax1.spines.values():
    spine.set_linewidth(1.2)
ax1.grid(True, alpha=0.3)
ax1.legend(fontsize=11)

# 图2: Ni层的能量损失分布
ax2 = axes[1]
ax2.plot(
    x_ni*1e6, y_ni,                 # 转换为eV显示
    color='red',
    linewidth=1.2,
    marker='o',
    markersize=4,
    markerfacecolor='red',
    label='Ni层能量损失'
)
ax2.set_xlabel("Energy Loss (eV)", fontsize=12)
ax2.set_ylabel("Count", fontsize=12)
ax2.set_title("Energy Loss Distribution in Ni Layer", fontsize=13, fontweight='bold')
ax2.tick_params(
    direction='in',
    top=True,
    right=True,
    width=1.2,
    length=6,
    labelsize=11
)
for spine in ax2.spines.values():
    spine.set_linewidth(1.2)
ax2.grid(True, alpha=0.3)
ax2.legend(fontsize=11)

plt.tight_layout()
plt.savefig("energy_deposition_analysis.png", dpi=300, bbox_inches='tight')
print("\n✓ 生成能量沉积分析图: energy_deposition_analysis.png")

# ===== 绘制归一化能量沉积剖面 =====
plt.figure(figsize=(10, 6))

# 文献风格的绘图
y_normalized = y_depth / np.max(y_depth) if np.max(y_depth) > 0 else y_depth

plt.plot(
    x_depth*1e4, y_normalized,
    color='black',
    linewidth=1.5,
    marker='s',
    markersize=5,
    markerfacecolor='black',
    markeredgewidth=0.5,
    markeredgecolor='black',
    label='Normalized Deposition Profile'
)

plt.xlabel("Depth in SiC (μm)", fontsize=14, fontweight='bold')
plt.ylabel("Normalized Energy Deposition", fontsize=14, fontweight='bold')
plt.title("Normalized Depth Profile of Energy Deposition", fontsize=14, fontweight='bold')

# 坐标轴设置
plt.tick_params(
    direction='in',
    top=True,
    right=True,
    width=1.5,
    length=8,
    labelsize=12
)

ax = plt.gca()
for spine in ax.spines.values():
    spine.set_linewidth(1.5)

plt.grid(True, alpha=0.3, linestyle='--')
plt.legend(fontsize=12, loc='best')
plt.tight_layout()
plt.savefig("normalized_profile.png", dpi=300, bbox_inches='tight')
print("✓ 生成归一化剖面图: normalized_profile.png")

# ===== 绘制累积分布函数 (CDF) =====
plt.figure(figsize=(10, 6))

# 计算CDF
cumulative_edep = np.cumsum(y_depth)
cdf_normalized = cumulative_edep / cumulative_edep[-1] if cumulative_edep[-1] > 0 else cumulative_edep

plt.plot(
    x_depth*1e4, cdf_normalized,
    color='blue',
    linewidth=1.5,
    marker='o',
    markersize=5,
    markerfacecolor='blue',
    markeredgewidth=0.5,
    markeredgecolor='blue',
    label='Cumulative Energy Distribution'
)

plt.xlabel("Depth in SiC (μm)", fontsize=14, fontweight='bold')
plt.ylabel("Cumulative Energy Fraction", fontsize=14, fontweight='bold')
plt.title("Cumulative Energy Distribution vs Depth", fontsize=14, fontweight='bold')

plt.ylim(0, 1.05)
plt.tick_params(
    direction='in',
    top=True,
    right=True,
    width=1.5,
    length=8,
    labelsize=12
)

ax = plt.gca()
for spine in ax.spines.values():
    spine.set_linewidth(1.5)

plt.grid(True, alpha=0.3, linestyle='--')
plt.legend(fontsize=12, loc='best')
plt.tight_layout()
plt.savefig("cumulative_distribution.png", dpi=300, bbox_inches='tight')
print("✓ 生成累积分布图: cumulative_distribution.png")

print("\n✓ 数据分析完成！所有图表已保存。")
print("="*50 + "\n")

plt.show()

f.Close()