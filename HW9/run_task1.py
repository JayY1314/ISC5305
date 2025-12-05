#!/usr/bin/env python3
"""
Script to run Task 1 experiments: Serial optimization with different matrix layouts.
Tests N = 2^8, 2^9, ..., 2^14 for three versions: original, row_major, col_major
"""

import subprocess
import sys
import os

def run_experiment(n, version):
    """Run a single experiment and return the output."""
    cmd = ["./mv_serial.x", str(n), version]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        print(f"Error running {cmd}: {e}", file=sys.stderr)
        return None

def main():
    # Check if executable exists
    if not os.path.exists("./mv_serial.x"):
        print("Error: mv_serial.x not found. Please run 'make' first.", file=sys.stderr)
        sys.exit(1)
    
    # N values: 2^8 to 2^14
    n_values = [2**i for i in range(8, 15)]
    versions = ["original", "row_major", "col_major"]
    
    print("Metric,N,Threads,Min,Avg,StdDev")
    
    for n in n_values:
        for version in versions:
            output = run_experiment(n, version)
            if output:
                print(output)
            else:
                print(f"Failed: {n}, {version}", file=sys.stderr)

if __name__ == "__main__":
    main()

