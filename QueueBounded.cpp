//
// Created by itamar on 5/30/22.
//
#include <semaphore.h>
#include "QueueBounded.h"

using namespace std;

//class QueueBounded {
//public:
//    queue<string> Queue;
//    mutex mtx;
//    int size;
//    sem_t is_full;
//    sem_t is_empty;
//
//    QueueBounded(int a) {
//        this->size = a;
//        sem_init(&this->is_empty, 0, a);
//        sem_init(&this->is_full, 0, 0);
//    }
//
//    void my_push(std::string article) {
//        sem_wait(&this->is_empty);        /* one less empty buffer  */
//        this->mtx.lock();       /* enter critical section */
//        this->Queue.push(article);   /* modify shared buffer pool */
//        this->mtx.unlock();        /* leave critical section */
//        sem_post(&this->is_full);        /* one more full  buffer  */
//    }
//
//    string my_pop() {
////        std::string article;
////        int j;
////        sem_getvalue(&is_full, &j);
//        sem_wait(&this->is_full);
//        this->mtx.lock();
//        string s = Queue.front();
//        Queue.pop();
//        this->mtx.unlock();
//        sem_post(&this->is_empty);
//        return s;
//    }
//
//};