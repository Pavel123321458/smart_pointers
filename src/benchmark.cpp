#include <iostream>
#include <chrono>
#include <memory>
#include "benchmark.h"
#include "UnqPtr.h"
#include "ShrdPtr.h"

using namespace std::chrono;

long long testRawPointers(int N) {
    int** ptrs = new int*[N];

    auto start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) ptrs[i] = new int(i);
    for (int i = 0; i < N; i++) delete ptrs[i];
    auto end = high_resolution_clock::now();

    delete[] ptrs;
    return duration_cast<milliseconds>(end - start).count();
}

long long testUnqPtr(int N) {
    UnqPtr<int>** ptrs = new UnqPtr<int>*[N];

    auto start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) ptrs[i] = new UnqPtr<int>(new int(i));
    for (int i = 0; i < N; i++) delete ptrs[i];
    auto end = high_resolution_clock::now();

    delete[] ptrs;
    return duration_cast<milliseconds>(end - start).count();
}

long long testShrdPtr(int N) {
    ShrdPtr<int>** ptrs = new ShrdPtr<int>*[N];

    auto start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) ptrs[i] = new ShrdPtr<int>(new int(i));
    for (int i = 0; i < N; i++) delete ptrs[i];
    auto end = high_resolution_clock::now();

    delete[] ptrs;
    return duration_cast<milliseconds>(end - start).count();
}

long long testUniquePtrSTL(int N) {
    std::unique_ptr<int>** ptrs = new std::unique_ptr<int>*[N];

    auto start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) ptrs[i] = new std::unique_ptr<int>(new int(i));
    for (int i = 0; i < N; i++) delete ptrs[i];
    auto end = high_resolution_clock::now();

    delete[] ptrs;
    return duration_cast<milliseconds>(end - start).count();
}

long long testSharedPtrSTL(int N) {
    std::shared_ptr<int>** ptrs = new std::shared_ptr<int>*[N];

    auto start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) ptrs[i] = new std::shared_ptr<int>(new int(i));
    for (int i = 0; i < N; i++) delete ptrs[i];
    auto end = high_resolution_clock::now();

    delete[] ptrs;
    return duration_cast<milliseconds>(end - start).count();
}

void runBenchmark(int N) {
    std::cout << "\n=== Benchmark (N = " << N << ") ===\n";

    long long raw = testRawPointers(N);
    std::cout << "Raw pointers:     " << raw << " ms\n";

    long long unq = testUnqPtr(N);
    std::cout << "UnqPtr (ours):    " << unq << " ms\n";

    long long shrd = testShrdPtr(N);
    std::cout << "ShrdPtr (ours):   " << shrd << " ms\n";

    long long stlUnq = testUniquePtrSTL(N);
    std::cout << "std::unique_ptr:  " << stlUnq << " ms\n";

    long long stlShr = testSharedPtrSTL(N);
    std::cout << "std::shared_ptr:  " << stlShr << " ms\n";

    std::cout << "\nRatios (vs raw):\n";
    std::cout << "UnqPtr:          " << (double)unq / raw << "x\n";
    std::cout << "ShrdPtr:         " << (double)shrd / raw << "x\n";
    std::cout << "std::unique_ptr: " << (double)stlUnq / raw << "x\n";
    std::cout << "std::shared_ptr: " << (double)stlShr / raw << "x\n";
}