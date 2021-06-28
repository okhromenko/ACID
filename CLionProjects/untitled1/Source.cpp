//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#ifdef _DEBUG
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
//#endif

#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "Consist_list.h"
#include <iostream>
#define N 1000

TEST_CASE("iterator: begin(), end(); getRef_count()") {

	List<int> list;

	ListIterator<int> iter_begin = list.begin();
    ListIterator<int> iter_end = list.end();

	REQUIRE(*(iter_begin) == -1);
    REQUIRE(*(iter_end) == -2);

    for (int i = N; i > 0; i--){
        list.PushFront(i);
    }

	++iter_begin;
	REQUIRE(list.getRef_count((list.getFirst())) == 3);

	++iter_begin;
    REQUIRE(list.getRef_count((list.getFirst())) == 2);

    --iter_begin;
    --iter_begin;

    for (int i = N; i > 0; i--){
        ++iter_begin;
    }

    --iter_end;
    REQUIRE(list.getRef_count((list.getLast())) == 4);

    --iter_end;
    REQUIRE(list.getRef_count((list.getLast())) == 3);
}


TEST_CASE("Erase()") {

    List<int> list;

    for (int i = 0; i < N; i++){
        list.PushFront(i);
    }

    REQUIRE(list.CountNode() == N);

    REQUIRE(list.getValue(list.prev(list.getLast())) == 1);

    list.Erase(list.prev(list.getLast()));

    REQUIRE(list.getValue(list.prev(list.getLast())) == 2);
    REQUIRE(list.getValue(list.next(list.prev(list.getLast()))) == 0);
    REQUIRE(list.CountNode() == N - 1);

    for (int i = 0; i < N - 1; i++){
        list.Erase((list.getLast()));
    }

    REQUIRE(list.CountNode() == 0);
}


TEST_CASE("Erase() with iter") {

    List<int> list;

    for (int i = N; i > 0; i--){
        list.PushFront(i);
    }

    ListIterator<int> iter_begin = list.begin();
    ListIterator<int> iter_end = list.end();

    list.Erase((list.getFirst()));

    REQUIRE(list.getRef_count(list.getFirst()) == 2);
    REQUIRE(list.getValue(list.getFirst()) == 2);

    ++iter_begin;

    REQUIRE(list.getRef_count(list.getFirst()) == 3);
    REQUIRE(list.getRef_count(list.next(list.getFirst())) == 2);
}

TEST_CASE("iter") {

    List<int> list;

    for (int i = 100; i > 0; i--){
        list.PushFront(i);
    }

    ListIterator<int> iter = list.begin();

    REQUIRE(*(iter) == 1);
    REQUIRE(list.getRef_count(list.next(list.getFirst())) == 2);

    ++iter;

    REQUIRE(*(iter) == 2);
    REQUIRE(list.getRef_count(list.next(list.getFirst())) == 3);

    --iter;

    REQUIRE(*(iter) == 1);
    REQUIRE(list.getRef_count(list.next(list.getFirst())) == 2);
}

TEST_CASE("wowo") {

    List<int> list;

    for (int i = 100; i > 0; i--){
        list.PushFront(i);
    }

    ListIterator<int> iter = list.begin();

    ++iter;

    list.Erase(list.next(list.getFirst()));

    REQUIRE(*iter == 2);
    REQUIRE(list.getValue(list.next(list.getFirst())) == 3);


    REQUIRE(*(++iter) == 3);
    REQUIRE(*(--iter) == 1);
    REQUIRE(*(++iter) == 3);
}


TEST_CASE("wgfgfwo") {

    List<int> list;

    for (int i = 100; i > 0; i--){
        list.PushFront(i);
    }

    ListIterator<int> iter = list.begin();
    ListIterator<int> p = list.begin();
    ++iter;
    ++p;

    REQUIRE(*iter == 2);

    list.Erase(list.next(list.getFirst()));

    REQUIRE(*iter == 2);
    REQUIRE(list.getValue(list.next(list.getFirst())) == 3);


    REQUIRE(*(++iter) == 3);
    REQUIRE(*(--iter) == 1);
    REQUIRE(*(++iter) == 3);
    REQUIRE(*p == 2);
}
