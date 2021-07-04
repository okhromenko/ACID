#pragma once
#include <mutex>
#include <shared_mutex>

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

    Node<T>* start;
    Node<T>* finish;

    std::shared_mutex mutex;

    int countNode;
public:
    template<class T> friend class ListIterator;

    List() {

        std::unique_lock<std::shared_mutex> lock(mutex);

        start = new Node<T>();
        finish = new Node<T>();
        countNode = 0;

        start->Ref_count = 1;
        start->next = finish;
        start->prev = nullptr;
        start->Value = -1;

        finish->Ref_count = 1;
        finish->next = nullptr;
        finish->prev = start;
        finish->Value = -2;

        head = start;
        tail = finish;
    }

    ~List() {

        Node<T>* node = start;

        while (node != nullptr) {
            Node<T>* pnode = node->next;
            delete node;
            node = pnode;
        }
        //		_CrtDumpMemoryLeaks();
    }


    void acquire(Node<T>** node, Node<T>* pnode) {

        *node = pnode;
        pnode->Ref_count++;
    }


    void release(Node<T>* node) {

        node->Ref_count--;

        if (node->Ref_count == 0) {
            if (!node->delete_tag) {
                release(node->next);
                release(node->prev);
            }
            node->next = nullptr;
            node->prev = nullptr;
            delete node;
        }
    }

    int CountNode() {

        std::shared_lock<std::shared_mutex> lock(mutex);
        return countNode;
    }

    int getRef_count(Node<T>* node) {

        std::shared_lock<std::shared_mutex> lock(mutex);
        return node->Ref_count;
    }

    Node<T>* next(Node<T>* node) {

        std::shared_lock<std::shared_mutex> lock(mutex);
        return node->next;
    }

    Node<T>* prev(Node<T>* node) {

        std::shared_lock<std::shared_mutex> lock(mutex);
        return node->prev;
    }

    T getValue(Node<T>* ptr) {

        std::shared_lock<std::shared_mutex> lock(mutex);
        return ptr->Value;
    }

    Node<T>* getFirst() {

        std::shared_lock<std::shared_mutex> lock(mutex);
        return head->next;
    }

    Node<T>* getLast() {

        std::shared_lock<std::shared_mutex> lock(mutex);
        return tail->prev;
    }

    ListIterator<T> begin() {

        std::unique_lock<std::shared_mutex> lock(mutex);
        if (countNode == 0) {
            ListIterator<T> iter(this, &start);
            return iter;
        }
        else {
            ListIterator<T> iter(this, &start->next);
            return iter;
        }
    }

    ListIterator<T> end() {

        std::unique_lock<std::shared_mutex> lock(mutex);
        if (countNode == 0) {
            ListIterator<T> iter(this, &finish);
            return iter;
        }
        else {
            ListIterator<T> iter(this, &finish->prev);
            return iter;
        }
    }

    void Insert() {

        std::unique_lock<std::shared_mutex> lock(mutex);

    }

    void PushFront(T value) {

        std::unique_lock<std::shared_mutex> lock(mutex);

        auto element = new Node<T>();

        element->next = start->next;
        element->prev = start;
        element->Value = value;
        element->Ref_count += 2;

        element->next->prev = element;
        start->next = element;
        countNode++;
    }

    void Erase(Node<T>* node) {

        std::unique_lock<std::shared_mutex> lock(mutex);
        Node<T>* next_node = node->next;
        Node<T>* prev_node = node->prev;
        node->delete_tag = true;

        if (node != finish) {
            next_node->prev = prev_node;
            node->Ref_count -= 1;
        }

        if (node != start) {
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


    ListIterator<T>(List<T>* list, Node<T>** node) {

        iter_list = list;
        iter_node = *node;

        iter_node->Ref_count += 1;
    }

    int& operator*() {

        std::shared_lock<std::shared_mutex> lock(iter_list->mutex);
        return iter_node->Value;
    }

    ListIterator<T>& operator++() {

        std::unique_lock<std::shared_mutex> lock(iter_list->mutex);
        Node<T>* prev(iter_node);
        iter_list->acquire(&iter_node, iter_node->next);
        iter_list->release(prev);
        return *this;
    }

    ListIterator<T>& operator--() {

        std::unique_lock<std::shared_mutex> lock(iter_list->mutex);
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


