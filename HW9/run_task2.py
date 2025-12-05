#!/usr/bin/env python3
"""
Script to run Task 2 experiments: OpenMP and Threads with triangular workload.
Tests different thread counts for OpenMP (static, dynamic, guided) and Threads.
"""

import subprocess
import sys
import os

def run_openmp_experiment(n, num_threads, schedule, chunk_size=1):
    """Run OpenMP experiment."""
    cmd = ["./mv_openmp.x", str(n), str(num_threads), schedule, str(chunk_size)]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        print(f"Error running {cmd}: {e}", file=sys.stderr)
        return None

def run_threads_experiment(n, num_threads):
    """Run Threads experiment."""
    cmd = ["./mv_threads.x", str(n), str(num_threads)]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        print(f"Error running {cmd}: {e}", file=sys.stderr)
        return None

def main():
    # Check if executables exist
    if not os.path.exists("./mv_openmp.x"):
        print("Error: mv_openmp.x not found. Please run 'make' first.", file=sys.stderr)
        sys.exit(1)
    if not os.path.exists("./mv_threads.x"):
        print("Error: mv_threads.x not found. Please run 'make' first.", file=sys.stderr)
        sys.exit(1)
    
    # Fixed N (choose 2^13 or 2^14)
    n = 2**13  # 8192
    # n = 2**14  # 16384
    
    # Thread counts: 1, 2, 4, 8, 16, ... up to P_max (adjust based on your machine)
    # P_max should be at least 2x physical cores
    thread_counts = [1, 2, 4, 8, 16, 32]  # Adjust based on your machine
    
    print("Metric,N,Threads,Min,Avg,StdDev")
    
    # OpenMP experiments
    for num_threads in thread_counts:
        for schedule in ["static", "dynamic", "guided"]:
            chunk_size = 1 if schedule == "dynamic" else 1
            output = run_openmp_experiment(n, num_threads, schedule, chunk_size)
            if output:
                print(output)
    
    # Threads experiments
    for num_threads in thread_counts:
        output = run_threads_experiment(n, num_threads)
        if output:
            print(output)

if __name__ == "__main__":
    main()

