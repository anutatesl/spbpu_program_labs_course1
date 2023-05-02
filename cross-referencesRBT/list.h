#ifndef LIST_H
#define LIST_H

#include <stdexcept>

template <typename T>
class List
{
public:
    class IteratorList;

    List();
    List(const List& other);
    List(List&& other) noexcept;
    List(std::initializer_list<T> list);
    ~List();
    List<T>& operator=(const List& other);
    List<T>& operator=(List&& other) noexcept;

    void pushBack(const T& value);

    IteratorList begin();
    IteratorList end();

private:
    struct ListImpl;

    ListImpl impl_;
};

namespace listDetails
{
    template <typename T>
    struct Node;

    template <typename T>
    using NodePtr = Node<T>*;

    template <typename T>
    struct Node
    {
        T value;
        NodePtr<T> next;
    };
}

template <typename T>
class List<T>::IteratorList
{
public:
    IteratorList() : 
        node{ nullptr }
    { }

    IteratorList(listDetails::NodePtr<T> node) : 
        node{ node }
    { }

    IteratorList& operator++()
    {
        node = node->next;
        return *this;
    }

    IteratorList operator++(int)
    {
        auto t = *this;
        node = node->next;
        return t;
    }

    T& operator*() const
    {
        return node->value;
    }

    bool operator==(const IteratorList& other) const
    {
        return node == other.node;
    }

    bool operator!=(const IteratorList& other) const
    {
        return node != other.node;
    }

private:
    listDetails::NodePtr<T> node;
};

template <typename T>
struct List<T>::ListImpl
{
    listDetails::NodePtr<T> head;
};

template <typename T>
List<T>::List() :
    impl_{ nullptr }
{ }

template <typename T>
List<T>::List(const List& other):
    impl_{ nullptr }
{
    if (!other.impl_.head) 
    {
        return;
    }
    listDetails::Node<T>* tail = impl_.head = new listDetails::Node<T>{ other.impl_.head->value, nullptr };
    for (listDetails::Node<T>* itr = other.impl_.head->next; itr; itr = itr->next) 
    {
        tail = tail->next = new listDetails::Node<T>{ itr->value, nullptr };
    }
}

template <typename T>
List<T>::List(List&& other) noexcept:
    impl_{ other.impl_ }
{
    other.impl_ = { nullptr };
}

template <typename T>
List<T>::List(std::initializer_list<T> list):
    impl_{ nullptr }
{
    auto itr = list.begin();
    listDetails::Node<T>* tail = impl_.head = new listDetails::Node<T>{ *itr, nullptr };
    for (++itr; itr != list.end(); ++itr) 
    {
        tail = tail->next = new listDetails::Node<T>{ *itr, nullptr };
    }
}

namespace listDetails
{
    template <typename T>
    void destructList(NodePtr<T> begin);
}

template <typename T>
List<T>::~List()
{
    if (!impl_.head) 
    {
        return;
    }
    listDetails::destructList(impl_.head);
}

template <typename T>
List<T>& List<T>::operator=(const List& other)
{
    if (this == &other) 
    {
        return *this;
    }
    listDetails::destructList(impl_.head);
    listDetails::Node<T>* tail = impl_.head = new listDetails::Node<T>{ other.impl_.head->value, nullptr };
    for (listDetails::Node<T>* itr = other.impl_.head->next; itr; itr = itr->next)
    {
        tail = tail->next = new listDetails::Node<T>{ itr->value, nullptr };
    }
    return *this;
}

template <typename T>
List<T>& List<T>::operator=(List&& other) noexcept
{
    listDetails::destructList(impl_.head);
    impl_ = other.impl_;
    other.impl_ = { nullptr };
    return *this;
}

template <typename T>
void List<T>::pushBack(const T& value)
{
    if (!impl_.head) 
    {
        impl_.head = new listDetails::Node<T>{ value, nullptr };
        return;
    }
    if (impl_.head->value == value) 
    {
        return;
    }
    listDetails::Node<T>* tail = impl_.head;
    while (tail->next) 
    {
        if (tail->value == value) 
        {
            return;
        }
        tail = tail->next;
    }
    if (tail->value == value) 
    {
        return;
    }
    tail->next = new listDetails::Node<T>{ value, nullptr };
}

template <typename T>
typename List<T>::IteratorList List<T>::begin()
{
    return IteratorList(impl_.head);
}

template <typename T>
typename List<T>::IteratorList List<T>::end()
{
    return IteratorList();
}

namespace listDetails
{
    template <typename T>
    void destructList(NodePtr<T> begin)
    {
        for (listDetails::Node<T>* r = begin; begin; r = begin)
        {
            begin = begin->next;
            delete r;
        }
    }
}

#endif