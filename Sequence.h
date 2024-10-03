#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "UnqPtr.h"

template <typename T>
class Sequence {
public:
    virtual void Append(const T& item)  = 0;
    virtual void Prepend(const T& item) = 0;
    virtual UnqPtr<Sequence<T>> GetSubsequence(int startIndex, int endIndex) const  = 0;
    virtual void InsertAt(const T& item, int index)  = 0;
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;
    virtual int GetLength() const = 0;
    virtual ~Sequence() = default;
};

#endif //SEQUENCE_H
