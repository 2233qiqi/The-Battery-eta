import ROOT
import csv

# 打开 ROOT 文件
f = ROOT.TFile("DepositionData.root")

# 获取直方图（改成你的名字）
h = f.Get("Edep_vs_depth")

# 检查是否读取成功
if not h:
    print("没找到直方图，请检查名字")
    exit()


# 写 CSV
with open("sic_energy.csv", "w", newline="") as file:
    writer = csv.writer(file)
    writer.writerow(["thickness", "energy_per_particle"])

    for i in range(1, h.GetNbinsX() + 1):
        thickness = h.GetBinCenter(i)
        energy    = h.GetBinContent(i) 

        writer.writerow([thickness, energy])

