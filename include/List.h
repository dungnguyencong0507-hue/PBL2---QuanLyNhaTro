#pragma once
#include <iostream>
#include <utility>
#include <algorithm>
#include <stdexcept>

using std::max;
using std::min;
using std::move;
using std::runtime_error;

template <typename T>
class List
{
    T* l;
    int n;
    int capacity;

    public:
        List();
        List(int);
        List(int, const T&);
        List(List&&) noexcept;
        List(const List&);
        ~List();
        List& operator=(const List&);
        List& operator=(List&& other) noexcept;
        int max_size() const;
        int size() const;
        bool empty() const;

        T& operator[](const int&);                      //duoc sua gia tri
        const T& operator[](const int&) const;          //khong duoc sua chi duoc doc
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

        T* begin();
        const T* begin() const;
        T* end();
        const T* end() const;

        T* push_back(T);
        T* push_front(T);
        T* insert(int, T);
        T* pop_back();
        T* pop_front();
        T* erase(int);
        T* erase(const T*);
        void clear();
        void reserve(int);

        void sort(bool(*compare)(const T&,const T&));
};
template<typename T>
List<T>::List() : l(nullptr), n(0), capacity(0)
{}
template<typename T>
List<T>::List(int n) : l(nullptr)
{
    if (n > 0) this->l = new T[n];
    this->n = max(0, n);
    this->capacity = max(0, n);
}
template<typename T>
List<T>::List(int n, const T& t) : List()
{
    if (n > 0) this->l = new T[n];
    this->n = max(0, n);
    this->capacity = max(0, n);
    for (int i = 0; i < this->n; i++) this->l[i] = move(t);
}
template<typename T>
List<T>::List(List&& other) noexcept
    : l(other.l), n(other.n), capacity(other.capacity)
{
    other.l = nullptr;
    other.n = 0;
    other.capacity = 0;
}
template<typename T>
List<T>::List(const List& other)
{
    this->l = nullptr;
    this->n = other.size();
    this->capacity = other.max_size();
    if (this->capacity > 0) this->l = new T[this->capacity];
    for (int i = 0; i < this->n; i++) this->l[i] = other[i];
}
template<typename T>
List<T>::~List()
{
    if (this->capacity > 0) delete[] this->l;
}
template<typename T>
List<T>& List<T>::operator=(const List& other)
{
    if(this != &other)
    {
        if (this->capacity > 0) {
            delete[] this->l;
            this->l = nullptr;
        }
        this->n = other.size();
        this->capacity = other.max_size();
        if (this->capacity > 0) this->l = new T[this->capacity];
        for (int i = 0; i < this->n; i++) this->l[i] = other[i];
    }
    return *this;
}
template<typename T>
List<T>& List<T>::operator=(List&& other) noexcept
{
    if (this != &other) {
        delete[] this->l;
        this->l = other.l;
        this->n = other.n;
        this->capacity = other.capacity;

        other.l = nullptr;
        other.n = 0;
        other.capacity = 0;
    }
    return *this;
}
template<typename T>
int List<T>::max_size() const
{
    return this->capacity;
}
template<typename T>
int List<T>::size() const
{
    return this->n;
}
template<typename T>
bool List<T>::empty() const
{
    return this->n == 0;
}
template<typename T>
T& List<T>::operator[](const int& index)
{
    if (index < 0 || index >= this->n) 
        throw runtime_error("Truy cap ngoai mang");
    return this->l[index];
}                  
template<typename T>
const T& List<T>::operator[](const int& index) const
{
    if (index < 0 || index >= this->n) 
        throw runtime_error("Truy cap ngoai mang");
    return this->l[index];
}
template<typename T>                  
T& List<T>::front()
{
    if (this->empty()) throw runtime_error("Truy cap mang trong");
    return this->l[0];
}
template<typename T>                  
const T& List<T>::front() const
{
    if (this->empty()) throw runtime_error("Truy cap mang trong");
    return this->l[0];
}
template<typename T>
T& List<T>::back()
{
    if (this->empty()) throw runtime_error("Truy cap mang trong");
    return this->l[this->n - 1];
}
template<typename T>
const T& List<T>::back() const
{
    if (this->empty()) throw runtime_error("Truy cap mang trong");
    return this->l[this->n - 1];
}
template<typename T>
T* List<T>::begin()
{
    return this->l;
}
template<typename T>
const T* List<T>::begin() const
{
    if (this->n == 0) return this->l;
    return this->l;
}
template<typename T>
T* List<T>::end()
{
    if (this->n == 0) return this->l;
    return this->l + this->n;
}
template<typename T>
const T* List<T>::end() const
{
    if (this->l == nullptr) return this->l;
    return this->l + this->n;
}
template<typename T>
T* List<T>::push_back(T t)
{
    if (this->capacity == 0) this->reserve(4);
    else if (this->n == this->capacity) this->reserve(this->n * 2);

    this->l[this->n] = move(t);
    this->n++;
    return this->l + this->n - 1;
}
template<typename T>
T* List<T>::insert(int index, T t)
{
    if (index < 0 || index > this->n) throw runtime_error("Truy cap ngoai mang");
    if (this->capacity == 0) this->reserve(4);
    else if (this->n == this->capacity) this->reserve(this->n * 2);

    for (int i = this->n; i > index; i--) this->l[i] = move(this->l[i-1]);
    this->l[index] = move(t);
    this->n++;
    return &this->l[index];
}
template<typename T>
T* List<T>::push_front(T t)
{
    if (this->capacity == 0) this->reserve(4);
    else if (this->n == this->capacity) this->reserve(this->n * 2);

    for (int i = this->n; i > 0; i--) this->l[i] = move(this->l[i-1]);
    this->l[0] = move(t);
    this->n++;
    return this->l;
}
template<typename T>
T* List<T>::pop_back()
{
    return erase(this->n - 1);
}
template<typename T>
T* List<T>::erase(int index)
{
    if (this->n == 0) throw runtime_error("Kong the xoa mang rong");
    if(index < 0 || index >= this->n) throw runtime_error("Truy cap ngoai mang");
    for (int i = index; i < this->n - 1; i++) this->l[i] = move(this->l[i+1]);
    this->n--;
    if (index < this->n) return &this->l[index];
    if (this->n > 0) return &this->l[index - 1];
    return nullptr;
}
template<typename T>
T* List<T>::erase(const T* it)
{
    if (this->n == 0) throw runtime_error("Kong the xoa mang rong");
    if (it < this->begin() || it >= this->end()) 
        throw runtime_error("Truy cap ngoai mang");
    int index = it - this->begin();
    return erase(index);
}
template<typename T>
T* List<T>::pop_front()
{
    return erase(0);
}
template<typename T>
void List<T>::clear()
{
    if (this->capacity > 0) delete[] this->l;
    this->capacity = 0;
    this->n = 0;
    this->l = nullptr;
}
template<typename T>
void List<T>::sort(bool(*compare)(const T&,const T&))
{
    for (int i = 1; i < this->n; i++) {
        if (compare(this->l[i-1], this->l[i])) continue;
        
        if (!compare(this->l[0], this->l[i])) {
            T tmp = move(this->l[i]);
            for (int j = i; j > 0; j--) this->l[j] = move(this->l[j-1]);
            this->l[0] = move(tmp);
            continue;
        }

        int l = 0, r = i-1, mid, pos = 0;
        while (l <= r) {
            mid = (l + r) >> 1;
            if (compare(this->l[mid], this->l[i])) {
                pos = mid;
                l = mid + 1;
            }
            else r = mid - 1;
        }
        T tmp = move(this->l[i]);
        for (int j = i; j > pos + 1; j--) this->l[j] = move(this->l[j-1]);
        this->l[pos+1] = move(tmp);
    }
}
template<typename T>
void List<T>::reserve(int n)
{
    if (n < 0) throw runtime_error("Khong the cap mang co so phan tu < 0");
    if (n == this->capacity) return;

    if (n < this->n) 
    throw runtime_error("Loi xoa phan tu khi cap lai vung nho cho List");

    if (n == 0) {
        if (this->capacity > 0) delete[] this->l;
        this->l = nullptr;
        this->capacity = this->n = 0;
        return;
    }

    T* newl = nullptr;
    if (n > 0) newl = new T[n];
    for (int i = 0; i < this->n; i++) newl[i] = move(this->l[i]);
    if (this->capacity > 0) delete[] this->l;
    this->l = newl;
    this->capacity = n;
}