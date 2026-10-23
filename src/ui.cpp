#include <iostream>
#include <cstdlib>
#include "ui.h"
#include "benchmark.h"
#include "UnqPtr.h"
#include "ShrdPtr.h"
#include "DynamicArray.h"

void showMenu() {
    std::cout << "\n=== Smart Pointers Lab ===\n";
    std::cout << "1. Functional tests\n";
    std::cout << "2. Benchmark\n";
    std::cout << "3. Demo\n";
    std::cout << "4. Show memory sizes\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose: ";
}

void functionalTests() {
    std::cout << "\nRunning functional tests...\n";
    int result = std::system("./tests_run");
    if (result != 0) {
        std::cout << "Tests failed or not built. Build with:\n";
        std::cout << "g++ -std=c++17 -Iinclude tests/functional_tests.cpp "
                     "-o tests_run -lgtest -lgtest_main -pthread\n";
    }
}

void benchmarkMenu() {
    int N;
    std::cout << "\nEnter N (e.g. 1000000): ";
    std::cin >> N;

    runBenchmark(N);
}

void demo() {
    std::cout << "\n--- Demo UnqPtr ---\n";
    UnqPtr<int> p(new int(42));
    std::cout << "Value: " << *p << "\n";

    UnqPtr<int> q(std::move(p));
    std::cout << "After move, q = " << *q << "\n";
    std::cout << "p is empty: " << (p.get() == nullptr ? "yes" : "no") << "\n";

    std::cout << "\n--- Demo ShrdPtr ---\n";
    ShrdPtr<int> a(new int(10));
    std::cout << "a = " << *a << ", count = " << a.use_count() << "\n";

    ShrdPtr<int> b = a;
    std::cout << "After copy: count = " << a.use_count() << "\n";

    std::cout << "\n--- Demo DynamicArray ---\n";
    DynamicArray<int> arr;
    arr.Append(1);
    arr.Append(2);
    arr.Append(3);
    std::cout << "Size: " << arr.GetSize() << "\n";
    for (int i = 0; i < arr.GetSize(); i++) {
        std::cout << *arr.Get(i) << " ";
    }
    std::cout << "\n";
}

void showMemorySizes() {
    std::cout << "\n--- Memory sizes (bytes) ---\n";
    std::cout << "sizeof(int*)        = " << sizeof(int*) << "\n";
    std::cout << "sizeof(UnqPtr<int>) = " << sizeof(UnqPtr<int>) << "\n";
    std::cout << "sizeof(ShrdPtr<int>) = " << sizeof(ShrdPtr<int>) << "\n";
}