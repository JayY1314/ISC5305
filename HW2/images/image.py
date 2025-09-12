import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import os

def create_images_folder():
    """Create images folder if it doesn't exist."""
    if not os.path.exists("images"):
        os.makedirs("images")
        print("Created images/ folder")

def plot_position_vs_time_2d(filename="traject_2d.txt"):
    """
    Plot 2D particle position as a function of time in x-y plane.
    This satisfies the requirement for position vs time plots.
    """
    try:
        data = np.loadtxt(filename, skiprows=1)  # Skip header
        time, x, y = data[:, 0], data[:, 1], data[:, 2]
        
        # Create subplot for position vs time
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))
        
        # Plot x position vs time
        ax1.plot(time, x, "r-", linewidth=2, label="X Position")
        ax1.set_xlabel("Time (s)", fontsize=12)
        ax1.set_ylabel("X Position", fontsize=12)
        ax1.set_title("2D Particle: X Position vs Time", fontsize=14, fontweight="bold")
        ax1.legend()
        ax1.grid(True, alpha=0.3)
        
        # Plot y position vs time
        ax2.plot(time, y, "b-", linewidth=2, label="Y Position")
        ax2.set_xlabel("Time (s)", fontsize=12)
        ax2.set_ylabel("Y Position", fontsize=12)
        ax2.set_title("2D Particle: Y Position vs Time", fontsize=14, fontweight="bold")
        ax2.legend()
        ax2.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig("images/position_vs_time_2d.png", dpi=300, bbox_inches="tight")
        plt.show()
        print("2D position vs time plot saved to images/position_vs_time_2d.png")
        
    except FileNotFoundError:
        print(f"Error: {filename} not found. Run main.exe first to generate trajectory files.")
    except Exception as e:
        print(f"Error plotting 2D position vs time: {e}")

def plot_position_vs_time_3d(filename="traject_3d.txt"):
    """
    Plot 3D particle position as a function of time in x-y plane.
    For 3D case, ignores the z component as specified in requirements.
    """
    try:
        data = np.loadtxt(filename, skiprows=1)  # Skip header
        time, x, y, z = data[:, 0], data[:, 1], data[:, 2], data[:, 3]
        
        # Create subplot for position vs time (ignoring z component)
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))
        
        # Plot x position vs time (ignoring z)
        ax1.plot(time, x, "r-", linewidth=2, label="X Position")
        ax1.set_xlabel("Time (s)", fontsize=12)
        ax1.set_ylabel("X Position", fontsize=12)
        ax1.set_title("3D Particle: X Position vs Time (Z component ignored)", fontsize=14, fontweight="bold")
        ax1.legend()
        ax1.grid(True, alpha=0.3)
        
        # Plot y position vs time (ignoring z)
        ax2.plot(time, y, "b-", linewidth=2, label="Y Position")
        ax2.set_xlabel("Time (s)", fontsize=12)
        ax2.set_ylabel("Y Position", fontsize=12)
        ax2.set_title("3D Particle: Y Position vs Time (Z component ignored)", fontsize=14, fontweight="bold")
        ax2.legend()
        ax2.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig("images/position_vs_time_3d.png", dpi=300, bbox_inches="tight")
        plt.show()
        print("3D position vs time plot saved to images/position_vs_time_3d.png")
        
    except FileNotFoundError:
        print(f"Error: {filename} not found. Run main.exe first to generate trajectory files.")
    except Exception as e:
        print(f"Error plotting 3D position vs time: {e}")

def plot_xy_trajectory_2d(filename="traject_2d.txt"):
    """Plot 2D trajectory in x-y plane."""
    try:
        data = np.loadtxt(filename, skiprows=1)
        time, x, y = data[:, 0], data[:, 1], data[:, 2]
        
        plt.figure(figsize=(10, 8))
        plt.plot(x, y, "b-", linewidth=2, label="2D Particle Trajectory")
        plt.scatter(x[0], y[0], color="green", s=100, label="Start", zorder=5)
        plt.scatter(x[-1], y[-1], color="red", s=100, label="End", zorder=5)
        
        plt.xlabel("X Position", fontsize=12)
        plt.ylabel("Y Position", fontsize=12)
        plt.title("2D Particle Trajectory in X-Y Plane", fontsize=14, fontweight="bold")
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.axis("equal")
        
        plt.tight_layout()
        plt.savefig("images/trajectory_2d_xy.png", dpi=300, bbox_inches="tight")
        plt.show()
        print("2D trajectory plot saved to images/trajectory_2d_xy.png")
        
    except Exception as e:
        print(f"Error plotting 2D trajectory: {e}")

def plot_xy_trajectory_3d(filename="traject_3d.txt"):
    """
    Plot 3D trajectory in x-y plane (ignoring z component as required).
    This satisfies the requirement to ignore z component for 3D case.
    """
    try:
        data = np.loadtxt(filename, skiprows=1)
        time, x, y, z = data[:, 0], data[:, 1], data[:, 2], data[:, 3]
        
        plt.figure(figsize=(10, 8))
        # Plot only x-y components, ignoring z as specified
        plt.plot(x, y, "r-", linewidth=2, label="3D Particle Trajectory (X-Y projection, Z ignored)")
        plt.scatter(x[0], y[0], color="green", s=100, label="Start", zorder=5)
        plt.scatter(x[-1], y[-1], color="red", s=100, label="End", zorder=5)
        
        plt.xlabel("X Position", fontsize=12)
        plt.ylabel("Y Position", fontsize=12)
        plt.title("3D Particle Trajectory in X-Y Plane (Z component ignored)", fontsize=14, fontweight="bold")
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.axis("equal")
        
        plt.tight_layout()
        plt.savefig("images/trajectory_3d_xy.png", dpi=300, bbox_inches="tight")
        plt.show()
        print("3D trajectory plot (X-Y only) saved to images/trajectory_3d_xy.png")
        
    except Exception as e:
        print(f"Error plotting 3D trajectory: {e}")

