
#ifndef UNTITLED1_CONSIST_LIST_H
#define UNTITLED1_CONSIST_LIST_H

#pragma once


template <class T>
class Node {
public:
    T Value;
    int Ref_count = 0;
    bool delete_tag;
    Node* next;
    Node* prev;
};

template <class T>
class ListIterator;

template <class T>
class List {
    Node<T>* head;
    Node<T>* tail;

    Node<T>* n_remove_first;
    Node<T>* n_remove_last;

    int countNode;

public:
    friend class ListIterator<T>;

    List() {
        n_remove_first = new Node<T>();
        n_remove_last = new Node<T>();
        countNode = 0;

        n_remove_first->Ref_count = 1;
        n_remove_first->next = n_remove_last;
        n_remove_first->prev = nullptr;
        n_remove_first->Value = 000;

        n_remove_last->Ref_count = 1;
        n_remove_last->next = nullptr;
        n_remove_last->prev = n_remove_first;
        n_remove_last->Value = 001;

        head = n_remove_first;
        tail = n_remove_last;
    }

    ~List() {
        Node<T>* node = n_remove_first;

        while (node != nullptr) {
            Node<T>* pnode = node->next;
            delete node;
            node = pnode;
        }
    }


    void acquire(Node<T>** node, Node<T>* pnode) {
        *node = pnode;
        pnode->Ref_count++;
    }


    void release(Node<T>* node) {
        node->Ref_count--;

    }

    int CountNode() {
        return countNode;
    }

    int getRef_count(Node<T>* node) {
        return node->Ref_count;
    }

    Node<T>* next(Node<T>* node) {
        return node->next;
    }

    Node<T>* prev(Node<T>* node) {
        return node->prev;
    }

    T getValue(Node<T>* ptr) {
        return ptr->Value;
    }

    Node<T>* getFirst() {
        return head;
    }

    Node<T>* getLast() {
        return tail;
    }

    ListIterator<T> begin() {
    }

    ListIterator<T> end() {
    }

    void Insert() {

    }

    void PushFront(T value) {
        auto element = new Node<T>();

        element->next = n_remove_first->next;
        element->prev = n_remove_first;
        element->Value = value;
        element->Ref_count += 2;

        element->next->prev = element;
        n_remove_first->next = element;
        countNode++;
    }

    void Erase(Node<T>* node) {
        Node<T>* next_node = node->next;
        Node<T>* prev_node = node->prev;
        node->delete_tag = true;

        if (node != n_remove_last) {
            next_node->prev = prev_node;
            node->Ref_count -= 1;
        }

        if (node != n_remove_first) {
            prev_node->next = next_node;
            node->Ref_count -= 1;
        }
        countNode--;
    }
};


template <class T>
class ListIterator {

    Node<T>* iter_node;
    List<T>* iter_list;

public:

    ListIterator<T>() {
        iter_node = nullptr;
    }


    int& operator*() {
        return iter_node->Value;
    }

    ListIterator<T>& operator++() {
        Node<T>* prev(iter_node);
        iter_list->acquire(&iter_node, iter_node->next);
        iter_list->release(prev);
        return *this;
    }

    ListIterator<T>& operator--() {
        Node<T>* next(iter_node);
        iter_list->acquire(&iter_node, iter_node->prev);
        iter_list->release(next);
        return *this;
    }

    ListIterator<T> operator++(int){
        ListIterator<T> prev(*this);
        ++(*this);
        return prev;
    }

    ListIterator<T> operator--(int){
        ListIterator<T> iter(*this);
        --(*this);
        return iter;
    }
};

#endif //UNTITLED1_CONSIST_LIST_H
