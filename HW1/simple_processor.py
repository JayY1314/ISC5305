import pandas as pd
import numpy as np
import sys

def process_data(csv_file, num_lines, column_index):
    """
    Process CSV data file and generate normalized output.
    
    Args:
        csv_file (str): Path to CSV file
        num_lines (int): Number of lines to process
        column_index (int): Column index to process
    """
    # Read data
    df = pd.read_csv(csv_file)
    data = df.iloc[:num_lines, column_index].tolist()
    
    # Calculate statistics
    mean = np.mean(data)
    std_dev = np.std(data, ddof=1)
    
    # Normalize data
    min_val = min(data)
    max_val = max(data)
    normalized = [(x - min_val) / (max_val - min_val) for x in data]
    
    # Write results
    output_file = csv_file.replace('.csv', '_normalized.txt')
    with open(output_file, 'w') as f:
        f.write(f'Number of parameters read: 3\n')
        f.write(f'Mean: {mean:.2f}\n')
        f.write(f'Standard deviation: {std_dev:.2f}\n')
        f.write('Normalized data:\n')
        for value in normalized:
            f.write(f'{value:.2f}\n')
    
    # Create CSV for plotting
    csv_output = csv_file.replace('.csv', '_normalized.csv')
    df_normalized = pd.DataFrame({'value': normalized})
    df_normalized.to_csv(csv_output, index=False)
    
    print(f'Processed {csv_file}: {len(data)} points, Mean: {mean:.2f}, Std: {std_dev:.2f}')
    return csv_output

if __name__ == '__main__':
    # Process both data files
    abalone_csv = process_data('AbaloneAge.csv', 1000, 0)
    stress_csv = process_data('StressLevel.csv', 1000, 0)
    
    print(f'\\nTo plot the results, run:')
    print(f'python plot_normalized.py {abalone_csv} {stress_csv}')
