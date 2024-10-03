#ifndef SHRDPTR_H
#define SHRDPTR_H

#include <atomic>

template <typename T>
class WeekPtr;

template <typename T>
class ShrdPtr {
private:
    struct RefCounter {
        std::atomic<size_t> count;
        RefCounter() : count(1) {}
    };

    T* ptr;
    RefCounter* refCounter;

public:
    ShrdPtr() : ptr(nullptr), refCounter(nullptr) {}

    explicit ShrdPtr(T* p) : ptr(p), refCounter(p ? new RefCounter() : nullptr) {}

    ShrdPtr(const ShrdPtr& other) : ptr(other.ptr), refCounter(other.refCounter) {
        if (refCounter) {
            refCounter->count.fetch_add(1, std::memory_order_relaxed);
        }
    }

    ShrdPtr(const WeekPtr<T>& other) : ptr(other.ptr), refCounter(other.refCounter) {
        if (refCounter) {
            refCounter->count.fetch_add(1, std::memory_order_relaxed);
        }
    }

    ShrdPtr(ShrdPtr&& other) noexcept : ptr(other.ptr), refCounter(other.refCounter) {
        other.ptr = nullptr;
        other.refCounter = nullptr;
    }

    ~ShrdPtr() {
        if (refCounter && refCounter->count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete ptr;
            delete refCounter;
        }
    }

    ShrdPtr& operator=(const ShrdPtr& other) {
        if (this == &other) {
            return *this;
        }
        if (refCounter) {
            size_t oldCount = refCounter->count.load();
            if (refCounter->count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                delete ptr;
                delete refCounter;
                ptr = nullptr;
                refCounter = nullptr;
            }
        }
        ptr = other.ptr;
        refCounter = other.refCounter;
        if (refCounter) {
            refCounter->count.fetch_add(1, std::memory_order_relaxed);
        }
        return *this;
    }

    ShrdPtr& operator=(ShrdPtr&& other) noexcept {
        if (this != &other) {
            if (refCounter && refCounter->count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                delete ptr;
                delete refCounter;
            }
            ptr = other.ptr;
            refCounter = other.refCounter;
            other.ptr = nullptr;
            other.refCounter = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* Get() const { return ptr; }
    bool operator!() const {
        return ptr == nullptr;
    }
    bool operator!=(std::nullptr_t) const {
        return ptr != nullptr;
    }

    void reset() {
        if (refCounter && refCounter->count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete ptr;
            delete refCounter;
        }
        ptr = nullptr;
        refCounter = nullptr;
    }

    void reset(T* newPtr) {
        if (refCounter && refCounter->count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete ptr;
            delete refCounter;
        }
        ptr = newPtr;
        refCounter = newPtr ? new RefCounter() : nullptr;
    }

    RefCounter* getRefCounter() const { return refCounter; }

    size_t getRefCount() const {
        return refCounter ? refCounter->count.load() : static_cast<size_t>(0);
    }

    operator bool() const {
        return ptr != nullptr;
    }

    friend class WeekPtr<T>;
};

#endif //SHRDPTR_H