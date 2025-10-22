#!/usr/bin/env python3
"""
Python script to generate histograms for BrainMesh analysis
Creates histograms for triangle edge lengths and vertex-centered areas
"""

import matplotlib.pyplot as plt
import numpy as np
import os

def read_data_file(filename):
    """Read data from a text file, one value per line"""
    try:
        with open(filename, 'r') as f:
            data = [float(line.strip()) for line in f if line.strip()]
        return np.array(data)
    except FileNotFoundError:
        print(f"Error: File {filename} not found")
        return None
    except ValueError as e:
        print(f"Error reading {filename}: {e}")
        return None

def plot_edge_length_histogram(edge_lengths, output_dir="images"):
    """Plot histogram of triangle edge lengths"""
    if edge_lengths is None or len(edge_lengths) == 0:
        print("No edge length data to plot")
        return
    
    # Calculate statistics
    mean_length = np.mean(edge_lengths)
    std_length = np.std(edge_lengths)
    
    # Create histogram
    plt.figure(figsize=(10, 6))
    plt.hist(edge_lengths, bins=50, alpha=0.7, color='skyblue', edgecolor='black')
    
    # Add vertical line for mean
    plt.axvline(mean_length, color='red', linestyle='--', linewidth=2, 
                label=f'Mean: {mean_length:.4f}')
    
    # Add vertical lines for mean ± std
    plt.axvline(mean_length - std_length, color='orange', linestyle=':', linewidth=2,
                label=f'Mean - Std: {mean_length - std_length:.4f}')
    plt.axvline(mean_length + std_length, color='orange', linestyle=':', linewidth=2,
                label=f'Mean + Std: {mean_length + std_length:.4f}')
    
    plt.xlabel('Edge Length')
    plt.ylabel('Frequency')
    plt.title(f'Histogram of Triangle Edge Lengths\n'
              f'Mean: {mean_length:.4f}, Standard Deviation: {std_length:.4f}')
    plt.legend()
    plt.grid(True, alpha=0.3)
    
    # Save the plot
    os.makedirs(output_dir, exist_ok=True)
    plt.savefig(os.path.join(output_dir, 'edge_lengths_histogram.png'), 
                dpi=300, bbox_inches='tight')
    plt.close()
    print(f"Edge lengths histogram saved to {output_dir}/edge_lengths_histogram.png")

def plot_vertex_area_histogram(vertex_areas, output_dir="images"):
    """Plot histogram of vertex-centered areas"""
    if vertex_areas is None or len(vertex_areas) == 0:
        print("No vertex area data to plot")
        return
    
    # Calculate statistics
    mean_area = np.mean(vertex_areas)
    std_area = np.std(vertex_areas)
    
    # Create histogram
    plt.figure(figsize=(10, 6))
    plt.hist(vertex_areas, bins=50, alpha=0.7, color='lightgreen', edgecolor='black')
    
    # Add vertical line for mean
    plt.axvline(mean_area, color='red', linestyle='--', linewidth=2,
                label=f'Mean: {mean_area:.6f}')
    
    # Add vertical lines for mean ± std
    plt.axvline(mean_area - std_area, color='orange', linestyle=':', linewidth=2,
                label=f'Mean - Std: {mean_area - std_area:.6f}')
    plt.axvline(mean_area + std_area, color='orange', linestyle=':', linewidth=2,
                label=f'Mean + Std: {mean_area + std_area:.6f}')
    
    plt.xlabel('Vertex-Centered Area')
    plt.ylabel('Frequency')
    plt.title(f'Histogram of Vertex-Centered Areas\n'
              f'Mean: {mean_area:.6f}, Standard Deviation: {std_area:.6f}')
    plt.legend()
    plt.grid(True, alpha=0.3)
    
    # Save the plot
    os.makedirs(output_dir, exist_ok=True)
    plt.savefig(os.path.join(output_dir, 'vertex_areas_histogram.png'), 
                dpi=300, bbox_inches='tight')
    plt.close()
    print(f"Vertex areas histogram saved to {output_dir}/vertex_areas_histogram.png")

def main():
    """Main function to generate both histograms"""
    print("Generating histograms for BrainMesh analysis...")
    
    # Read edge lengths data
    print("Reading edge lengths data...")
    edge_lengths = read_data_file("edge_lengths.txt")
    
    # Read vertex areas data
    print("Reading vertex areas data...")
    vertex_areas = read_data_file("vertex_areas.txt")
    
    # Generate histograms
    if edge_lengths is not None:
        print("Creating edge lengths histogram...")
        plot_edge_length_histogram(edge_lengths)
    
    if vertex_areas is not None:
        print("Creating vertex areas histogram...")
        plot_vertex_area_histogram(vertex_areas)
    
    print("Histogram generation complete!")

if __name__ == "__main__":
    main()
