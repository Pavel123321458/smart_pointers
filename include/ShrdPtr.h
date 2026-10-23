#pragma once
#include <cstddef>
#include <utility>
#include <type_traits>

template <typename T>
class ShrdPtr {
private:
    T* ptr;
    size_t* referenceCount;

public:
    ShrdPtr(T* p = nullptr) 
        : ptr(p), referenceCount(new size_t(1)) {}

    ShrdPtr(const ShrdPtr& other)
        : ptr(other.ptr), referenceCount(other.referenceCount) {
        ++(*referenceCount);
    }
    
    template <typename> friend class ShrdPtr;
    template <typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    ShrdPtr(const ShrdPtr<U>& other) noexcept
        : ptr(other.ptr), referenceCount(other.referenceCount) {
        if (referenceCount) ++(*referenceCount);
    }

    ShrdPtr& operator=(const ShrdPtr& other) {
        if (this != &other) {
            if (--(*referenceCount) == 0) {
                delete ptr;
                delete referenceCount;
            }
            ptr = other.ptr;
            referenceCount = other.referenceCount;
            ++(*referenceCount);
        }
        return *this;
    }

    ~ShrdPtr() {
        if (--(*referenceCount) == 0) {
            delete ptr;
            delete referenceCount;
        }
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }

    size_t use_count() const { return *referenceCount; }

    void reset(T* p = nullptr) {
        if (ptr == p) return;
        if (--(*referenceCount) == 0) {
            delete ptr;
            delete referenceCount;
        }
        ptr = p;
        referenceCount = new size_t(1);
    }

};


template <typename T>
class ShrdPtr<T[]> {
private:
    T* ptr;
    size_t* referenceCount;

public:
    ShrdPtr(T* p = nullptr) 
        : ptr(p), referenceCount(new size_t(1)) {}

    ShrdPtr(const ShrdPtr& other)
        : ptr(other.ptr), referenceCount(other.referenceCount) {
        ++(*referenceCount);
    }

    ShrdPtr& operator=(const ShrdPtr& other) {
        if (this != &other) {
            if (--(*referenceCount) == 0) {
                delete[] ptr;
                delete referenceCount;
            }
            ptr = other.ptr;
            referenceCount = other.referenceCount;
            ++(*referenceCount);
        }
        return *this;
    }

    ~ShrdPtr() {
        if (--(*referenceCount) == 0) {
            delete[] ptr;
            delete referenceCount;
        }
    }

    T& operator[](size_t index) const { return ptr[index]; }
    T* get() const { return ptr; }

    size_t use_count() const { return *referenceCount; }

    void reset(T* p = nullptr) {
        if (ptr == p) return;
        if (--(*referenceCount) == 0) {
            delete[] ptr;
            delete referenceCount;
        }
        ptr = p;
        referenceCount = new size_t(1);
    }
};