#ifndef WEEKPTR_H
#define WEEKPTR_H

#include "ShrdPtr.h"

template <typename T>
class WeekPtr {
private:
    T* ptr;
    typename ShrdPtr<T>::RefCounter* refCounter;

public:
    WeekPtr() : ptr(nullptr), refCounter(nullptr) {}

    WeekPtr(const ShrdPtr<T>& other) : ptr(other.Get()), refCounter(other.getRefCounter()) {}

    WeekPtr(const WeekPtr& other) = default;
    WeekPtr& operator=(const WeekPtr& other) = default;

    WeekPtr(WeekPtr&& other) noexcept = default;
    WeekPtr& operator=(WeekPtr&& other) noexcept = default;

    ~WeekPtr() = default;

    T* operator->() const {
        if (expired()) {
            throw std::runtime_error("Trying to dereference expired WeekPtr");
        }
        return ptr;
    }

    T& operator*() const {
        if (expired()) {
            throw std::runtime_error("Trying to dereference expired WeekPtr");
        }
        return *ptr;
    }

    bool expired() const { return !refCounter || refCounter->count == 0; }

    ShrdPtr<T> lock() const {
        if (expired()) {
            return ShrdPtr<T>();
        } else {
            ShrdPtr<T> newShrdPtr(ptr);
            newShrdPtr.refCounter = refCounter;
            newShrdPtr.refCounter->count.fetch_add(1, std::memory_order_relaxed); // <-- ИСПРАВЛЕНО!
            return newShrdPtr;
        }
    }

    friend class ShrdPtr<T>;
};

#endif // WEEKPTR_H