#include "Header.h"
#include <iostream>



using namespace std;

int main() {

	List<int> list;

	ListIterator<int> iter;
	ListIterator<int> p;
	ListIterator<int> k;

	list.PushFront(4);
	list.PushFront(3);
	list.PushFront(2);
	list.PushFront(1);



	//cout << list.getRef_count(list.getFirst()) << " " << list.getRef_count(list.getLast()) << endl;
	//iter = list.getFirst();
	//p = list.getLast();
	//cout << *iter << " " << *p << endl;
	//cout << list.getRef_count(list.getFirst()) << " " << list.getRef_count(list.getLast()) << endl;


	//iter = list.getFirst();
	//k = list.getFirst();
	//cout << list.getRef_count(list.getFirst());


	/*iter = list.getFirst();
	cout << *iter << " " << list.getRef_count(list.NEXT(list.getFirst())) << endl;
	++iter;
	cout << *iter << " " << list.getRef_count(list.NEXT(list.getFirst())) << endl;
	--iter;
	cout << *iter << " " << list.getRef_count(list.NEXT(list.getFirst()));*/


	/*iter = list.getFirst();
    ++iter;
	cout << *iter << endl;
	list.Delete(list.NEXT(list.getFirst()));
	cout << *iter << " " << list.getValue(list.NEXT(list.getFirst())) << endl;
	cout << *(++iter) <<  " " << *(--iter) << " " << *(++iter);*/

	/*iter = list.getFirst();
	p = list.getFirst();
	++p;
	++iter;
	cout << *iter << endl;
	list.Delete(list.NEXT(list.getFirst()));
	cout << *iter << " " << list.getValue(list.NEXT(list.getFirst())) << endl;
	cout << *(++iter) << " " << *(--iter) << " " << *(++iter) << endl;
	cout << *p;*/



	//list.PushFront(1);
	//iter = list.getFirst();
	//cout << list.getRef_count(list.getFirst()) << endl;

	//list.Delete(list.getFirst());
	//--iter;
	//cout << list.getValue(list.getFirst()) << endl;

	//list.Delete(list.getFirst());
	//cout << list.getValue(list.getFirst()) << endl;

	//list.Delete(list.getLast());
	//cout << list.getValue(list.getLast()) << endl;

}