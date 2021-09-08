//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#ifdef _DEBUG
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
//#endif


#include "gtest/gtest.h"
#include "../Consist_list.h"
#include <barrier>
#include <thread>


using namespace std;


TEST(ListTest, PushFront) {

List<int> list;

int n_list = 1000;

for (int i = n_list; i > 0; i--){
list.PushFront(i);
}

EXPECT_EQ(list.CountNode(), 1000);

EXPECT_EQ(list.getValue(list.getFirst()), 1);
EXPECT_EQ(list.getValue(list.next(list.getFirst())), 2);

EXPECT_EQ(list.getValue(list.getLast()), 1000);
EXPECT_EQ(list.getValue(list.prev(list.getLast())), 999);

auto nextNode = list.getFirst();

for (int i = 50; i > 1; i--){
nextNode = list.next(nextNode);
}

EXPECT_EQ(list.getValue(nextNode), 50);
}


TEST(ListTest, Insert) {

    List<int> list;
    ListIterator<int> iter = list.begin();

    int n_list = 1000;

    for (int i = n_list; i > 0; i--) {
        list.Insert(iter, i);
    }

    EXPECT_EQ(list.CountNode(), 1000);

    EXPECT_EQ(list.getValue(list.getFirst()), 1);
    EXPECT_EQ(list.getValue(list.next(list.getFirst())), 2);

    EXPECT_EQ(list.getValue(list.getLast()), 1000);
    EXPECT_EQ(list.getValue(list.prev(list.getLast())), 999);

    auto nextNode = list.getFirst();

    for (int i = 50; i > 1; i--) {
        nextNode = list.next(nextNode);
    }
}

TEST(ListTest, InsertBack) {

    List<int> list;
    ListIterator<int> iter = list.end();

    int n_list = 100;

    for (int i = n_list; i > 0; i--) {
        list.Insert(iter, i);
    }

    EXPECT_EQ(list.CountNode(), 100);
    EXPECT_EQ(list.getValue(list.getFirst()), 100);
    EXPECT_EQ(list.getValue(list.getLast()), 1);
}


TEST(ListTest, PushBack) {

    List<int> list;

    int n_list = 100;

    for (int i = n_list; i > 0; i--) {
        list.PushBack(i);
    }

    EXPECT_EQ(list.CountNode(), 100);

    EXPECT_EQ(list.getValue(list.getLast()), 1);
    EXPECT_EQ(list.getValue((list.getFirst())), 100);
}

TEST(ListTest, BeginEnd) {

List<int> list;

int n_list = 1000;

ListIterator<int> iter_begin_off_list = list.begin();
ListIterator<int> iter_end_off_list = list.end();

for (int i = n_list; i > 0; i--){
list.PushFront(i);
}

ListIterator<int> iter_begin_inside_list = list.begin();
ListIterator<int> iter_end_inside_list = list.end();

EXPECT_EQ(*(iter_begin_inside_list), 1);
EXPECT_EQ(*(iter_end_inside_list), 1000);

EXPECT_EQ(*(iter_begin_off_list), 000);
EXPECT_EQ(*(iter_end_off_list), 001);

}

TEST(ListTest, Ref_count){

List<int> list;

int n_list = 1000;

for (int i = n_list; i > 0; i--){
list.PushFront(i);
}


EXPECT_EQ(list.getRef_count(list.getFirst()), 3);
EXPECT_EQ(list.getRef_count(list.next(list.getFirst())), 2);

EXPECT_EQ(list.getRef_count(list.getLast()), 3);
EXPECT_EQ(list.getRef_count(list.prev(list.getLast())), 2);
}

TEST(ListTest, IncDec){

List<int> list;

int n_list = 1000;

ListIterator<int> iter_begin = list.begin();
ListIterator<int> iter_end = list.end();

for (int i = n_list; i > 0; i--){
list.PushFront(i);
}

++iter_begin;
EXPECT_EQ(list.getRef_count((list.getFirst())), 3);

++iter_begin;
EXPECT_EQ(list.getRef_count((list.getFirst())), 2);

--iter_begin;
--iter_begin;

for (int i = n_list; i > 0; i--){
++iter_begin;
}

--iter_end;
EXPECT_EQ(list.getRef_count((list.getLast())), 4);

--iter_end;
EXPECT_EQ(list.getRef_count((list.getLast())), 3);
}

TEST(ListTest, Erase){

List<int> list;

int n_list = 1000;

for (int i = 0; i < n_list; i++){
list.PushFront(i);
}

EXPECT_EQ(list.CountNode(), n_list);

EXPECT_EQ(list.getValue(list.prev(list.getLast())), 1);

list.Erase(list.prev(list.getLast()));

EXPECT_EQ(list.getValue(list.prev(list.getLast())), 2);
EXPECT_EQ(list.getValue(list.next(list.prev(list.getLast()))), 0);
EXPECT_EQ(list.CountNode(), n_list - 1);

for (int i = 0; i < n_list - 1; i++){
list.Erase((list.getLast()));
}

EXPECT_EQ(list.CountNode(), 0);
}

TEST(ListTest, EraseWithIter){

List<int> list;

int n_list = 10;

for (int i = n_list; i > 0; i--){
list.PushFront(i);
}

ListIterator<int> iter_begin = list.begin();

EXPECT_EQ(list.getRef_count(list.getFirst()), 3);
EXPECT_EQ(list.getValue(list.getFirst()), 1);
EXPECT_EQ(*(iter_begin), 1);

list.Erase((list.getFirst()));

EXPECT_EQ(list.getRef_count(list.getFirst()), 2);
EXPECT_EQ(list.getValue(list.getFirst()), 2);
EXPECT_EQ(*(iter_begin), 1);

++iter_begin;

EXPECT_EQ(list.getRef_count(list.getFirst()), 3);
EXPECT_EQ(list.getRef_count(list.next(list.getFirst())), 2);
EXPECT_EQ(*(iter_begin), 2);

--iter_begin;
EXPECT_EQ(*(iter_begin), 000);
}

TEST(ListTest, StepOneIterator){

List<int> list;

int n_list = 10;

for (int i = n_list; i > 0; i--){
list.PushFront(i);
}

ListIterator<int> iter = list.begin();

++iter;

list.Erase(list.next(list.getFirst()));

EXPECT_EQ(*iter, 2);
EXPECT_EQ(list.getValue(list.next(list.getFirst())), 3);


EXPECT_EQ(*(++iter), 3);
EXPECT_EQ(*(--iter), 1);
EXPECT_EQ(*(++iter), 3);
}


TEST(ListTest, StepSeveralIterator){

List<int> list;

int n_list = 10;

for (int i = n_list; i > 0; i--){
list.PushFront(i);
}

ListIterator<int> iter = list.begin();
ListIterator<int> p = list.begin();
++iter;
++p;

EXPECT_EQ(*iter, 2);
EXPECT_EQ(*p, 2);

list.Erase(list.next(list.getFirst()));

EXPECT_EQ(*iter, 2);
EXPECT_EQ(list.getValue(list.next(list.getFirst())), 3);


EXPECT_EQ(*(++iter), 3);
EXPECT_EQ(*(--iter), 1);
EXPECT_EQ(*(++iter), 3);
EXPECT_EQ(*p, 2);
}
