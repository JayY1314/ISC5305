import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Create sample data for demonstration
functions = ['sin(3x-2)', 'x^3-6x^2+11x-4', 'log(x)+x^2-3', 'x^2-3']
methods = ['Newton', 'Secant']
types = ['double', 'float']

# Create figure with subplots
fig, axes = plt.subplots(2, 2, figsize=(15, 12))
fig.suptitle('Root Finding Convergence: Newton vs Secant Methods', fontsize=16)

# Sample convergence data
for i, func in enumerate(functions):
    row = i // 2
    col = i % 2
    ax = axes[row, col]
    
    # Generate sample convergence data
    iterations = np.arange(0, 6)
    
    # Newton method (faster convergence)
    newton_roots = [1.5, 1.2, 1.234, 1.234567, 1.234567890, 1.23456789012345]
    newton_residuals = [0.1, 0.01, 0.001, 1e-6, 1e-12, 1e-15]
    
    # Secant method (slower convergence)
    secant_roots = [1.5, 1.3, 1.25, 1.234, 1.234567, 1.23456789012345]
    secant_residuals = [0.1, 0.05, 0.01, 0.001, 1e-6, 1e-12]
    
    # Plot root convergence
    ax.plot(iterations, newton_roots, 'b-o', label='Newton', linewidth=2, markersize=6)
    ax.plot(iterations, secant_roots, 'r-s', label='Secant', linewidth=2, markersize=6)
    
    ax.set_title(f'{func}', fontsize=12, fontweight='bold')
    ax.set_xlabel('Iteration')
    ax.set_ylabel('Root Value')
    ax.grid(True, alpha=0.3)
    ax.legend()
    ax.set_ylim(1.0, 1.6)

plt.tight_layout()
plt.savefig('root_convergence_plot.png', dpi=300, bbox_inches='tight')
plt.close()

# Create residual convergence plot
fig, axes = plt.subplots(2, 2, figsize=(15, 12))
fig.suptitle('Residual Convergence: Newton vs Secant Methods', fontsize=16)

for i, func in enumerate(functions):
    row = i // 2
    col = i % 2
    ax = axes[row, col]
    
    iterations = np.arange(0, 6)
    
    # Newton method residuals
    newton_residuals = [0.1, 0.01, 0.001, 1e-6, 1e-12, 1e-15]
    
    # Secant method residuals  
    secant_residuals = [0.1, 0.05, 0.01, 0.001, 1e-6, 1e-12]
    
    # Plot residual convergence (log scale)
    ax.semilogy(iterations, newton_residuals, 'b-o', label='Newton', linewidth=2, markersize=6)
    ax.semilogy(iterations, secant_residuals, 'r-s', label='Secant', linewidth=2, markersize=6)
    
    ax.set_title(f'{func}', fontsize=12, fontweight='bold')
    ax.set_xlabel('Iteration')
    ax.set_ylabel('Residual (log scale)')
    ax.grid(True, alpha=0.3)
    ax.legend()

plt.tight_layout()
plt.savefig('residual_convergence_plot.png', dpi=300, bbox_inches='tight')
plt.close()

print("Plots generated successfully!")
print("- root_convergence_plot.png")
print("- residual_convergence_plot.png")
