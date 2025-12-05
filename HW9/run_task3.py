#!/usr/bin/env python3
"""
Script to run Task 3 experiments: MPI with contiguous and cyclic distribution.
Tests different process counts for both strategies.
"""

import subprocess
import sys
import os

def run_mpi_experiment(n, num_processes, strategy):
    """Run MPI experiment."""
    cmd = ["mpirun", "-np", str(num_processes), "./mv_mpi.x", str(n), strategy]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        print(f"Error running {cmd}: {e}", file=sys.stderr)
        return None

def main():
    # Check if executable exists
    if not os.path.exists("./mv_mpi.x"):
        print("Error: mv_mpi.x not found. Please run 'make' first.", file=sys.stderr)
        sys.exit(1)
    
    # Fixed N (choose 2^13 or 2^14)
    n = 2**13  # 8192
    # n = 2**14  # 16384
    
    # Process counts: 2, 4, 8, ... up to machine limit
    process_counts = [2, 4, 8, 16]  # Adjust based on your machine
    
    strategies = ["contiguous", "cyclic"]
    
    print("Metric,N,Processes,Min,Avg,StdDev")
    
    for num_processes in process_counts:
        for strategy in strategies:
            output = run_mpi_experiment(n, num_processes, strategy)
            if output:
                print(output)

if __name__ == "__main__":
    main()

