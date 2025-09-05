import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

def read_series(path, label):
    """
    Read a series of data from a CSV file.
    
    Args:
        path (str): Path to the CSV file
        label (str): Label for the series in the plot
    
    Returns:
        tuple: (data values, label)
    """
    df = pd.read_csv(path)
    return df['value'], label

def main():
    """
    Main function to create plots from normalized data files.
    Expects at least 2 CSV files as command line arguments.
    """
    if len(sys.argv) < 3:
        print('Usage: python plot_normalized.py <normalized_file1> <normalized_file2> [more ...]')
        sys.exit(1)
    
    plt.figure(figsize=(12, 8))
    
    for i, path in enumerate(sys.argv[1:], 1):
        if not os.path.exists(path):
            print(f'Warning: File {path} does not exist, skipping...')
            continue
            
        label = f'Series {i} ({os.path.basename(path).replace("_1000_normalized.csv", "")})'
        y, lab = read_series(path, label)
        plt.plot(y, label=lab, linewidth=1.5, alpha=0.8)
        
        # Print statistics
        print(f'Series {i}: {len(y)} data points')
        print(f'  Min: {min(y):.4f}, Max: {max(y):.4f}')
        print(f'  Mean: {sum(y)/len(y):.4f}')
        print()
    
    plt.title('Normalized 1-D Data (Min-Max to [0,1]) - 1000 Samples Each', fontsize=14, fontweight='bold')
    plt.xlabel('Index', fontsize=12)
    plt.ylabel('Normalized Value', fontsize=12)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    
    # Save the plot
    plt.savefig('normalized_data_1000_samples.png', dpi=300, bbox_inches='tight')
    print('Plot saved as: normalized_data_1000_samples.png')
    
    # Show the plot
    plt.show()

if __name__ == '__main__':
    main()
