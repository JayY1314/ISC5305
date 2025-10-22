// tests.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <complex>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "doctest.h"

// ---- Student headers ----
// Adjust these includes/namespaces to match your file layout.
#include "code.cpp"  // or: #include "main.hpp"

struct TestBanner {
    std::string name;
    explicit TestBanner(std::string n) : name(std::move(n)) {
        std::cout << "\n========================================\n"
                  << "[Running] " << name << "\n";
        //   << "========================================\n";
    }
    ~TestBanner() {
        std::cout << "========================================\n";
    }
};

// ---- Small helpers for capturing std::cout ----
struct CoutCapture {
    std::streambuf* old = nullptr;
    std::ostringstream oss;
    CoutCapture() {
        old = std::cout.rdbuf(oss.rdbuf());
    }
    ~CoutCapture() {
        std::cout.rdbuf(old);
    }
    std::string str() const {
        return oss.str();
    }
};

void diagnostics(const std::string& task, const std::string& output) {
    std::vector<std::string> error_indicators = {"ERROR",
                                                 "Error",
                                                 "error",
                                                 "EXCEPTION",
                                                 "Exception",
                                                 "exception",
                                                 "INVALID",
                                                 "Invalid",
                                                 "invalid",
                                                 "FAIL",
                                                 "Fail",
                                                 "fail",
                                                 "THROW",
                                                 "Throw",
                                                 "throw",
                                                 "already exists",
                                                 "String is empty",
                                                 "Element already exists",
                                                 "Metadata for the key",
                                                 "Exception caught",
                                                 "Error during computation"};
    bool has_error = false;
    for (const auto& indicator : error_indicators) {
        if (output.find(indicator) != std::string::npos) {
            has_error = true;
            break;
        }
    }

    if (has_error) {
        std::cerr << "cap.str(): " << output << std::endl;
    } else {
        std::cerr << "[Passed] " << task << std::endl;
    }
}

// ---- Traits / helpers ----
template <typename F, typename... Args>
using is_invocable_t = std::is_invocable<F, Args...>;

// If process_data is found by ADL / overload resolution:
template <typename T>
using process_sig = decltype(process_data(std::declval<T const&>()));

// template <typename T>
// constexpr bool has_process_data_v = std::is_detected_v<process_sig, T>;

// Provide a type-name helper (adjust if your code already has one)
static const char* obj_type_of(const ScientificObject& obj) {
    if (dynamic_cast<const VectorCalculation*>(&obj)) return "VectorCalculation";
    if (dynamic_cast<const MatrixCalculation*>(&obj)) return "MatrixCalculation";
    if (dynamic_cast<const TensorCalculation*>(&obj)) return "TensorCalculation";
    return "ScientificObject";
}

// ------------------------------
// Task 1: TensorCalculation exists and is usable
// ------------------------------
TEST_CASE("Task 1: TensorCalculation::compute prints a message and is storable") {
    TestBanner _banner("Task 1 — TensorCalculation computes and can be stored");
    auto t = std::make_shared<TensorCalculation>();
    CHECK(t != nullptr);

    CoutCapture cap;
    t->compute();
    auto out = cap.str();
    CHECK(out.find("tensor") != std::string::npos);  // case-insensitive not enforced

    // Store in container
    ScientificContainer<int> sc;
    sc.add_element(6, t);

    // Ensure it’s really in there via range-for (Task 7 covers iteration fully)
    bool found = false;
    for (const auto& [k, obj] : sc) {
        if (k == 6 && obj.get() == t.get()) found = true;
    }
    CHECK(found);
    diagnostics("Task 1", cap.str());
}

// ------------------------------
// Task 2: Executable metadata stored and executed on print (not on add)
// ------------------------------
TEST_CASE("Task 2: metadata callback executes on print, not on add") {
    TestBanner _banner("Task 2 — metadata callback executes on print, not on add");
    ScientificContainer<int> sc;
    CoutCapture cap;
    sc.add_element(1, std::make_shared<VectorCalculation>());

    bool ran = false;
    auto cb = [&]() { ran = true; };

    // Add metadata should NOT execute the callback
    sc.add_metadata(1, std::make_tuple(42, 3.14, std::string("Vector"), std::function<void()>(cb)));
    CHECK(ran == false);

    // Now printing metadata SHOULD execute callbacks
    {
        CoutCapture cap;
        sc.print_metadata();
        (void)cap;  // ignore text, the side-effect is 'ran'
    }
    CHECK(ran == true);
    diagnostics("Task 2", cap.str());
}

// ------------------------------
// Task 3: DuplicateElementException is thrown (std::runtime_error derived)
// ------------------------------
// TEST_CASE("Task 3: adding duplicate key throws DuplicateElementException (runtime_error)") {
//     TestBanner _banner(
//         "Task 3 — adding duplicate key throws DuplicateElementException (runtime_error)");
//     ScientificContainer<int> sc;
//     sc.add_element(2, std::make_shared<MatrixCalculation>());

//     try {
//         sc.add_element(2, std::make_shared<VectorCalculation>());  // duplicate
//         FAIL_CHECK("Expected DuplicateElementException");
//     } catch (const DuplicateElementException& e) {
//         // should derive from std::runtime_error
//         const std::runtime_error* as_rt = dynamic_cast<const std::runtime_error*>(&e);
//         CHECK(as_rt != nullptr);

