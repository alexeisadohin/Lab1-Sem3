#ifndef LISTSEQUENCE_H
#define LISTSEQUENCE_H

#include "Sequence.h"
#include "LinkedList.h"

template <typename T>
class ListSequence : public Sequence<T> {
private:
    UnqPtr<LinkedList<T>> list;

public:
    ListSequence() : list(new LinkedList<T>()) {}
    ListSequence(const T* items, int count) : list(new LinkedList<T>(items, count)) {}
    ListSequence(const ListSequence<T>& listSequence) : list(new LinkedList<T>(*listSequence.list)) {}
    ListSequence(ListSequence<T>&& other) noexcept: list(std::move(other.list)){}
    ListSequence(const LinkedList<T>& otherList) : list(new LinkedList<T>(otherList)) {}
    ~ListSequence() override  = default;

    void Append(const T& item) override {
        list->Append(item);
    }

    void Prepend(const T& item) override {
        list->Prepend(item);
    }

    UnqPtr<Sequence<T>> GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex < 0 ||
       startIndex >= list->GetLength() || endIndex >= list->GetLength() ||
       startIndex > endIndex) {
            throw std::out_of_range("Invalid index range for GetSubsequence");
       }

        ShrdPtr<LinkedList<T>> subList = list->GetSubList(startIndex, endIndex);
        return UnqPtr<Sequence<T>>(new ListSequence<T>(*subList));
    }

    void InsertAt(const T& item, int index) override {
        list->InsertAt(item, index);
    }

    T GetFirst() const override {
        if (list->GetLength() == 0) {
            throw std::out_of_range("IndexOutOfRange");
        }
        return list->GetFirst();
    }

    T GetLast() const override {
        if (list->GetLength() == 0) {
            throw std::out_of_range("IndexOutOfRange");
        }
        return list->GetLast();
    }

    T Get(int index) const override {
        return list->Get(index);
    }

    int GetLength() const override {
        return list->GetLength();
    }
};
#endif //LISTSEQUENCE_H