def plot_combined_analysis():
    """Create combined analysis plot showing all required visualizations."""
    try:
        data_2d = np.loadtxt("traject_2d.txt", skiprows=1)
        data_3d = np.loadtxt("traject_3d.txt", skiprows=1)
        
        time_2d, x_2d, y_2d = data_2d[:, 0], data_2d[:, 1], data_2d[:, 2]
        time_3d, x_3d, y_3d, z_3d = data_3d[:, 0], data_3d[:, 1], data_3d[:, 2], data_3d[:, 3]
        
        fig, axes = plt.subplots(2, 2, figsize=(16, 12))
        
        # 2D Position vs Time
        axes[0, 0].plot(time_2d, x_2d, "r-", linewidth=2, label="X position")
        axes[0, 0].plot(time_2d, y_2d, "b-", linewidth=2, label="Y position")
        axes[0, 0].set_xlabel("Time (s)")
        axes[0, 0].set_ylabel("Position")
        axes[0, 0].set_title("2D Particle: Position vs Time")
        axes[0, 0].legend()
        axes[0, 0].grid(True, alpha=0.3)
        
        # 3D Position vs Time (ignoring Z)
        axes[0, 1].plot(time_3d, x_3d, "r-", linewidth=2, label="X position")
        axes[0, 1].plot(time_3d, y_3d, "b-", linewidth=2, label="Y position")
        axes[0, 1].set_xlabel("Time (s)")
        axes[0, 1].set_ylabel("Position")
        axes[0, 1].set_title("3D Particle: Position vs Time (Z ignored)")
        axes[0, 1].legend()
        axes[0, 1].grid(True, alpha=0.3)
        
        # 2D Trajectory
        axes[1, 0].plot(x_2d, y_2d, "b-", linewidth=2, label="2D Trajectory")
        axes[1, 0].scatter(x_2d[0], y_2d[0], color="green", s=100, label="Start")
        axes[1, 0].scatter(x_2d[-1], y_2d[-1], color="red", s=100, label="End")
        axes[1, 0].set_xlabel("X Position")
        axes[1, 0].set_ylabel("Y Position")
        axes[1, 0].set_title("2D Trajectory in X-Y Plane")
        axes[1, 0].legend()
        axes[1, 0].grid(True, alpha=0.3)
        axes[1, 0].axis("equal")
        
        # 3D Trajectory (X-Y only, Z ignored)
        axes[1, 1].plot(x_3d, y_3d, "r-", linewidth=2, label="3D Trajectory (X-Y only)")
        axes[1, 1].scatter(x_3d[0], y_3d[0], color="green", s=100, label="Start")
        axes[1, 1].scatter(x_3d[-1], y_3d[-1], color="red", s=100, label="End")
        axes[1, 1].set_xlabel("X Position")
        axes[1, 1].set_ylabel("Y Position")
        axes[1, 1].set_title("3D Trajectory in X-Y Plane (Z ignored)")
        axes[1, 1].legend()
        axes[1, 1].grid(True, alpha=0.3)
        axes[1, 1].axis("equal")
        
        plt.tight_layout()
        plt.savefig("images/complete_analysis.png", dpi=300, bbox_inches="tight")
        plt.show()
        print("Complete analysis plot saved to images/complete_analysis.png")
        
    except Exception as e:
        print(f"Error creating combined analysis: {e}")

def main():
    """Main function to create all required visualizations."""
    print("Particle Trajectory Visualization")
    print("=" * 50)
    print("Creating plots to satisfy assignment requirements:")
    print("1. Position as function of time in x-y plane")
    print("2. For 3D case: ignoring z component as specified")
    print("=" * 50)
    
    create_images_folder()
    
    # Check if trajectory files exist
    if not os.path.exists("traject_2d.txt") or not os.path.exists("traject_3d.txt"):
        print("Trajectory files not found. Please run main.exe first to generate them.")
        print("Expected files: traject_2d.txt, traject_3d.txt")
        return
    
    print("Creating required visualizations...")
    
    # Create position vs time plots (main requirement)
    plot_position_vs_time_2d()
    plot_position_vs_time_3d()
    
    # Create trajectory plots
    plot_xy_trajectory_2d()
    plot_xy_trajectory_3d()
    
    # Create combined analysis
    plot_combined_analysis()
    
    print("\n" + "=" * 50)
    print("All required plots completed!")
    print("Check the images/ folder for:")
    print("- position_vs_time_2d.png: 2D position vs time")
    print("- position_vs_time_3d.png: 3D position vs time (Z ignored)")
    print("- trajectory_2d_xy.png: 2D trajectory in X-Y plane")
    print("- trajectory_3d_xy.png: 3D trajectory in X-Y plane (Z ignored)")
    print("- complete_analysis.png: Combined analysis")
    print("=" * 50)

if __name__ == "__main__":
    main()
