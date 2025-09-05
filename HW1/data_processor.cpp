#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>

/**
 * Function to read parameters from input file
 * @param filename: name of the parameter file
 * @param dataFile: reference to store data filename
 * @param numLines: reference to store number of lines to read
 * @param column: reference to store column index to read
 * @return true if successful, false otherwise
 */
bool readParameters(const std::string& filename, std::string& dataFile, int& numLines, int& column) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open parameter file: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    int paramCount = 0;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            // Remove whitespace
            key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
            value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
            
            if (key == "data_file") {
                dataFile = value;
                paramCount++;
            } else if (key == "num_lines") {
                numLines = std::stoi(value);
                paramCount++;
            } else if (key == "column") {
                column = std::stoi(value);
                paramCount++;
            }
        }
    }
    
    file.close();
    return paramCount == 3;
}

/**
 * Function to read data from CSV file
 * @param filename: name of the data file
 * @param numLines: number of lines to read (0 means all)
 * @param column: column index to read (0-based)
 * @return vector of data values
 */
std::vector<double> readData(const std::string& filename, int numLines, int column) {
    std::vector<double> data;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open data file: " << filename << std::endl;
        return data;
    }
    
    std::string line;
    int lineCount = 0;
    bool firstLine = true;
    
    while (std::getline(file, line) && (numLines == 0 || lineCount < numLines)) {
        if (firstLine) {
            firstLine = false; // Skip header
            continue;
        }
        
        std::istringstream iss(line);
        std::string cell;
        int currentColumn = 0;
        
        while (std::getline(iss, cell, ',')) {
            if (currentColumn == column) {
                try {
                    double value = std::stod(cell);
                    data.push_back(value);
                    break;
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing value: " << cell << std::endl;
                }
            }
            currentColumn++;
        }
        lineCount++;
    }
    
    file.close();
    return data;
}

/**
 * Function to calculate mean of data
 * @param data: vector of data values
 * @return mean value
 */
double calculateMean(const std::vector<double>& data) {
    if (data.empty()) return 0.0;
    
    double sum = 0.0;
    for (double value : data) {
        sum += value;
    }
    return sum / data.size();
}

/**
 * Function to calculate standard deviation of data
 * @param data: vector of data values
 * @param mean: mean value of the data
 * @return standard deviation
 */
double calculateStdDev(const std::vector<double>& data, double mean) {
    if (data.size() <= 1) return 0.0;
    
    double sumSquaredDiff = 0.0;
    for (double value : data) {
        double diff = value - mean;
        sumSquaredDiff += diff * diff;
    }
    
    return std::sqrt(sumSquaredDiff / (data.size() - 1));
}

/**
 * Function to normalize data to range [0, 1]
 * @param data: vector of data values
 * @return vector of normalized values
 */
std::vector<double> normalizeData(const std::vector<double>& data) {
    if (data.empty()) return data;
    
    double minVal = *std::min_element(data.begin(), data.end());
    double maxVal = *std::max_element(data.begin(), data.end());
    
    if (maxVal == minVal) {
        // All values are the same, set to 0.5
        return std::vector<double>(data.size(), 0.5);
    }
    
    std::vector<double> normalized;
    for (double value : data) {
        double normalizedValue = (value - minVal) / (maxVal - minVal);
        normalized.push_back(normalizedValue);
    }
    
    return normalized;
}

/**
 * Function to write results to output file
 * @param filename: name of the output file
 * @param numParams: number of parameters read
 * @param mean: mean value
 * @param stdDev: standard deviation
 * @param normalizedData: normalized data values
 */
void writeResults(const std::string& filename, int numParams, double mean, double stdDev, 
                 const std::vector<double>& normalizedData) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot create output file: " << filename << std::endl;
        return;
    }
    
    file << std::fixed << std::setprecision(2);
    file << "Number of parameters read: " << numParams << std::endl;
    file << "Mean: " << mean << std::endl;
    file << "Standard deviation: " << stdDev << std::endl;
    file << "Normalized data:" << std::endl;
    
    for (double value : normalizedData) {
        file << value << std::endl;
    }
    
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <parameter_file>" << std::endl;
        return 1;
    }
    
    std::string paramFile = argv[1];
    std::string dataFile;
    int numLines, column;
    
    // Read parameters
    if (!readParameters(paramFile, dataFile, numLines, column)) {
        std::cerr << "Failed to read parameters from: " << paramFile << std::endl;
        return 1;
    }
    
    // Read data
    std::vector<double> data = readData(dataFile, numLines, column);
    if (data.empty()) {
        std::cerr << "No data read from file: " << dataFile << std::endl;
        return 1;
    }
    
    // Calculate statistics
    double mean = calculateMean(data);
    double stdDev = calculateStdDev(data, mean);
    
    // Normalize data
    std::vector<double> normalizedData = normalizeData(data);
    
    // Generate output filename
    std::string outputFile = dataFile.substr(0, dataFile.find_last_of('.')) + "_normalized.txt";
    
    // Write results
    writeResults(outputFile, 3, mean, stdDev, normalizedData);
    
    std::cout << "Processing completed for: " << dataFile << std::endl;
    std::cout << "Output written to: " << outputFile << std::endl;
    
    return 0;
}
