#pragma once
#include <type_traits>

template <typename T>
class UnqPtr {
private:
    T* ptr;

public:
    UnqPtr(T* p = nullptr) : ptr(p) {}
    ~UnqPtr() { delete ptr; }

    UnqPtr(const UnqPtr&) = delete;
    UnqPtr& operator=(const UnqPtr&) = delete;

    UnqPtr(UnqPtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    template <typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    UnqPtr(UnqPtr<U>&& other) noexcept : ptr(other.release()) {}

    UnqPtr& operator=(UnqPtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }

    T* release() {
        T* tmp = ptr;
        ptr = nullptr;
        return tmp;
    }

    void reset(T* p = nullptr) {
        if (ptr == p) return;
        delete ptr;
        ptr = p;
    }
};


template <typename T>
class UnqPtr<T[]> {
private:
    T* ptr;

public:
    UnqPtr(T* p = nullptr) : ptr(p) {}
    ~UnqPtr() { delete[] ptr; }

    UnqPtr(const UnqPtr&) = delete;
    UnqPtr& operator=(const UnqPtr&) = delete;

    UnqPtr(UnqPtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UnqPtr& operator=(UnqPtr&& other) noexcept {
        if (this != &other) {
            delete[] ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator[](size_t index) const { return ptr[index]; }
    T* get() const { return ptr; }

    T* release() {
        T* tmp = ptr;
        ptr = nullptr;
        return tmp;
    }

    void reset(T* p = nullptr) {
        if (ptr == p) return;
        delete[] ptr;
        ptr = p;
    }
};