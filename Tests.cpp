#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <cassert>

#include "ShrdPtr.h"
#include "WeekPtr.h"
#include "UnqPtr.h"
#include "ListSequence.h"
#include "Tests.h"

template<typename Func>
double measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
}

void testFunctionality() {
    std::cout << "Testing functionality...\n";
    {
        std::cout << "  - ShrdPtr: ";
        ShrdPtr<int> ptr1(new int(5));
        assert(ptr1.Get() != nullptr);
        assert(*ptr1 == 5);

        ShrdPtr<int> ptr2(ptr1);
        assert(ptr1.Get() == ptr2.Get());
        assert(*ptr2 == 5);
        assert(ptr1.getRefCount() == 2);

        ShrdPtr<int> ptr3;
        ptr3 = ptr1;
        assert(ptr3.Get() == ptr1.Get());
        assert(*ptr3 == 5);
        assert(ptr1.getRefCount() == 3);
        std::cout << "OK\n";
    }

    {
        std::cout << "  - WeekPtr: ";
        ShrdPtr<int> shrdPtr(new int(10));
        WeekPtr<int> weekPtr;

        weekPtr = shrdPtr;
        assert(!weekPtr.expired());

        assert(*weekPtr == 10);

        shrdPtr.reset();
        std::cout << "OK\n";
    }

    {
        std::cout << "  - UnqPtr: ";
        UnqPtr<int> ptr1(new int(15));
        assert(ptr1.Get() != nullptr);
        assert(*ptr1 == 15);

        UnqPtr<int> ptr2(std::move(ptr1));
        assert(ptr1.Get() == nullptr);
        assert(ptr2.Get() != nullptr);
        assert(*ptr2 == 15);

        std::cout << "OK\n";
    }
    std::cout << "Functionality tests passed!\n\n";
}

void testPerformance() {
    const std::vector<size_t> objectCounts = { 100, 1000, 10000, 100000, 1000000 };

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "|  count  |   ShrdPtr   |  Raw Pointer  | std::shared_ptr |   UnqPtr     |\n";
    std::cout << "---------------------------------------------------------------------------\n";

    for (size_t objectCount : objectCounts) {
        double shrdPtrTime = measureTime([&objectCount]() {
            for (size_t i = 0; i < objectCount; ++i) {
                ShrdPtr<int> ptr(new int(i));
            }
        });

        double rawPtrTime = measureTime([&objectCount]() {
            for (size_t i = 0; i < objectCount; ++i) {
                int* ptr = new int(i);
                delete ptr;
            }
        });

        double stdShrdPtrTime = measureTime([&objectCount]() {
            for (size_t i = 0; i < objectCount; ++i) {
                std::shared_ptr<int> ptr = std::make_shared<int>(i);
            }
        });

        double unqPtrTime = measureTime([&objectCount]() {
            for (size_t i = 0; i < objectCount; ++i) {
                UnqPtr<int> ptr(new int(i));
            }
        });

        std::cout << "| " << std::setw(10) << objectCount << " |";
        std::cout << std::setw(12) << shrdPtrTime << " ms |";
        std::cout << std::setw(14) << rawPtrTime << " ms |";
        std::cout << std::setw(15) << stdShrdPtrTime << " ms |";
        std::cout << std::setw(14) << unqPtrTime << " ms |\n";
    }
    std::cout << "---------------------------------------------------------------------------\n";
}
void TestAppend() {
    std::cout << "Running TestAppend..." << std::endl;
    ListSequence<int> list;
    list.Append(1);
    list.Append(2);
    assert(list.GetLength() == 2);
    assert(list.GetLast() == 2);
    std::cout << "TestAppend completed successfully." << std::endl;
}

void TestPrepend() {
    std::cout << "Running TestPrepend..." << std::endl;
    ListSequence<int> list;
    list.Prepend(1);
    assert(list.GetLength() == 1);
    assert(list.GetFirst() == 1);
    list.Prepend(2);
    assert(list.GetLength() == 2);
    assert(list.GetFirst() == 2);
   std::cout << "TestPrepend completed successfully." << std::endl;
}

void TestGetSubsequence() {
    std::cout << "Running TestGetSubsequence..." << std::endl;
    ListSequence<int> list;
    for (int i = 1; i <= 5; ++i) {
        list.Append(i);
    }
    UnqPtr<Sequence<int>> sub = list.GetSubsequence(1, 3);
    assert(sub->GetLength() == 2);
    for (int i = 0; i < sub->GetLength(); ++i) {
        assert(sub->Get(i) == i + 2);
    }
    std::cout << "TestGetSubsequence completed successfully." << std::endl;
}

void TestInsertAt() {
    std::cout << "Running TestInsertAt..." << std::endl;
    ListSequence<int> list;
    for (int i = 1; i <= 3; ++i) {
        list.Append(i);
    }
    list.InsertAt(4, 2);
    assert(list.GetLength() == 4);
    assert(list.Get(2) == 4);
    std::cout << "TestInsertAt completed successfully." << std::endl;
}

void TestGetFirst() {
    std::cout << "Running TestGetFirst..." << std::endl;
    ListSequence<int> list;
    list.Append(1);
    assert(list.GetFirst() == 1);
    std::cout << "TestGetFirst completed successfully." << std::endl;
}

void TestGetLast() {
    std::cout << "Running TestGetLast..." <<std::endl;
    ListSequence<int> list;
    list.Append(1);
    assert(list.GetLast() == 1);
    list.Append(2);
    assert(list.GetLast() == 2);
    std::cout << "TestGetLast completed successfully." << std::endl;
}

void TestGet() {
    std::cout << "Running TestGet..." << std::endl;
    ListSequence<int> list;
    for (int i = 1; i <= 5; ++i) {
        list.Append(i);
    }
    assert(list.Get(3) == 4);
    std::cout << "TestGet completed successfully." << std::endl;
}

void TestGetLength() {
    std::cout << "Running TestGetLength..." << std::endl;
    ListSequence<int> list;
    assert(list.GetLength() == 0);
    list.Append(1);
    assert(list.GetLength() == 1);
    std::cout << "TestGetLength completed successfully." << std::endl;
}

void testListSequence() {
    TestAppend();
    TestPrepend();
    TestGetSubsequence();
    TestInsertAt();
    TestGetFirst();
    TestGetLast();
    TestGet();
    TestGetLength();
}

void tests() {
    testFunctionality();
    testPerformance();
    testListSequence();
    std::cout << "Tests completed successfully." << std::endl;
    std::cout <<"--------------------------------------------------------------"<< std::endl;
}