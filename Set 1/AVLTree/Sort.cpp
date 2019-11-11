//
// Created by Yogesh Kumar on 2019-09-04.
//

#include "Sort.h"


template<typename T>
bool comp::increasing(const T& x, const T& y){return x < y;}

template<typename T>
bool comp::decreasing(const T& x, const T& y){return x > y;}

template<typename Pointer>
bool comp::increasingPtr(const Pointer& x, const Pointer& y){
    return *x < *y;
}

template<typename Pointer>
bool comp::decreasingPtr(const Pointer& x, const Pointer& y){
    return *x > *y;
}

template<typename Pointer>
void Sort<Pointer>::operator ()(Pointer start, Pointer end){
    quickSort(start, end, &comp::increasing);
}

template<typename Pointer>
template<typename FuncType>
void Sort<Pointer>::operator ()(Pointer start, Pointer end, FuncType compFunc){
    quickSort(start, end, compFunc);
}

template<typename Pointer>
template<typename FuncType>
void Sort<Pointer>::quickSort(Pointer start, Pointer end, FuncType compFunc){
    if(end > start){
        Pointer pivotIndex = partition(start, end, compFunc);
        quickSort(start,pivotIndex - 1, compFunc);
        quickSort(pivotIndex + 1 , end, compFunc);
    }
}

template<typename Pointer>
template<typename FuncType>
Pointer Sort<Pointer>::partition(Pointer start, Pointer end, FuncType compFunc){
    Pointer pivot = end;
    Pointer left = start - 1, right = start;

    while(right < end){
        if(compFunc(*right, *pivot)){
            Swap(++left, right);
        }
        ++right;
    }

    Swap(++left, end);
    return left;
}

template<typename Pointer>
void Sort<Pointer>::Swap(Pointer x, Pointer y){
    auto temp = *x;
    *x = *y;
    *y = temp;
}

template<typename T, typename FuncType>
void mySort(std::vector<T>& Arr, FuncType compFunc){
    typename std::vector<T>::iterator start = Arr.begin();
    typename std::vector<T>::iterator end = Arr.end();
    Sort<typename std::vector<T>::iterator> quickSort;
    quickSort(start, end - 1, compFunc);
}

template<typename Pointer, typename FuncType>
void mySort(Pointer start, Pointer end, FuncType compFunc){
    Sort<Pointer> quickSort;
    quickSort(start, end - 1, compFunc);
}

template<typename T, typename FuncType>
void mySort(T A[], int size, FuncType compFunc){
    T* start = A;
    T* end = A + size - 1;
    Sort<T*> quickSort;
    quickSort(start, end, compFunc);
}