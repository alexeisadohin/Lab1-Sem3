#ifndef UNQPTR_H
#define UNQPTR_H

template <typename T>
class UnqPtr {
private:
    T* ptr;

public:
    UnqPtr() : ptr(nullptr) {}

    explicit UnqPtr(T* p) : ptr(p) {}

    UnqPtr(UnqPtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    ~UnqPtr() {
        delete ptr;
    }

    UnqPtr& operator=(UnqPtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    UnqPtr(const UnqPtr&) = delete;
    UnqPtr& operator=(const UnqPtr&) = delete;

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    T* Get() const { return ptr; }

    void swap(UnqPtr& other) noexcept {
        std::swap(ptr, other.ptr);
    }
};
#endif //UNQPTR_H