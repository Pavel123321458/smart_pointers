#pragma once
#include "UnqPtr.h"
#include "ShrdPtr.h"
#include <stdexcept>

template <typename T>
class DynamicArray{
    private:
        UnqPtr<T>* items;
        int size;
        int capacity;

        void ensureCapacity(int needed){
            if (needed <= capacity){
                return;
            }
            
            int newCapacity = capacity;
            if (newCapacity == 0){
                newCapacity = 1;
            }
            while (newCapacity < needed){
                newCapacity *= 2;
            }

            UnqPtr<T>* newItems = new UnqPtr<T>[newCapacity];
            for (int i = 0; i < size; i++){
                newItems[i] = std::move(items[i]);
            }

            delete[] items;
            items = newItems;
            capacity = newCapacity;
        }
    public:
        DynamicArray() : items(nullptr), size(0), capacity(0){}

        explicit DynamicArray(int s) : items(nullptr), size(s),capacity(s) {
            if (s < 0) throw std::invalid_argument("Negative size");
            items = new UnqPtr<T>[s];
        }

        ~DynamicArray() { delete[] items; }

        UnqPtr<T>& Get(int ind){
            if (ind < 0 || ind >= size) throw std::out_of_range("IndexOutOfRange");
            return items[ind];
        }

        void Set(int ind, const T& val){
            if (ind < 0 || ind >= size) throw std::out_of_range("IndexOutOfRange");
            items[ind] = UnqPtr<T>(new T(val));
        }

        void Append(const T& val){
            ensureCapacity(size + 1);
            items[size] = UnqPtr<T>(new T(val));
            ++size;
        }

        int GetSize() const { return size; }



};