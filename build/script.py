import subprocess
import os
import sys

# --- 配置 ---
GEANT4_EXE_PATH = "./exampleB1"  
MAIN_MACRO_FILE = "run.mac" 
OUTPUT_LOG_FILE = "run.log"       # <--- 修改为 Log 文件名
# ------------

def parse_last_line(log_file: str) -> dict:
    """
    读取 Log 文件最后一行，并解析出标记后的数据。
    """
    try:
        # 使用 'r' 模式以防止读取大文件时内存爆炸
        with open(log_file, 'r') as f:
            # 找到最后一行
            last_line = ""
            for line in f:
                last_line = line.strip()
            
            # 检查是否包含我们的标记
            if ">>>SIM_RESULT:" not in last_line:
                print(f"ERROR: Marker not found in the last line of {log_file}.")
                return None
            
            # 解析数据
            result_str = last_line.split(">>>SIM_RESULT:")[1].strip()
            
            data = {}
            # 分割键值对 (e.g., "Events=50000, TotalEdep_MeV=1.234")
            for item in result_str.split(','):
                key, value = item.split('=')
                data[key.strip()] = float(value.strip()) if '.' in value else int(value.strip())
            
            return data

    except Exception as e:
        print(f"ERROR parsing log file: {e}")
        return None


def run_simulation(run_count: int) -> dict:
    
    # ... (1. 清理旧 Log 文件) ...
    # 清理 Log 文件而不是 JSON 文件
    if os.path.exists(OUTPUT_LOG_FILE):
        os.remove(OUTPUT_LOG_FILE)
        
    # ... (2. 构造命令行调用) ...
    command = [GEANT4_EXE_PATH, f"-Dparticle_count={run_count}", MAIN_MACRO_FILE]
    
    print(f"Executing command: {' '.join(command)}")

    # 3. 执行 Geant4 程序
    try:
        subprocess.run(command, check=True, capture_output=True, text=True)
        print("Geant4 simulation finished successfully.")
    except Exception as e:
        print(f"Error executing Geant4: {e}")
        return None

    # 4. 读取并解析 Log 结果
    if os.path.exists(OUTPUT_LOG_FILE):
        return parse_last_line(OUTPUT_LOG_FILE)
    else:
        print(f"ERROR: Output file {OUTPUT_LOG_FILE} not found. Check main.cc log setup.")
        return None

# ====================================================
# 主执行区域
# ====================================================

if __name__ == "__main__":
    
    # ... (省略参数处理逻辑，假设直接运行) ...
    
    events_to_run = 10000 
    print(f"\n[Test Run] Running with {events_to_run} particles.")

    results = run_simulation(events_to_run)

    if results:
        print("\n--- Simulation Results from Log ---")
        print(f"Events Processed: {results.get('Events')}")
        print(f"Total Energy Deposit (MeV): {results.get('TotalEdep_MeV')}")
    else:
        sys.exit(1)