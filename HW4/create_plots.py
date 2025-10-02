import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

def create_plots():
    # Check if CSV files exist
    if not os.path.exists('newton_plot.csv') or not os.path.exists('secant_plot.csv'):
        print("Error: CSV files not found. Please run main_visualization.cpp first.")
        return
    
    # Read the data
    newton_data = pd.read_csv('newton_plot.csv')
    secant_data = pd.read_csv('secant_plot.csv')
    
    # Get unique functions
    functions = newton_data['function'].unique()
    
    # Create subplots
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    fig.suptitle('Root Finding Convergence: Newton vs Secant Methods', fontsize=16)
    
    # Plot for each function
    for i, func in enumerate(functions):
        row = i // 2
        col = i % 2
        
        # Filter data for current function
        newton_func = newton_data[newton_data['function'] == func]
        secant_func = secant_data[secant_data['function'] == func]
        
        # Plot Newton's method
        axes[row, col].plot(newton_func['iteration'], newton_func['root'], 
                           'b-o', label='Newton', markersize=4, linewidth=2)
        
        # Plot Secant method
        axes[row, col].plot(secant_func['iteration'], secant_func['root'], 
                           'r-s', label='Secant', markersize=4, linewidth=2)
        
        axes[row, col].set_title(f'{func}', fontsize=12)
        axes[row, col].set_xlabel('Iteration')
        axes[row, col].set_ylabel('Root Value')
        axes[row, col].legend()
        axes[row, col].grid(True, alpha=0.3)
        
        # Add convergence line
        final_newton = newton_func['root'].iloc[-1]
        final_secant = secant_func['root'].iloc[-1]
        axes[row, col].axhline(y=final_newton, color='b', linestyle='--', alpha=0.5)
        axes[row, col].axhline(y=final_secant, color='r', linestyle='--', alpha=0.5)
    
    plt.tight_layout()
    plt.savefig('root_convergence_plot.png', dpi=300, bbox_inches='tight')
    plt.show()
    
    # Create residual plots
    fig2, axes2 = plt.subplots(2, 2, figsize=(15, 12))
    fig2.suptitle('Residual Convergence: Newton vs Secant Methods', fontsize=16)
    
    for i, func in enumerate(functions):
        row = i // 2
        col = i % 2
        
        # Filter data for current function
        newton_func = newton_data[newton_data['function'] == func]
        secant_func = secant_data[secant_data['function'] == func]
        
        # Plot residuals (log scale)
        axes2[row, col].semilogy(newton_func['iteration'], newton_func['residual'], 
                                'b-o', label='Newton', markersize=4, linewidth=2)
        axes2[row, col].semilogy(secant_func['iteration'], secant_func['residual'], 
                                'r-s', label='Secant', markersize=4, linewidth=2)
        
        axes2[row, col].set_title(f'{func} - Residual', fontsize=12)
        axes2[row, col].set_xlabel('Iteration')
        axes2[row, col].set_ylabel('Residual (log scale)')
        axes2[row, col].legend()
        axes2[row, col].grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('residual_convergence_plot.png', dpi=300, bbox_inches='tight')
    plt.show()
    
    print("Plots saved as:")
    print("- root_convergence_plot.png")
    print("- residual_convergence_plot.png")

if __name__ == "__main__":
    create_plots()
