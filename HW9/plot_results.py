#!/usr/bin/env python3
"""
Script to plot results from all three tasks.
Generates Plot 1, Plot 2, and Plot 3 as required.
"""

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import sys
import os

def plot_task1(data_file="task1_results.csv"):
    """Plot 1: Execution Time vs. Matrix Size (Log-Log plot)"""
    if not os.path.exists(data_file):
        print(f"Error: {data_file} not found. Run run_task1.py first.", file=sys.stderr)
        return
    
    df = pd.read_csv(data_file)
    
    plt.figure(figsize=(10, 6))
    
    for version in ["original", "row_major", "col_major"]:
        subset = df[df['Metric'].str.contains(version, case=False)]
        if not subset.empty:
            plt.loglog(subset['N'], subset['Min'], marker='o', label=f'{version}')
    
    plt.xlabel('Matrix Size (N)', fontsize=12)
    plt.ylabel('Execution Time (ms)', fontsize=12)
    plt.title('Plot 1: Execution Time vs. Matrix Size (Log-Log)', fontsize=14)
    plt.legend()
    plt.grid(True, which="both", ls="-", alpha=0.3)
    plt.tight_layout()
    plt.savefig('plot1_execution_time_vs_matrix_size.png', dpi=300)
    print("Saved: plot1_execution_time_vs_matrix_size.png")

def plot_task2(data_file="task2_results.csv"):
    """Plot 2: Speedup (T1/TP) vs. Number of Threads"""
    if not os.path.exists(data_file):
        print(f"Error: {data_file} not found. Run run_task2.py first.", file=sys.stderr)
        return
    
    df = pd.read_csv(data_file)
    
    # Get T1 (single thread time) for each method
    t1_static = df[(df['Metric'].str.contains('OpenMP_static')) & (df['Threads'] == 1)]['Min'].values
    t1_dynamic = df[(df['Metric'].str.contains('OpenMP_dynamic')) & (df['Threads'] == 1)]['Min'].values
    t1_guided = df[(df['Metric'].str.contains('OpenMP_guided')) & (df['Threads'] == 1)]['Min'].values
    t1_threads = df[(df['Metric'].str.contains('Threads')) & (df['Threads'] == 1)]['Min'].values
    
    if len(t1_static) == 0 or len(t1_dynamic) == 0 or len(t1_guided) == 0 or len(t1_threads) == 0:
        print("Error: T1 (single thread) data not found.", file=sys.stderr)
        return
    
    t1_static = t1_static[0]
    t1_dynamic = t1_dynamic[0]
    t1_guided = t1_guided[0]
    t1_threads = t1_threads[0]
    
    plt.figure(figsize=(10, 6))
    
    # OpenMP Static
    static_data = df[df['Metric'].str.contains('OpenMP_static')]
    if not static_data.empty:
        speedup_static = t1_static / static_data['Min']
        plt.plot(static_data['Threads'], speedup_static, marker='o', label='OpenMP Static')
    
    # OpenMP Dynamic
    dynamic_data = df[df['Metric'].str.contains('OpenMP_dynamic')]
    if not dynamic_data.empty:
        speedup_dynamic = t1_dynamic / dynamic_data['Min']
        plt.plot(dynamic_data['Threads'], speedup_dynamic, marker='s', label='OpenMP Dynamic')
    
    # OpenMP Guided
    guided_data = df[df['Metric'].str.contains('OpenMP_guided')]
    if not guided_data.empty:
        speedup_guided = t1_guided / guided_data['Min']
        plt.plot(guided_data['Threads'], speedup_guided, marker='^', label='OpenMP Guided')
    
    # Threads
    threads_data = df[df['Metric'].str.contains('Threads')]
    if not threads_data.empty:
        speedup_threads = t1_threads / threads_data['Min']
        plt.plot(threads_data['Threads'], speedup_threads, marker='d', label='std::thread (Atomic)')
    
    # Ideal speedup line
    thread_counts = sorted(static_data['Threads'].unique())
    plt.plot(thread_counts, thread_counts, 'k--', label='Ideal Speedup', alpha=0.5)
    
    plt.xlabel('Number of Threads', fontsize=12)
    plt.ylabel('Speedup (T1/TP)', fontsize=12)
    plt.title('Plot 2: Speedup vs. Number of Threads', fontsize=14)
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('plot2_speedup_vs_threads.png', dpi=300)
    print("Saved: plot2_speedup_vs_threads.png")

def plot_task3(data_file="task3_results.csv"):
    """Plot 3: Execution Time vs. Number of Processes"""
    if not os.path.exists(data_file):
        print(f"Error: {data_file} not found. Run run_task3.py first.", file=sys.stderr)
        return
    
    df = pd.read_csv(data_file)
    
    plt.figure(figsize=(10, 6))
    
    # Contiguous (Unbalanced)
    contiguous_data = df[df['Metric'].str.contains('contiguous')]
    if not contiguous_data.empty:
        plt.plot(contiguous_data['Processes'], contiguous_data['Min'], 
                marker='o', label='MPI Contiguous Scatter (Unbalanced)')
    
    # Cyclic (Balanced)
    cyclic_data = df[df['Metric'].str.contains('cyclic')]
    if not cyclic_data.empty:
        plt.plot(cyclic_data['Processes'], cyclic_data['Min'], 
                marker='s', label='MPI Optimized Strategy (Balanced)')
    
    plt.xlabel('Number of Processes', fontsize=12)
    plt.ylabel('Execution Time (ms)', fontsize=12)
    plt.title('Plot 3: Execution Time vs. Number of Processes', fontsize=14)
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('plot3_execution_time_vs_processes.png', dpi=300)
    print("Saved: plot3_execution_time_vs_processes.png")

def main():
    if len(sys.argv) > 1:
        task = sys.argv[1]
        if task == "1":
            plot_task1()
        elif task == "2":
            plot_task2()
        elif task == "3":
            plot_task3()
        else:
            print("Usage: python plot_results.py [1|2|3]")
    else:
        # Plot all
        plot_task1()
        plot_task2()
        plot_task3()

if __name__ == "__main__":
    main()

