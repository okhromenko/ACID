#pragma once
#include <mutex>
#include <shared_mutex>
#include <atomic>

template <class T>
class Node {
public:
    T Value;
    std::atomic<int> Ref_count = 0;
    std::atomic<bool> delete_tag;
    Node* next;
    Node* prev;
    std::shared_mutex m;

    ~Node(){}
};

template <class T>
class ListIterator;

template <class T>
class List {
    Node<T>* head;
    Node<T>* tail;

    Node<T>* start;
    Node<T>* finish;

    int countNode;

public:

    template<class T> friend class ListIterator;

    List() {
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
        return countNode;
    }

    int getRef_count(Node<T>* node) {
        return node->Ref_count;
    }

    Node<T>* next_node(Node<T>* node) {
        return node->next;
    }

    Node<T>* prev_node(Node<T>* node) {
        return node->prev;
    }

    T getValue(Node<T>* ptr) {
        return ptr->Value;
    }

    Node<T>* getFirst() {
        return start;
    }

    bool Empty(){
        return countNode == 0;
    }

    Node<T>* getLast() {
        return finish;
    }

    ListIterator<T> begin() {
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
        if (countNode == 0) {
            ListIterator<T> iter(this, &finish);
            return iter;
        }
        else {
            ListIterator<T> iter(this, &finish->prev);
            return iter;
        }
    }

    void PushFront(const T& value) {
        Insert(begin(), value);
    }

    void PushBack(const T& value) {
        Insert(end(), value);
    }

    ListIterator<T> Insert(ListIterator<T> it, const T& value) {
        Node<T>* node_next = it.iter_node;

        if (node_next == head){
            node_next = node_next->next;
        }

        std::unique_lock<std::shared_mutex> lock_write(node_next->m);

        for (bool retry = true; retry;){
            retry = false;

            auto node_prev = node_next->prev;
            assert(node_prev->Ref_count);

            std::unique_lock<std::shared_mutex> lock_write_1(node_prev->m);

            if (node_prev->next == node_next){
                auto node = new Node<T>(value);

                node->next = node_next;
                node->prev = node_prev;
                node_next->prev = node;
                node_prev->next = node;
                node->Ref_count += 2;

                countNode++;
            }
            else{
                retry = true;
            }

            lock_write.unlock();
            lock_write_1.unlock();
        }
    }

    void PopFront(){
        Erase(begin());
    }

    void PopBack(){
        Erase(end());
    }

    void Erase(ListIterator<T> &it){
        Node<T> *node = it.iter_node;

        for (bool retry = true; retry;){
            retry = false;

            if (node->delete_tag){
                return;
            }

            std::shared_lock<std::shared_mutex> lock_read(node->m);
            auto prev = node->prev;
            assert(prev->Ref_count);
            prev->Ref_count++;

            auto next = node->next;
            assert(next->Ref_count);
            next->Ref_count++;

            lock_read.unlock();

            std::unique_lock<std::shared_mutex> lock_write_1(prev->m);
            std::shared_lock<std::shared_mutex> lock_read_1(node->m);
            std::unique_lock<std::shared_mutex> lock_write_2(next->m);

            if (prev->next == node && next->prev == node){
                node->delete_tag = true;

                node->next->prev = prev;
                node->prev->next = next;

                countNode--;
            }
            else {
                retry = true;
            }

            prev->Ref_count--;
            next->Ref_count--;
            lock_write_1.unlock();
            lock_read_1.unlock();
            lock_write_2.unlock();
        }
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

        iter_node->Ref_count++;
    }

    ~ListIterator(){
        if (!iter_node){
            return;
        }
        iter_list->release(iter_node);
        iter_list = nullptr;
    }


    friend bool operator==(const ListIterator<T>& iter_1, const ListIterator<T>& iter_2){
        return iter_1.iter_node == iter_2.iter_node;
    }

    friend bool operator!=(const ListIterator<T>& iter_1, const ListIterator<T>& iter_2){
        return iter_1.iter_node != iter_2.iter_node;
    }

    ListIterator<T>& operator=(const ListIterator<T> iter){
        std::unique_lock<std::shared_mutex> write_lock(iter_node->m);
        if (iter.iter_node != iter_node){
            std::shared_lock<std::shared_mutex> read_lock(iter.iter_node->m);

            iter_node = iter.iter_node;
            iter_list = iter.iter_list;
            iter_node->Ref_count++;
        }
        return *this;
    }

    T& operator*() {
        std::shared_lock<std::shared_mutex> lock(iter_node->m);
        return iter_node->Value;
    }


    ListIterator<T>& operator++() {
        std::unique_lock<std::shared_mutex> lock(iter_node->m);
        Node<T>* prev(iter_node);
        iter_list->acquire(&iter_node, iter_node->next);
        iter_list->release(prev);
        return *this;
    }

    ListIterator<T>& operator--() {
        std::unique_lock<std::shared_mutex> lock(iter_node->m);
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

