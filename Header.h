#pragma once


template <class T>
class Node {
	T Value = -1;
	int Ref_count = 2;
	bool delete_tag = false;
	class Node<T>* next = nullptr;
	class Node<T>* prev = nullptr;
	template <class T> friend class List;
	template <class T> friend class ListIterator;
};


template <class T>
class ListIterator {
	Node<T>* node = new Node<T>();
	template <class T> friend class List;
public:
	
	ListIterator() {
		node = nullptr;
	}


	ListIterator(Node<T>* inf) {
		if (inf != nullptr) {
			node = inf;
			node->Ref_count += 1;
		}
	}

	T operator*();

	ListIterator operator++();

	ListIterator operator--();
};

template <class T>
class List {
	Node<T>* head;
	Node<T>* end;
	int countNode;
public:
	Node<T>* start = new Node<T>();
	Node<T>* finish = new Node<T>();

	List() : head(nullptr), end(nullptr), countNode(0){
		start->Value = -1;
		finish->Value = -2;
	}

	Node<T>* acquire(Node<T> **, Node<T> *);

	Node<T>* release(Node<T> **);

	int getRef_count(Node<T>*);

	Node<T>* NEXT(Node<T>*);

	T getValue(Node<T> *);

	Node<T>* getFirst();

	Node<T>* getLast();

	void PushFront(T);

	void Delete(Node<T> *);
};



template <class T>
T ListIterator<T>::operator*() {
	return node->Value;
};


template <class T>
ListIterator<T> ListIterator<T>::operator++() {
	List<T> List;
	Node<T>* prev = node;
	node = List.acquire(&node, node->next);
	List.release(&prev);
	if (prev->delete_tag != false and prev->Ref_count <= 2) {
		List.Delete(prev);
	}
	return *this;
};


template <class T>
ListIterator<T> ListIterator<T>::operator--() {
	List<T> List;
	Node<T>* next = node;
	node = List.acquire(&node, node->prev);
	List.release(&next);
	if (next->delete_tag != false and next->Ref_count <= 2) {
		List.Delete(next);
	}
	return *this;
};


template<class T>
Node<T>* List<T>::acquire(Node<T>** node, Node<T>* pnode) {
	Node<T>* AcNode = *node;
	List<T> List;
	AcNode = pnode;
	AcNode->Ref_count += 1;
	return AcNode;
};


template<class T>
Node<T>* List<T>::release(Node<T>** node) {
	Node<T>* AcNode = *node;
	AcNode->Ref_count -= 1;
	return AcNode;
};


template<class T>
int List<T>::getRef_count(Node<T>* node) {
	return node->Ref_count;
}


template<class T>
Node<T>* List<T>::NEXT(Node<T>* node) {
	return node->next;
}


template<class T>
T List<T>::getValue(Node<T>* ptr) {
	if (ptr != nullptr) {
		return ptr->Value;
	}
	else {
		return -1;
	}
}


template<class T>
Node<T>* List<T>::getFirst() {
	if (countNode == 0) {
		head = start;
	}
	return head;
}


template<class T>
Node<T>* List<T>::getLast() {
	if (countNode == 0) {
		end = finish;
	}
	return end;
}


template<class T>
void List<T>::PushFront(T value) {
	Node<T>* element = new Node<T>();
	start->next = head;
	finish->prev = end;
	element->prev = start;
	element->Value = value;

	if (countNode > 0) {
		element->next = head;
		head->prev = element;
		head = element;
	}
	if (countNode == 0) {
		element->next = finish;
		head = end = element;
	}
	countNode++;
}


template<class T>
void List<T>::Delete(Node<T>* node) {
	Node<T>* next_node = new Node<T>();
	Node<T>* prev_node = new Node<T>();
	next_node = node->next;
	prev_node = node->prev;

	if (node == head) {
		if (node->next != finish) {
			head = node->next;
		}
		else {
			head = node->prev;
		}
	}

	if (node == end) {
		if (node->prev != start) {
			end = node->prev;
		}
		else {
			end = node->next;
		}
	}

	if (next_node != nullptr) {
		next_node->prev = prev_node;
	}

	if (prev_node != nullptr) {
		prev_node->next = next_node;
	}

	if (countNode != 0) {
		countNode--;
	}
	
	if (node->Ref_count > 2) {
		node->delete_tag = true;
	}
	else if (node != start and node != finish){
		delete node;
		
	}
}