#include "gtest/gtest.h"
#include "../Consist_list.h"
#include <thread>
#include <vector>

using namespace std;
//
//TEST(ListTestOneThread, InsertFront) {
//
//    List<int> list;
//    ListIterator<int> iter = list.begin();
//
//    int n_list = 1000;
//
//    for (int i = n_list; i > 0; i--) {
//        list.Insert(iter, i);
//    }
//
//    EXPECT_EQ(list.CountNode(), n_list);
//
//    EXPECT_EQ(list.getValue(list.getFirst()), 1);
//    EXPECT_EQ(list.getValue(list.next(list.getFirst())), 2);
//
//    EXPECT_EQ(list.getValue(list.getLast()), 1000);
//    EXPECT_EQ(list.getValue(list.prev(list.getLast())), 999);
//}
//
//TEST(ListTestOneThread, InsertBack) {
//
//    List<int> list;
//    ListIterator<int> iter = list.end();
//
//    int n_list = 100;
//
//    for (int i = n_list; i > 0; i--) {
//        list.Insert(iter, i);
//    }
//
//    EXPECT_EQ(list.CountNode(), n_list);
//    EXPECT_EQ(list.getValue(list.getFirst()), 100);
//    EXPECT_EQ(list.getValue(list.getLast()), 1);
//}
//
//
//TEST(ListTestOneThread, PushFront) {
//
//    List<int> list;
//
//    int n_list = 1000;
//
//    for (int i = n_list; i > 0; i--) {
//        list.PushFront(i);
//    }
//
//    EXPECT_EQ(list.CountNode(), n_list);
//
//    EXPECT_EQ(list.getValue(list.getFirst()), 1);
//    EXPECT_EQ(list.getValue(list.next(list.getFirst())), 2);
//
//    EXPECT_EQ(list.getValue(list.getLast()), 1000);
//    EXPECT_EQ(list.getValue(list.prev(list.getLast())), 999);
//}
//
//
//
//TEST(ListTestOneThread, PushBack) {
//
//    List<int> list;
//
//    int n_list = 100;
//
//    for (int i = n_list; i > 0; i--) {
//        list.PushBack(i);
//    }
//
//    EXPECT_EQ(list.CountNode(), n_list);
//
//    EXPECT_EQ(list.getValue(list.getLast()), 1);
//    EXPECT_EQ(list.getValue((list.getFirst())), 100);
//}
//
//
//TEST(ListTestOneThread, Erase){
//
//    List<int> list;
//
//    int n_list = 1000;
//
//    for (int i = 0; i < n_list; i++){
//        list.PushFront(i);
//    }
//
//    EXPECT_EQ(list.CountNode(), n_list);
//    EXPECT_EQ(list.getValue(list.prev(list.getLast())), 1);
//
//    list.Erase(list.prev(list.getLast()));
//
//    EXPECT_EQ(list.getValue(list.prev(list.getLast())), 2);
//    EXPECT_EQ(list.getValue(list.next(list.prev(list.getLast()))), 0);
//    EXPECT_EQ(list.CountNode(), n_list - 1);
//}







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

    List<int> list;
    vector<thread> threads;


    int n_list = 1000;
    int n_thread = 2;

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


//    auto functionErase = [&](){
//        list.Erase(list.prev(list.getLast()));
//    };
//
//    for (int i = 0; i < n_thread; i++){
//        threads.emplace_back(functionErase);
//    }
//
//    for (auto & active_thread : threads){
//        if (active_thread.joinable())
//            active_thread.join();
//    }
//
//
//    EXPECT_EQ(list.getValue(list.prev(list.getLast())), 2);
//    EXPECT_EQ(list.getValue(list.next(list.prev(list.getLast()))), 0);
//    EXPECT_EQ(list.CountNode(), n_list - 1);
}
