#pragma once
#include <iostream>
using namespace std;
template <typename T>
struct node
{
    T data;
    node* next;
};
template <typename T>
class LinkedList
{
    node<T>*  l;
    int n;

    node<T>* Getnode(int) const;
    public:
        LinkedList();
        ~LinkedList();
        LinkedList(const LinkedList&);
        LinkedList& operator=(const LinkedList&);
        int size() const;
        bool empty() const;

        T& operator[](const int&);                      //duoc sua gia tri
        const T& operator[](const int&) const;                //khong duoc sua chi duoc doc
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

        int search(const T&) const;
        void push_back(const T&);
        bool insert(int,const T&);
        void pop_back();
        bool erase(int);
        bool remove(const T&);
        void clear();
        void sort(bool(*compare)(const T&,const T&));
};
template<typename T>
node<T>* LinkedList<T>::Getnode(int index) const
{
    node<T>* p = this->l;
    for(int i = 0; i< index;i++) p = p->next;
    return p;
}
template<typename T>
LinkedList<T>::LinkedList()
{
    this->l = nullptr;
    this->n = 0;
}
template<typename T>
LinkedList<T>::~LinkedList()
{
    this->clear();
}
template<typename T>
LinkedList<T>::LinkedList(const LinkedList& list)
{
    this->l = nullptr;
    this->n = 0;
    for(node<T>* p = list.l ; p != nullptr ; p = p->next) push_back(p->data);
}
template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& list)
{
    if(this != &list)
    { 
        clear();
        for(node<T>* p = list.l;p != nullptr ;p = p->next) push_back(p->data);
    }
    return *this;

}
template<typename T>
int LinkedList<T>::size() const
{
    return this->n;
}
template<typename T>
bool LinkedList<T>::empty() const
{
    return this->n == 0;
}
template<typename T>
T& LinkedList<T>::operator[](const int& index)
{
    return Getnode(index)->data;
}                  
template<typename T>
const T& LinkedList<T>::operator[](const int& index) const
{
    return Getnode(index)->data;
}
template<typename T>                  
T& LinkedList<T>::front()
{
    return this->l->data;
}
template<typename T>                  
const T& LinkedList<T>::front() const
{
    return this->l->data;
}
template<typename T>
T& LinkedList<T>::back()
{
    int index = this->n - 1;
    return Getnode(index)->data;
}
template<typename T>
const T& LinkedList<T>::back() const
{
    int index = this->n - 1;
    return Getnode(index)->data;
}
template<typename T>
int LinkedList<T>::search(const T& l) const
{
    int i = 0;
    for(node<T>* p = this->l; p != nullptr;p = p->next) 
    {
        if(p->data == l) return i;
        i++;
    }
    return -1;
}
template<typename T>
void LinkedList<T>::push_back(const T& t)
{
    insert(this->n,t);
}
template<typename T>
bool LinkedList<T>::insert(int index,const T& t)
{
    if(index < 0 || index > this->n) return false;
    node<T>* p = new node<T>;
    p->data = t;

    if(index == 0) 
    {
        p->next = this->l;
        this->l = p;
    }
    else
    {
        node<T>* q = Getnode(index - 1);
        p->next = q->next;
        q->next = p;
    }
    this->n++;
    return true;
}
template<typename T>
void LinkedList<T>::pop_back()
{
    if(this->n > 0) erase(this->n - 1);
}
template<typename T>
bool LinkedList<T>::erase(int index)
{
    if(index < 0 || index >= this->n) return false;
    
    node<T>* p;
    if(index == 0)
    {
        p = this->l;
        this->l = this->l->next;
    }
    else 
    {
        node<T>* q =Getnode(index - 1);
        p = q->next;
        q->next = q->next->next;
    }
    delete p;
    this->n--;
    return true;
}
template<typename T>
bool LinkedList<T>::remove(const T& t)
{
    int i = search(t);
    if(i == -1) return false;
    return erase(i);
}
template<typename T>
void LinkedList<T>::clear()
{
    while(this->l != nullptr)
    {
        node<T>* p = this->l;
        this->l = this->l->next;
        delete p;
    }
    this->n = 0;
}
template<typename T>
void LinkedList<T>::sort(bool(*compare)(const T&,const T&))
{
    for(int i = 0; i < this->n - 1;i++)
    {
        for(node<T>* p = this->l ; p->next != nullptr ; p = p->next)
        {
            if(compare(p->next->data,p->data))
            {
                T tmp = p->data;
                p->data = p->next->data;
                p->next->data = tmp;
            }
        }
    }
}
