#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <functional>
#include <string>
#include <algorithm>
#include <iterator>
#include <complex>
#include <fstream>
#include <sstream>

// Abstract base class for scientific objects
class ScientificObject {
public:
    virtual void compute() const = 0;
    virtual ~ScientificObject() = default;
};

// Derived class for specific calculations
class VectorCalculation : public ScientificObject {
public:
    void compute() const override {
        std::cout << "Performing vector calculation..." << std::endl;
    }
};

class MatrixCalculation : public ScientificObject {
public:
    void compute() const override {
        std::cout << "Performing matrix calculation..." << std::endl;
    }
};

class TensorCalculation : public ScientificObject {
public:
    void compute() const override {
        std::cout << "Performing tensor calculation..." << std::endl;
    }
};

// Helper function to get type name of ScientificObject
std::string get_object_type_name(const ScientificObject& obj) {
    if (dynamic_cast<const VectorCalculation*>(&obj)) {
        return "VectorCalculation";
    } else if (dynamic_cast<const MatrixCalculation*>(&obj)) {
        return "MatrixCalculation";
    } else if (dynamic_cast<const TensorCalculation*>(&obj)) {
        return "TensorCalculation";
    }
    return "ScientificObject";
}

// Helper function to escape JSON strings
std::string escape_json_string(const std::string& str) {
    std::string result;
    result.reserve(str.length() + 10); // Reserve some extra space
    
    for (char c : str) {
        switch (c) {
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:   result += c; break;
        }
    }
    return result;
}

// Custom exception for duplicate elements
class DuplicateElementException : public std::runtime_error {
public:
    explicit DuplicateElementException(const std::string& message) 
        : std::runtime_error(message) {}
};

// Template function to process scientific data
template <typename T>
void process_data(const T& data) {
    static_assert(std::is_arithmetic<T>::value, "Template type must be arithmetic.");
    std::cout << "Processing data: " << data << std::endl;
}

// Overload for std::string - reverses the string using standard algorithms
void process_data(const std::string& data) {
    // Runtime check for empty string
    if (data.empty()) {
        throw std::invalid_argument("String is empty");
    }
    
    // Optimized approach: use reverse_copy to avoid modifying original
    // This is more efficient for long strings as it avoids the copy + reverse
    std::string reversed;
    reversed.reserve(data.size()); // Pre-allocate to avoid reallocations
    
    // Use standard algorithm to reverse copy the string
    std::reverse_copy(data.begin(), data.end(), std::back_inserter(reversed));
    
    std::cout << "Processing string: " << reversed << std::endl;
}

// Template specialization for std::complex<double>
template <>
void process_data<std::complex<double>>(const std::complex<double>& data) {
    std::cout << "Processing complex number - Real: " << data.real() 
              << ", Imaginary: " << data.imag() << std::endl;
}

// Template class that handles different types of scientific containers
template <typename T>
class ScientificContainer {
private:
    std::list<std::pair<T, std::shared_ptr<ScientificObject>>> container;
    std::unordered_map<T, std::tuple<int, double, std::string, std::function<void()>>> metadata;
    std::unordered_set<T> unique_elements;

public:
    void add_element(const T& key, std::shared_ptr<ScientificObject> obj) {
        if (unique_elements.find(key) != unique_elements.end()) {
            std::ostringstream oss;
            oss << "Duplicate element with key: " << key;
            throw DuplicateElementException(oss.str());
        }
        container.emplace_back(key, obj);
        unique_elements.insert(key);
    }

    void add_metadata(const T& key, const std::tuple<int, double, std::string, std::function<void()>>& data) {
        // Check if key exists in the container
        if (unique_elements.find(key) == unique_elements.end()) {
            throw std::runtime_error("Cannot add metadata for nonexistent key.");
        }
        
        // Check if metadata already exists for this key
        if (metadata.find(key) != metadata.end()) {
            throw std::runtime_error("Metadata for this key already exists.");
        }
        
        metadata[key] = data;
    }

    void compute_all() const {
        for (const auto& [key, obj] : container) {
            try {
                std::cout << "Computing for key: " << key << std::endl;
                obj->compute();
            } catch (const std::exception& e) {
                std::cerr << "Error during computation for key " << key << ": " << e.what() << std::endl;
            }
        }
    }

