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

	Node<T>* start;
	Node<T>* finish;

	int countNode;
public:

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
//		_CrtDumpMemoryLeaks();
	}


	void acquire(Node<T>** node, Node<T>* pnode) {
	    *node = pnode;
        (*node)->Ref_count += 1;
	}


    void release(Node<T>* node) {
		node->Ref_count -= 1;

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
		return head->next;
	}

	Node<T>* getLast() {
		return tail->prev;
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


	void PushFront(T value) {
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
		return iter_node->Value;
	}

	ListIterator<T>& operator++(){
		Node<T>* prev = iter_node;
		iter_list->acquire(&iter_node, iter_node->next);
		iter_list->release(prev);
		return *this;
	}

	ListIterator<T>& operator--(){
		Node<T>* next = iter_node;
		iter_list->acquire(&iter_node, iter_node->prev);
		iter_list->release(next);
		return *this;
	}
};


