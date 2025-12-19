import json
import matplotlib.pyplot as plt
import sys

def plot_results(json_path):
    try:
        with open(json_path, 'r') as f:
            data = json.load(f)["history"]
    except FileNotFoundError:
        print(f"Error: {json_path} not found. Run the C++ app first.")
        return

    steps = [x["step"] for x in data]
    integrity = [x["functional_integrity"] for x in data]
    recovery = [x["recovery_capacity"] for x in data]
    resilience = [x["resilience_potential"] for x in data]

    plt.figure(figsize=(10, 6))
    
    # Plot Functional Integrity line
    plt.plot(steps, integrity, label="Functional Integrity", marker='o', linewidth=2)
    
    # Plot Recovery Capacity as explicit bars or secondary line
    plt.plot(steps, recovery, label="Recovery Capacity (Logic)", marker='s', linestyle='--', alpha=0.7)
    
    plt.title("Ecofunctional Trajectory Analysis")
    plt.xlabel("Simulation Step (Time)")
    plt.ylabel("Index Value (0-1)")
    plt.ylim(0, 1.1)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.legend()
    
    # Annotate final state
    plt.annotate(f'Final Recovery: {recovery[-1]:.2f}', 
                 xy=(steps[-1], recovery[-1]), 
                 xytext=(steps[-1]-1, recovery[-1]+0.1),
                 arrowprops=dict(facecolor='black', shrink=0.05))

    output_file = "trajectory_plot.png"
    plt.savefig(output_file)
    print(f"Plot saved to {output_file}")

if __name__ == "__main__":
    json_file = "inference_results.json"
    if len(sys.argv) > 1:
        json_file = sys.argv[1]
    plot_results(json_file)