    void print_metadata() const {
        for (const auto& [key, data] : metadata) {
            std::cout << "Metadata for key " << key << ": ("
                      << std::get<0>(data) << ", "
                      << std::get<1>(data) << ", "
                      << std::get<2>(data) << ")" << std::endl;
            
            // Execute the callback function
            std::get<3>(data)();
        }
    }

    // Filter elements based on a predicate - prints matching keys
    void filter_elements(std::function<bool(const T&)> predicate) const {
        // Handle empty container - no output, no crash
        if (container.empty()) {
            return;
        }
        
        // Iterate through all keys and apply predicate
        for (const auto& [key, obj] : container) {
            if (predicate(key)) {
                std::cout << "Key " << key << " matches the filter" << std::endl;
            }
        }
    }

    // Serialize container to JSON-like format
    void serialize() const {
        const std::string filename = "sci_container.json";
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for serialization: " + filename);
        }
        
        file << "{\n";
        file << "  \"elements\": [\n";
        
        // Serialize elements
        bool first_element = true;
        for (const auto& [key, obj] : container) {
            if (!first_element) {
                file << ",\n";
            }
            file << "    {\n";
            file << "      \"key\": " << key << ",\n";
            file << "      \"type\": \"" << get_object_type_name(*obj) << "\"\n";
            file << "    }";
            first_element = false;
        }
        
        file << "\n  ],\n";
        file << "  \"metadata\": [\n";
        
        // Serialize metadata
        bool first_metadata = true;
        for (const auto& [key, data] : metadata) {
            if (!first_metadata) {
                file << ",\n";
            }
            file << "    {\n";
            file << "      \"key\": " << key << ",\n";
            file << "      \"int_value\": " << std::get<0>(data) << ",\n";
            file << "      \"double_value\": " << std::get<1>(data) << ",\n";
            file << "      \"string_value\": \"" << escape_json_string(std::get<2>(data)) << "\"\n";
            file << "    }";
            first_metadata = false;
        }
        
        file << "\n  ]\n";
        file << "}\n";
        
        file.close();
    }

    // Iterator support for range-based for loops
    auto begin() { return container.begin(); }
    auto end() { return container.end(); }
    auto begin() const { return container.begin(); }
    auto end() const { return container.end(); }
};

#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
int main() {
    try {
        ScientificContainer<int> sci_container;
        
        // Adding elements to the container
        sci_container.add_element(1, std::make_shared<VectorCalculation>());
        sci_container.add_element(2, std::make_shared<MatrixCalculation>());
        sci_container.add_element(3, std::make_shared<TensorCalculation>());
        
        // Adding metadata with callbacks
        sci_container.add_metadata(1, std::make_tuple(42, 3.14, "Vector", 
            std::function<void()>([]() { std::cout << "  -> Vector callback executed!" << std::endl; })));
        sci_container.add_metadata(2, std::make_tuple(7, 2.718, "Matrix", 
            std::function<void()>([]() { std::cout << "  -> Matrix callback executed!" << std::endl; })));
        sci_container.add_metadata(3, std::make_tuple(10, 1.414, "Tensor", 
            std::function<void()>([]() { std::cout << "  -> Tensor callback executed!" << std::endl; })));
        
        // Process some data
        process_data(100);
        process_data(3.1415);
        process_data(std::string("student"));
        process_data(std::complex<double>(3.0, 4.0));
        
        // Compute all elements
        sci_container.compute_all();
        
        // Print metadata
        sci_container.print_metadata();
        
        // Test filter_elements - filter for even numbers
        std::cout << "\nFiltering for even numbers:" << std::endl;
        sci_container.filter_elements([](const int& key) { return key % 2 == 0; });
        
        // Test range-based for loops
        std::cout << "\nTesting range-based for loops:" << std::endl;
        std::cout << "Non-const container iteration:" << std::endl;
        for (const auto& [key, obj] : sci_container) {
            std::cout << "  Key: " << key << ", Object type: " << typeid(*obj).name() << std::endl;
        }
        
        // Test const container iteration
        const auto& const_container = sci_container;
        std::cout << "Const container iteration:" << std::endl;
        int sum = 0;
        for (const auto& [key, obj] : const_container) {
            (void)obj;
            sum += key;
        }
        std::cout << "  Sum of all keys: " << sum << std::endl;
        
        // Test serialization
        std::cout << "\nTesting serialization:" << std::endl;
        sci_container.serialize();
        std::cout << "Container serialized to sci_container.json" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in main: " << e.what() << std::endl;
    }
    
    return 0;
}
#endif
