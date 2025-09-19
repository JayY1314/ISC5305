import matplotlib.pyplot as plt
import numpy as np

# Sample data (in microseconds)
sizes = [10, 100, 1000]
grid1d_times = [50, 5000, 500000]  # Estimated times
gridvec_times = [80, 8000, 800000]  # Estimated times  
gridnew_times = [70, 7000, 700000]  # Estimated times

# Create the plot
plt.figure(figsize=(12, 8))

# Plot the data
plt.loglog(sizes, grid1d_times, 'b-o', label='Grid1D (1D Array)', linewidth=2, markersize=8)
plt.loglog(sizes, gridvec_times, 'r-s', label='GridVec (Vector)', linewidth=2, markersize=8)
plt.loglog(sizes, gridnew_times, 'g-^', label='GridNew (New Operator)', linewidth=2, markersize=8)

# Customize the plot
plt.xlabel('Grid Size (nnn)', fontsize=12)
plt.ylabel('Execution Time (microseconds)', fontsize=12)
plt.title('3D Grid Addition Performance Comparison', fontsize=14, fontweight='bold')
plt.grid(True, alpha=0.3)
plt.legend(fontsize=11)

# Add annotations
plt.annotate('Best Performance\n(Cache-friendly)', 
             xy=(100, 5000), xytext=(200, 2000),
             arrowprops=dict(arrowstyle='->', color='blue', alpha=0.7),
             fontsize=10, color='blue')

plt.annotate('Worst Performance\n(Multiple indirections)', 
             xy=(100, 8000), xytext=(200, 15000),
             arrowprops=dict(arrowstyle='->', color='red', alpha=0.7),
             fontsize=10, color='red')

# Set axis limits and ticks
plt.xlim(8, 1200)
plt.ylim(30, 1000000)

# Add text box with performance analysis
textstr = '''Performance Analysis:
 Grid1D: Best cache locality
 GridNew: Direct access, some fragmentation  
 GridVec: Multiple indirections, worst performance'''

props = dict(boxstyle='round', facecolor='wheat', alpha=0.8)
plt.text(0.02, 0.98, textstr, transform=plt.gca().transAxes, fontsize=9,
         verticalalignment='top', bbox=props)

plt.tight_layout()
plt.savefig('performance_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

print("Performance plot saved as 'performance_comparison.png'")
