// public_tests.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include <complex>
#include <memory>
#include <string>
#include <vector>

#include "doctest.h"

// ── Student code:
#include "code.cpp"  // if this defines main(), switch to a header-only split.

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

// ---- Utility for capturing cout (kept tiny for public suite)
struct CoutCapture {
    std::streambuf* old = nullptr;
    std::ostringstream oss;
    CoutCapture() {
        old = std::cout.rdbuf(oss.rdbuf());
    }
    // In CoutCapture class, add:
    void restore() {
        if (old) {
            std::cout.rdbuf(old);
            old = nullptr;
        }
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

// ───────────────────────────────────────────────────────────────────────────────
// Task 1: TensorCalculation::compute and storage in container
// ───────────────────────────────────────────────────────────────────────────────
TEST_CASE("Task 1: TensorCalculation computes and can be stored") {
    TestBanner _banner("Task 1 — TensorCalculation computes and can be stored");
    auto t = std::make_shared<TensorCalculation>();
    CHECK(t != nullptr);

    CoutCapture cap;
    t->compute();
    CHECK(cap.str().find("tensor") != std::string::npos);

    ScientificContainer<int> sc;
    sc.add_element(6, t);

    bool found = false;
    for (const auto& kv : sc) {
        if (kv.first == 6 && kv.second.get() == t.get()) found = true;
    }
    CHECK(found);
    diagnostics("Task 1", cap.str());
}

// ───────────────────────────────────────────────────────────────────────────────
// Task 4: process_data(std::string) reverses non-empty
// ───────────────────────────────────────────────────────────────────────────────
TEST_CASE("Task 4: process_data(string) reverses") {
    TestBanner _banner("Task 4 — process_data(string) reverses");
    CoutCapture cap;
    process_data(std::string("student"));
    CHECK(cap.str().find("tneduts") != std::string::npos);
    diagnostics("Task 4", cap.str());
}

// ───────────────────────────────────────────────────────────────────────────────
// Task 7: basic range-for works
// ───────────────────────────────────────────────────────────────────────────────
TEST_CASE("Task 7: range-for over container") {
    TestBanner _banner("Task 7 — basic range-for works");
    CoutCapture cap;
    ScientificContainer<int> sc;
    sc.add_element(1, std::make_shared<VectorCalculation>());
    sc.add_element(2, std::make_shared<MatrixCalculation>());

    std::vector<int> keys;
    for (const auto& [k, _] : sc) keys.push_back(k);
    std::sort(keys.begin(), keys.end());
    CHECK(keys == std::vector<int>({1, 2}));
    diagnostics("Task 7", cap.str());
}

// ───────────────────────────────────────────────────────────────────────────────
// Task 5: specialization compiles for complex<double>
// ───────────────────────────────────────────────────────────────────────────────
TEST_CASE("Task 5: process_data(complex<double>) prints parts") {
    TestBanner _banner("Task 5 — process_data(complex<double>) prints parts");
    std::complex<double> c{3.0, 4.0};
    CoutCapture cap;
    process_data(c);
    auto out = cap.str();
    CHECK(out.find("Real") != std::string::npos);
    CHECK(out.find("Imag") != std::string::npos);
    diagnostics("Task 5", cap.str());
}

// ───────────────────────────────────────────────────────────────────────────────
// Task 2 (light): printing metadata triggers some callback effect
// ───────────────────────────────────────────────────────────────────────────────
TEST_CASE("Task 2 (light): metadata callback effect on print") {
    TestBanner _banner("Task 2 (light) — metadata callback effect on print");
    ScientificContainer<int> sc;
    sc.add_element(1, std::make_shared<VectorCalculation>());

    bool ran = false;
    // Store a callback that flips a flag; we won't assert output strings
    sc.add_metadata(1, std::make_tuple(42, 3.14, std::string("ok"),
                                       std::function<void()>([&] { ran = true; })));

    // Light check: before printing, no effect
    CHECK(ran == false);

    // After printing metadata, the side effect must have happened
    CoutCapture cap;
    sc.print_metadata();
    (void)cap;  // ignore text
    CHECK(ran == true);
    diagnostics("Task 2 (light)", cap.str());
}

// ───────────────────────────────────────────────────────────────────────────────
// Task 3 (light): duplicate insertion throws (type/message hidden)
// ───────────────────────────────────────────────────────────────────────────────
TEST_CASE("Task 3 (light): duplicate insertion throws") {
    TestBanner _banner("Task 3 (light) — duplicate insertion throws");
    CoutCapture cap;
    ScientificContainer<int> sc;
    sc.add_element(2, std::make_shared<MatrixCalculation>());
    // Light check: do not assert type or message
    CHECK_THROWS(sc.add_element(2, std::make_shared<VectorCalculation>()));
    diagnostics("Task 3 (light)", cap.str());
}

// ───────────────────────────────────────────────────────────────────────────────
// Task 6 (light): filter prints something for a matching predicate (non-mutating)
// ───────────────────────────────────────────────────────────────────────────────
TEST_CASE("Task 6 (light): filter prints for matches and does not erase") {
    TestBanner _banner("Task 6 (light) — filter prints for matches and does not erase");
    CoutCapture cap;
    ScientificContainer<int> sc;
    sc.add_element(1, std::make_shared<VectorCalculation>());
    sc.add_element(2, std::make_shared<VectorCalculation>());
    sc.add_element(3, std::make_shared<VectorCalculation>());

    sc.filter_elements([](const int& k) { return k % 2 == 0; });
    auto out = cap.str();
    // Light: don't match exact strings; just assert something was printed
    CHECK(out.size() > 0);

    // Light non-mutation check
    std::vector<int> keys;
    for (const auto& [k, _] : sc) keys.push_back(k);
    std::sort(keys.begin(), keys.end());
    CHECK(keys == std::vector<int>({1, 2, 3}));
    diagnostics("Task 6 (light)", cap.str());
}

// ───────────────────────────────────────────────────────────────────────────────
// Task 8 (light): file exists and has top-level fields (no format details)
// ───────────────────────────────────────────────────────────────────────────────
TEST_CASE("Task 8 (light): serialization produces file with basic sections") {
    TestBanner _banner("Task 8 (light) — serialization produces file with basic sections");
    CoutCapture cap;
    ScientificContainer<int> sc;
    sc.add_element(1, std::make_shared<VectorCalculation>());
    // Tolerate no metadata; students may add some in their demo

    const std::string fname = "sci_container.json";
    // Prefer serialize(filename); if your API differs, adjust accordingly
    sc.serialize();

    std::ifstream in(fname);
    REQUIRE(in.good());
    std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    // Light keys presence — no strict structure/commas/types in public suite
    CHECK(data.find("elements") != std::string::npos);
    CHECK(data.find("metadata") != std::string::npos);
    diagnostics("Task 8 (light)", cap.str());
}
