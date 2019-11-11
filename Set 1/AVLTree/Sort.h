//
// Created by Yogesh Kumar on 2019-09-04.
//

#ifndef DS_ALGO_LAB_SORT_H
#define DS_ALGO_LAB_SORT_H

#include <vector>

namespace comp{
    template<typename T>
    bool increasing(const T& x, const T& y);

    template<typename T>
    bool decreasing(const T& x, const T& y);

    template<typename Pointer>
    bool increasingPtr(const Pointer& x, const Pointer& y);

    template<typename Pointer>
    bool decreasingPtr(const Pointer& x, const Pointer& y);
}

template<typename Pointer>
class Sort{
public:
    void operator ()(Pointer start, Pointer end);

    template<typename FuncType>
    void operator ()(Pointer start, Pointer end, FuncType compFunc);

private:

    template<typename FuncType>
    static void quickSort(Pointer start, Pointer end, FuncType compFunc);

    template<typename FuncType>
    static Pointer partition(Pointer start, Pointer end, FuncType compFunc);

    static void Swap(Pointer x, Pointer y);
};

template<typename T, typename FuncType>
void mySort(std::vector<T>& Arr, FuncType compFunc = comp::increasing<T>);

template<typename Pointer, typename FuncType>
void mySort(Pointer start, Pointer end, FuncType compFunc = comp::increasingPtr<Pointer>);

template<typename T, typename FuncType>
void mySort(T A[], int size, FuncType compFunc = comp::increasing<T>);

#include "Sort.cpp"
#endif