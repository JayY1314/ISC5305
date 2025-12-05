#!/bin/bash
# Script to run all experiments and generate plots

echo "Building all executables..."
make clean
make

echo ""
echo "Running Task 1 experiments..."
python3 run_task1.py > task1_results.csv

echo ""
echo "Running Task 2 experiments..."
python3 run_task2.py > task2_results.csv

echo ""
echo "Running Task 3 experiments..."
python3 run_task3.py > task3_results.csv

echo ""
echo "Generating plots..."
python3 plot_results.py

echo ""
echo "All experiments completed!"
echo "Results saved in: task1_results.csv, task2_results.csv, task3_results.csv"
echo "Plots saved as: plot1_*.png, plot2_*.png, plot3_*.png"

