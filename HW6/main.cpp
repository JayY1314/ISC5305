#include <iostream>
#include <memory>
#include <sstream>
#include <functional>
#include <chrono>

// Include the core implementation
#include "code.cpp"

// Test functions
void test_task1() {
    std::cout << "=== Task 1 Test: TensorCalculation ===" << std::endl;
    
    try {
        // Test 1: Create TensorCalculation instance
        auto tensor = std::make_shared<TensorCalculation>();
        std::cout << "TensorCalculation instance created successfully" << std::endl;
        
        // Test 2: Call compute() method
        std::cout << "Calling compute() method:" << std::endl;
        tensor->compute();
        
        // Test 3: Add to container
        ScientificContainer<int> container;
        container.add_element(1, tensor);
        std::cout << "TensorCalculation added to container successfully" << std::endl;
        
        // Test 4: Test compute_all() calls TensorCalculation::compute()
        std::cout << "\nCalling compute_all():" << std::endl;
        container.compute_all();
        
        // Test 5: Test range-based for loop
        std::cout << "\nTesting range-based for loop:" << std::endl;
        for (const auto& [key, obj] : container) {
            std::cout << "Key: " << key << ", Object type: TensorCalculation" << std::endl;
        }
        
        std::cout << "\nAll Task 1 tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test_task2() {
    std::cout << "\n=== Task 2 Test: Metadata with Executable Callbacks ===" << std::endl;
    
    try {
        ScientificContainer<int> container;
        
        // Test 1: Add elements first
        container.add_element(1, std::make_shared<VectorCalculation>());
        container.add_element(2, std::make_shared<MatrixCalculation>());
        std::cout << "Elements added to container" << std::endl;
        
        // Test 2: Add metadata with callbacks
        bool callback1_executed = false;
        bool callback2_executed = false;
        
        container.add_metadata(1, std::make_tuple(42, 3.14, "Vector", 
            std::function<void()>([&callback1_executed]() { 
                callback1_executed = true;
                std::cout << "  -> Vector callback executed!" << std::endl; 
            })));
        
        container.add_metadata(2, std::make_tuple(7, 2.718, "Matrix", 
            std::function<void()>([&callback2_executed]() { 
                callback2_executed = true;
                std::cout << "  -> Matrix callback executed!" << std::endl; 
            })));
        
        std::cout << "Metadata with callbacks added successfully" << std::endl;
        
        // Test 3: Verify callbacks are NOT executed during add_metadata
        if (!callback1_executed && !callback2_executed) {
            std::cout << "Callbacks are NOT executed during add_metadata (correct behavior)" << std::endl;
        } else {
            std::cout << "Callbacks executed during add_metadata (incorrect behavior)" << std::endl;
        }
        
        // Test 4: Print metadata and verify callbacks ARE executed
        std::cout << "\nPrinting metadata (callbacks should execute):" << std::endl;
        container.print_metadata();
        
        if (callback1_executed && callback2_executed) {
            std::cout << "Callbacks executed during print_metadata (correct behavior)" << std::endl;
        } else {
            std::cout << "Callbacks not executed during print_metadata" << std::endl;
        }
        
        // Test 5: Test edge case - adding metadata to nonexistent key
        try {
            container.add_metadata(999, std::make_tuple(1, 2.0, "Test", 
                std::function<void()>([]() {})));
            std::cout << "Should have thrown exception for nonexistent key" << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "Correctly threw exception for nonexistent key: " << e.what() << std::endl;
        }
        
        // Test 6: Test edge case - adding duplicate metadata
        try {
            container.add_metadata(1, std::make_tuple(100, 200.0, "Duplicate", 
                std::function<void()>([]() {})));
            std::cout << "Should have thrown exception for duplicate metadata" << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "Correctly threw exception for duplicate metadata: " << e.what() << std::endl;
        }
        
        std::cout << "\nAll Task 2 tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test_task3() {
    std::cout << "\n=== Task 3 Test: DuplicateElementException ===" << std::endl;
    
    try {
        ScientificContainer<int> container;
        
        // Test 1: Add first element successfully
        container.add_element(1, std::make_shared<VectorCalculation>());
        std::cout << "First element added successfully" << std::endl;
        
        // Test 2: Try to add duplicate element - should throw DuplicateElementException
        try {
            container.add_element(1, std::make_shared<MatrixCalculation>());
            std::cout << "Should have thrown DuplicateElementException" << std::endl;
        } catch (const DuplicateElementException& e) {
            std::cout << "Correctly threw DuplicateElementException: " << e.what() << std::endl;
            
            // Verify the message includes the key
            std::string message = e.what();
            if (message.find("Duplicate element with key: 1") != std::string::npos) {
                std::cout << "Exception message includes the duplicate key" << std::endl;
            } else {
                std::cout << "Exception message does not include the key properly" << std::endl;
            }
        } catch (const std::runtime_error& e) {
            std::cout << "Threw generic runtime_error instead of DuplicateElementException: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "Threw unexpected exception type" << std::endl;
        }
        
        // Test 3: Verify exception inheritance
        try {
            container.add_element(1, std::make_shared<TensorCalculation>());
        } catch (const std::runtime_error& e) {
            // This should catch DuplicateElementException since it inherits from runtime_error
            std::cout << "DuplicateElementException is properly derived from std::runtime_error" << std::endl;
        }
        
        // Test 4: Test with string keys
        ScientificContainer<std::string> string_container;
        try {
            string_container.add_element("key1", std::make_shared<VectorCalculation>());
            string_container.add_element("key1", std::make_shared<MatrixCalculation>());
        } catch (const DuplicateElementException& e) {
            std::cout << "DuplicateElementException works with string keys: " << e.what() << std::endl;
        }
        
        // Test 5: Verify container state after exception
        if (container.begin() != container.end()) {
            std::cout << "Container state unchanged after exception (exception safety maintained)" << std::endl;
        } else {
            std::cout << "Container state may have been modified (exception safety issue)" << std::endl;
        }
        
        std::cout << "\nAll Task 3 tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void test_task4() {
    std::cout << "\n=== Task 4 Test: process_data(std::string) ===" << std::endl;
    
    try {
        // Test 1: Test with non-empty string
        std::string test_string = "Hello World";
        std::cout << "Original string: " << test_string << std::endl;
        process_data(test_string);
        std::cout << "After processing: " << test_string << std::endl;
        
        // Test 2: Test with empty string
        std::string empty_string = "";
        std::cout << "\nTesting empty string:" << std::endl;
        process_data(empty_string);
        std::cout << "Empty string processed: " << empty_string << std::endl;
        
        // Test 3: Test with single character
        std::string single_char = "A";
        std::cout << "\nTesting single character:" << std::endl;
        process_data(single_char);
        std::cout << "Single character processed: " << single_char << std::endl;
        
        // Test 4: Test with palindrome
        std::string palindrome = "racecar";
        std::cout << "\nTesting palindrome:" << std::endl;
        process_data(palindrome);
        std::cout << "Palindrome processed: " << palindrome << std::endl;
        
        // Test 5: Test with numbers in string
        std::string number_string = "12345";
        std::cout << "\nTesting number string:" << std::endl;
        process_data(number_string);
        std::cout << "Number string processed: " << number_string << std::endl;
        
        std::cout << "\nAll Task 4 tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void test_task5() {
    std::cout << "\n=== Task 5 Test: process_data(std::complex<double>) ===" << std::endl;
    
    try {
        // Test 1: Test with complex number
        std::complex<double> complex_num(3.0, 4.0);
        std::cout << "Testing complex number (3.0 + 4.0i):" << std::endl;
        process_data(complex_num);
        
        // Test 2: Test with another complex number
        std::complex<double> complex_num2(-1.5, 2.7);
        std::cout << "\nTesting complex number (-1.5 + 2.7i):" << std::endl;
        process_data(complex_num2);
        
        // Test 3: Test with zero imaginary part
        std::complex<double> real_only(5.0, 0.0);
        std::cout << "\nTesting complex with zero imaginary part (5.0 + 0.0i):" << std::endl;
        process_data(real_only);
        
        // Test 4: Test with zero real part
        std::complex<double> imag_only(0.0, 3.14);
        std::cout << "\nTesting complex with zero real part (0.0 + 3.14i):" << std::endl;
        process_data(imag_only);
        
        // Test 5: Verify template function still works for arithmetic types
        int int_val = 42;
        std::cout << "\nTesting template function with int (should use generic version):" << std::endl;
        process_data(int_val);
        std::cout << "Int value after processing: " << int_val << std::endl;
        
        // Test 6: Verify string overload still works
        std::string test_str = "Template";
        std::cout << "\nTesting string overload still works:" << std::endl;
        process_data(test_str);
        std::cout << "String after processing: " << test_str << std::endl;
        
        // Test 7: Performance test with multiple complex numbers
        std::cout << "\nPerformance test with 1000 complex numbers:" << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        
        std::vector<std::complex<double>> complex_numbers;
        for (int i = 0; i < 1000; ++i) {
            complex_numbers.emplace_back(i * 0.1, i * 0.2);
        }
        
        for (auto& num : complex_numbers) {
            process_data(num);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "1000 complex numbers processed in " << duration.count() << " microseconds" << std::endl;
        
        std::cout << "\nAll Task 5 tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void test_task6() {
    std::cout << "\n=== Task 6 Test: Lambda-Based Filtering ===" << std::endl;
    
    try {
        ScientificContainer<int> container;
        
        // Add some test elements
        for (int i = 1; i <= 10; ++i) {
            container.add_element(i, std::make_shared<VectorCalculation>());
        }
        
        std::cout << "Added 10 elements to container" << std::endl;
        
        // Test 1: Filter even numbers
        std::cout << "\nFiltering even numbers:" << std::endl;
        container.filter_elements([](const int& key) { return key % 2 == 0; });
        
        // Test 2: Filter numbers greater than 5
        std::cout << "\nFiltering numbers greater than 5:" << std::endl;
        container.filter_elements([](const int& key) { return key > 5; });
        
        // Test 3: Filter numbers divisible by 3
        std::cout << "\nFiltering numbers divisible by 3:" << std::endl;
        container.filter_elements([](const int& key) { return key % 3 == 0; });
        
        // Test 4: Filter with range
        std::cout << "\nFiltering numbers between 3 and 7:" << std::endl;
        container.filter_elements([](const int& key) { return key >= 3 && key <= 7; });
        
        // Test 5: Filter with complex condition
        std::cout << "\nFiltering numbers that are prime (simple check):" << std::endl;
        container.filter_elements([](const int& key) { 
            if (key < 2) return false;
            for (int i = 2; i * i <= key; ++i) {
                if (key % i == 0) return false;
            }
            return true;
        });
        
        // Test 6: Performance test with large container
        std::cout << "\nPerformance test with 1000 elements:" << std::endl;
        ScientificContainer<int> large_container;
        for (int i = 1; i <= 1000; ++i) {
            large_container.add_element(i, std::make_shared<MatrixCalculation>());
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        large_container.filter_elements([](const int& key) { return key % 100 == 0; });
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "1000 elements filtered in " << duration.count() << " microseconds" << std::endl;
        
        std::cout << "\nAll Task 6 tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void test_task7() {
    std::cout << "\n=== Task 7 Test: Range-Based For Loop Support ===" << std::endl;
    
    try {
        ScientificContainer<int> container;
        
        // Add some test elements
        container.add_element(1, std::make_shared<VectorCalculation>());
        container.add_element(2, std::make_shared<MatrixCalculation>());
        container.add_element(3, std::make_shared<TensorCalculation>());
        
        std::cout << "Added 3 elements to container" << std::endl;
        
        // Test 1: Non-const range-based for loop
        std::cout << "\nTesting non-const range-based for loop:" << std::endl;
        int count = 0;
        for (auto& [key, obj] : container) {
            std::cout << "Key: " << key << ", Type: " << get_object_type_name(*obj) << std::endl;
            count++;
        }
        
        if (count == 3) {
            std::cout << "Non-const range-based for loop works correctly" << std::endl;
        } else {
            std::cout << "Non-const range-based for loop failed" << std::endl;
        }
        
        // Test 2: Const range-based for loop
        std::cout << "\nTesting const range-based for loop:" << std::endl;
        const ScientificContainer<int>& const_container = container;
        count = 0;
        for (const auto& [key, obj] : const_container) {
            std::cout << "Key: " << key << ", Type: " << get_object_type_name(*obj) << std::endl;
            count++;
        }
        
        if (count == 3) {
            std::cout << "Const range-based for loop works correctly" << std::endl;
        } else {
            std::cout << "Const range-based for loop failed" << std::endl;
        }
        
        // Test 3: Sum operation using range-based for loop
        std::cout << "\nTesting sum operation using range-based for loop:" << std::endl;
        int sum = 0;
        for (const auto& [key, obj] : container) {
            sum += key;
        }
        std::cout << "Sum of all keys: " << sum << std::endl;
        
        if (sum == 6) { // 1 + 2 + 3
            std::cout << "Sum operation works correctly (sum = " << sum << ")" << std::endl;
        } else {
            std::cout << "Sum operation failed (sum = " << sum << ")" << std::endl;
        }
        
        // Test 4: Test with different key types
        std::cout << "\nTesting with string keys:" << std::endl;
        ScientificContainer<std::string> string_container;
        string_container.add_element("first", std::make_shared<VectorCalculation>());
        string_container.add_element("second", std::make_shared<MatrixCalculation>());
        
        for (const auto& [key, obj] : string_container) {
            std::cout << "String key: " << key << ", Type: " << get_object_type_name(*obj) << std::endl;
        }
        
        // Test 5: Test insertion order preservation
        std::cout << "\nTesting insertion order preservation:" << std::endl;
        ScientificContainer<int> order_test;
        for (int i = 10; i >= 1; --i) {
            order_test.add_element(i, std::make_shared<VectorCalculation>());
        }
        
        std::vector<int> collected_keys;
        for (const auto& [key, obj] : order_test) {
            collected_keys.push_back(key);
        }
        
        bool order_preserved = true;
        for (size_t i = 0; i < collected_keys.size() - 1; ++i) {
            if (collected_keys[i] > collected_keys[i + 1]) {
                order_preserved = false;
                break;
            }
        }
        
        if (order_preserved) {
            std::cout << "Insertion order preserved correctly" << std::endl;
        } else {
            std::cout << "Insertion order not preserved" << std::endl;
        }
        
        // Test 6: Test with large container
        std::cout << "\nTesting with large container (1000 elements):" << std::endl;
        ScientificContainer<int> large_container;
        for (int i = 1; i <= 1000; ++i) {
            large_container.add_element(i, std::make_shared<VectorCalculation>());
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        int large_sum = 0;
        for (const auto& [key, obj] : large_container) {
            large_sum += key;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "Large container iteration works (sum = " << large_sum << ", time = " << duration.count() << " μs)" << std::endl;
        
        // Test 7: Test with structured bindings
        std::cout << "\nTesting structured bindings with different key types:" << std::endl;
        ScientificContainer<std::string> mixed_container;
        mixed_container.add_element("alpha", std::make_shared<VectorCalculation>());
        mixed_container.add_element("beta", std::make_shared<MatrixCalculation>());
        
        for (const auto& [key, obj] : mixed_container) {
            std::cout << "Key: " << key << ", Object type: " << get_object_type_name(*obj) << std::endl;
        }
        
        std::cout << "\nAll Task 7 tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void test_task8() {
    std::cout << "\n=== Task 8 Test: JSON-Like Serialization ===" << std::endl;
    
    try {
        ScientificContainer<int> container;
        
        // Add some test elements
        container.add_element(1, std::make_shared<VectorCalculation>());
        container.add_element(2, std::make_shared<MatrixCalculation>());
        container.add_element(3, std::make_shared<TensorCalculation>());
        
        // Add metadata
        container.add_metadata(1, std::make_tuple(42, 3.14, "Vector", std::function<void()>([](){})));
        container.add_metadata(2, std::make_tuple(7, 2.718, "Matrix", std::function<void()>([](){})));
        
        std::cout << "Added elements and metadata to container" << std::endl;
        
        // Test 1: Basic serialization
        std::cout << "\nTesting basic serialization:" << std::endl;
        container.serialize("test_container.json");
        
        // Check if file was created
        std::ifstream file("test_container.json");
        if (file.good()) {
            std::cout << "Serialization file created successfully" << std::endl;
        } else {
            std::cout << "Serialization file not found" << std::endl;
        }
        
        // Test 2: Verify file content structure
        std::cout << "\nVerifying file content structure:" << std::endl;
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        
        bool has_elements = content.find("\"elements\"") != std::string::npos;
        bool has_metadata = content.find("\"metadata\"") != std::string::npos;
        bool has_type_vector = content.find("VectorCalculation") != std::string::npos;
        bool has_type_matrix = content.find("MatrixCalculation") != std::string::npos;
        bool has_type_tensor = content.find("TensorCalculation") != std::string::npos;
        
        if (has_elements && has_metadata && has_type_vector && has_type_matrix && has_type_tensor) {
            std::cout << "File structure is correct" << std::endl;
        } else {
            std::cout << "File structure is incorrect" << std::endl;
        }
        
        // Test 3: Test with empty container
        std::cout << "\nTesting empty container serialization:" << std::endl;
        ScientificContainer<int> empty_container;
        empty_container.serialize("empty_container.json");
        
        std::ifstream empty_file("empty_container.json");
        if (empty_file.good()) {
            std::cout << "Empty container serialized correctly" << std::endl;
        } else {
            std::cout << "Empty container serialization failed" << std::endl;
        }
        empty_file.close();
        
        // Test 4: Test with string keys
        std::cout << "\nTesting serialization with string keys:" << std::endl;
        ScientificContainer<std::string> string_container;
        string_container.add_element("key1", std::make_shared<VectorCalculation>());
        string_container.add_element("key2", std::make_shared<MatrixCalculation>());
        string_container.serialize("string_container.json");
        
        std::ifstream string_file("string_container.json");
        if (string_file.good()) {
            std::cout << "String keys serialized correctly" << std::endl;
        } else {
            std::cout << "String keys serialization failed" << std::endl;
        }
        string_file.close();
        
        // Test 5: Test file overwrite
        std::cout << "\nTesting file overwrite:" << std::endl;
        container.serialize("test_container.json"); // Overwrite existing file
        std::ifstream overwrite_file("test_container.json");
        if (overwrite_file.good()) {
            std::cout << "File overwrite works correctly" << std::endl;
        } else {
            std::cout << "File overwrite failed" << std::endl;
        }
        overwrite_file.close();
        
        std::cout << "\nAll Task 8 tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "=== ScientificContainer Comprehensive Test Suite ===" << std::endl;
    std::cout << "Author: Zhaoyang Xu" << std::endl;
    std::cout << "Date: " << __DATE__ << " " << __TIME__ << std::endl;
    std::cout << "=================================================" << std::endl;
    
    // Run all test tasks
    test_task1();
    test_task2();
    test_task3();
    test_task4();
    test_task5();
    test_task6();
    test_task7();
    test_task8();
    
    std::cout << "\n=================================================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    std::cout << "Generated files: test_container.json, empty_container.json, string_container.json" << std::endl;
    
    return 0;
}