//         std::string msg = e.what();
//         CHECK(msg.find("Duplicate element") != std::string::npos);
//         CHECK(msg.find("2") != std::string::npos);
//     } catch (...) {
//         FAIL_CHECK("Wrong exception type");
//     }
// }

// ------------------------------
// Task 4: process_data(std::string) reverses non-empty, rejects empty
// ------------------------------
TEST_CASE("Task 4: process_data(std::string) reverses string; empty rejected") {
    TestBanner _banner("Task 4 — process_data(std::string) reverses string; empty rejected");
    CoutCapture cap;
    {
        process_data(std::string("student"));
        auto out = cap.str();
        // Should contain "tneduts" somewhere
        CHECK(out.find("tneduts") != std::string::npos);
    }
    // Empty string should fail at runtime if that's how it's implemented
    // (If you used a static_assert, that will fail compile-time and this test can be guarded)
    CHECK_THROWS_AS(process_data(std::string("")), std::invalid_argument);
    diagnostics("Task 4", cap.str());
}

// ------------------------------
// Task 5: specialization for std::complex<double>
//        and NOT available for std::complex<float>
// ------------------------------

// TEST_CASE(
//     "Task 5: process_data<std::complex<double>> prints real & imag; float version is not "
//     "invocable") {
//     {
//         std::complex<double> c{3.0, 4.0};
//         CoutCapture cap;
//         process_data(c);
//         auto out = cap.str();
//         CHECK(out.find("Real") != std::string::npos);
//         CHECK(out.find("Imag") != std::string::npos);
//     }

//     // This should NOT compile if no viable overload exists.
//     // We simulate with a trait: function must not be invocable for complex<float>.
//     CHECK(has_process_data_v<std::complex<double>> == true);
//     CHECK(has_process_data_v<std::complex<float>> == false);
// }
// ------------------------------
// Task 6: filter_elements is NON-mutating and prints matching keys
// ------------------------------
TEST_CASE("Task 6: filter_elements prints keys and does not mutate container") {
    TestBanner _banner("Task 6 — filter_elements prints keys and does not mutate container");
    ScientificContainer<int> sc;
    sc.add_element(1, std::make_shared<VectorCalculation>());
    sc.add_element(2, std::make_shared<VectorCalculation>());
    sc.add_element(3, std::make_shared<VectorCalculation>());

    // Capture output for even keys
    CoutCapture cap;
    sc.filter_elements([](const int& k) { return k % 2 == 0; });
    auto out = cap.str();
    CHECK(out.find("Key 2") != std::string::npos);

    // Ensure no removal occurred
    std::vector<int> keys;
    for (const auto& [k, _] : sc) keys.push_back(k);
    std::sort(keys.begin(), keys.end());
    CHECK(keys == std::vector<int>({1, 2, 3}));
    diagnostics("Task 6", cap.str());
}

// ------------------------------
// Task 7: range-based for works on const and non-const
// ------------------------------
TEST_CASE("Task 7: begin()/end() on const and non-const containers") {
    TestBanner _banner("Task 7 — begin()/end() on const and non-const containers");
    ScientificContainer<int> sc;
    CoutCapture cap;
    sc.add_element(10, std::make_shared<VectorCalculation>());
    sc.add_element(20, std::make_shared<MatrixCalculation>());

    // non-const
    int sum = 0;
    for (const auto& [k, obj] : sc) {
        (void)obj;
        sum += k;
    }
    CHECK(sum == 30);

    // const
    const auto& csc = sc;
    int count = 0;
    for (const auto& [k, obj] : csc) {
        (void)k;
        (void)obj;
        ++count;
    }
    CHECK(count == 2);
    diagnostics("Task 7", cap.str());
}

// ------------------------------
// Task 8: serialize() produces JSON-like output
//         - no trailing commas
//         - includes a "type" field
// ------------------------------
TEST_CASE("Task 8: serialize produces comma-safe JSON-like output with type strings") {
    TestBanner _banner("Task 8 — serialize produces comma-safe JSON-like output with type strings");
    CoutCapture cap;
    ScientificContainer<int> sc;
    sc.add_element(1, std::make_shared<VectorCalculation>());
    sc.add_element(2, std::make_shared<TensorCalculation>());

    // Provide some metadata with a no-op callback
    sc.add_metadata(1,
                    std::make_tuple(42, 3.14, std::string("Vector"), std::function<void()>([] {})));

    const std::string fname = "sci_container.json";
    sc.serialize();

    std::ifstream in(fname);
    REQUIRE(in.good());
    std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    // No trailing commas pattern: "},\n  ]" should not appear
    CHECK(data.find("},\n  ]") == std::string::npos);

    // Types present
    CHECK(data.find("\"type\": \"VectorCalculation\"") != std::string::npos);
    CHECK(data.find("\"type\": \"TensorCalculation\"") != std::string::npos);

    // Minimal structure checks
    CHECK(data.find("\"elements\"") != std::string::npos);
    CHECK(data.find("\"metadata\"") != std::string::npos);
    diagnostics("Task 8", cap.str());
}
