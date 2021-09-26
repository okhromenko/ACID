#include "gtest/gtest.h"
#include "../Consist_list.h"
#include <thread>
#include <barrier>
#include <condition_variable>
#include <vector>

using namespace std;

TEST(ListTestOneThread, InsertFront) {

    List<int> list;
    ListIterator<int> iter = list.begin();

    int n_list = 1000;

    for (int i = n_list; i > 0; i--) {
        list.Insert(iter, i);
    }

    EXPECT_EQ(list.CountNode(), n_list);

    EXPECT_EQ(list.getValue(list.getFirst()), 1);
    EXPECT_EQ(list.getValue(list.next(list.getFirst())), 2);

    EXPECT_EQ(list.getValue(list.getLast()), 1000);
    EXPECT_EQ(list.getValue(list.prev(list.getLast())), 999);
}

TEST(ListTestOneThread, InsertBack) {

    List<int> list;
    ListIterator<int> iter = list.end();

    int n_list = 100;

    for (int i = n_list; i > 0; i--) {
        list.Insert(iter, i);
    }

    EXPECT_EQ(list.CountNode(), n_list);
    EXPECT_EQ(list.getValue(list.getFirst()), 100);
    EXPECT_EQ(list.getValue(list.getLast()), 1);
}


TEST(ListTestOneThread, PushFront) {

    List<int> list;

    int n_list = 1000;

    for (int i = n_list; i > 0; i--) {
        list.PushFront(i);
    }

    EXPECT_EQ(list.CountNode(), n_list);

    EXPECT_EQ(list.getValue(list.getFirst()), 1);
    EXPECT_EQ(list.getValue(list.next(list.getFirst())), 2);

    EXPECT_EQ(list.getValue(list.getLast()), 1000);
    EXPECT_EQ(list.getValue(list.prev(list.getLast())), 999);
}



TEST(ListTestOneThread, PushBack) {

    List<int> list;

    int n_list = 100;

    for (int i = n_list; i > 0; i--) {
        list.PushBack(i);
    }

    EXPECT_EQ(list.CountNode(), n_list);

    EXPECT_EQ(list.getValue(list.getLast()), 1);
    EXPECT_EQ(list.getValue((list.getFirst())), 100);
}


TEST(ListTestOneThread, Erase){

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
}







TEST(ListTestMultThreads, InsertFront) {

    List<int> list;
    ListIterator<int> iter = list.begin();
    vector<thread> threads;

    int n_thread = 10;
    int n_list = 1000;

    auto functionInsert = [&]() {
        for (int i = n_list; i > 0; i--) {
            list.Insert(iter, 1);
        }
    };

    for (int i = 0; i < n_thread; i++){
        threads.emplace_back(functionInsert);
    }

    for (auto & active_thread : threads){
        if (active_thread.joinable())
            active_thread.join();
    }

    EXPECT_EQ(list.CountNode(), n_list * n_thread);
}

TEST(ListTestMultThreads, PushFront) {

    List<int> list;
    vector<thread> threads;

    int n_list = 1000;
    int n_thread = 20;

    auto functionPushFront = [&](){
        for (int i = n_list; i > 0; i--) {
            list.PushFront(1);
        }
    };

    for (int i = 0; i < n_thread; i++){
        threads.emplace_back(functionPushFront);
    }

    for (auto & active_thread : threads){
        if (active_thread.joinable())
            active_thread.join();
    }

    EXPECT_EQ(list.CountNode(), n_list * n_thread);
}

TEST(ListTestMultThreads, PushBack) {

    List<int> list;
    vector<thread> threads;

    int n_list = 100;
    int n_thread = 20;

    auto functionPushBack = [&](){
        for (int i = n_list; i > 0; i--) {
            list.PushBack(1);
        }
    };

    for (int i = 0; i < n_thread; i++){
        threads.emplace_back(functionPushBack);
    }

    for (auto & active_thread : threads){
        if (active_thread.joinable())
            active_thread.join();
    }

    EXPECT_EQ(list.CountNode(), n_list * n_thread);
}


TEST(ListTestMultThreads, Erase){

    int n_list = 1000;
    int n_thread = 10;

    List<int> list;
    vector<thread> threads;
    barrier point_syn(n_thread);

   auto funcPushFrontWithErase = [&](){
        for (int i = n_list; i > 0; i--) {
            list.PushFront(1);
        }

       point_syn.arrive_and_wait();

       auto ptr_node = list.getFirst();

       for (int i = n_list; i > 1; i--){
           list.Erase(ptr_node);
           ptr_node = list.getFirst();
       }
    };

    for (int i = 0; i < n_thread; i++){
        threads.emplace_back(funcPushFrontWithErase);
    }

    for (auto & active_thread : threads){
        if (active_thread.joinable())
            active_thread.join();
    }

    EXPECT_EQ(list.CountNode(), n_thread);
}

TEST(ListTestMultThreads, IterBegin){

    int n_list = 1000;
    int n_thread = 10;

    List<int> list;
    ListIterator<int> iter;
    vector<thread> threads;

    auto funcRefCount = [&](){
        for (int i = n_list; i > 0; i--) {
            list.PushBack(i);
        }

        iter = list.begin();
    };

    for (int i = 0; i < n_thread; i++){
        threads.emplace_back(funcRefCount);
    }

    for (auto & active_thread : threads){
        if (active_thread.joinable())
            active_thread.join();
    }

    EXPECT_EQ(list.getRef_count(list.getFirst()), n_thread + 2);
    EXPECT_EQ(   list.CountNode(), n_thread * n_list);
}

TEST(ListTestMultThreads, Increment){

    int n_list = 1000;
    int n_thread = 50;

    List<int> list;
    ListIterator<int> iter_begin;
    ListIterator<int> iter_end;
    vector<thread> threads;

    for (int i = n_list; i > 0; i--) {
        list.PushFront(i);
    }

    iter_begin = list.begin();
    iter_end = list.end();

    auto funcInc = [&](){
        ++iter_begin;
        --iter_end;

        EXPECT_EQ(list.CountNode(), n_list);
    };

    for (int i = 0; i < n_thread; i++){
        threads.emplace_back(funcInc);
    }

    for (auto & active_thread : threads){
        if (active_thread.joinable())
            active_thread.join();
    }

    EXPECT_EQ(*iter_begin, n_thread + 1);
    EXPECT_EQ(*iter_end, n_list - n_thread);
}


