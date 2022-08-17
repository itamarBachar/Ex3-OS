////
//// Created by itamar on 5/31/22.
////
//
//#include "QueueUnbounded.h"
//#include <semaphore.h>
//#include <string>
//#include <mutex>
//#include <queue>
//#include <iostream>
//
//using namespace std;
//class QueueUnBounded {
//public:
//    queue<string> Queue;
//    mutex mtx;
//    int size;
//    sem_t is_full;
//
//    QueueUnBounded() {
//        sem_init(&this->is_full, 0, 0);
//    }
//
//    void my_push(std::string article) {
//        this->mtx.lock();       /* enter critical section */
//        this->Queue.push(article);   /* modify shared buffer pool */
//        this->mtx.unlock();        /* leave critical section */
//        sem_post(&this->is_full);        /* one more full  buffer  */
//    }
//    string my_pop() {
//        std:: string article;
//        sem_wait(&this->is_full);
//        this->mtx.lock();
//        string s = Queue.front();
//        Queue.pop();
//        this->mtx.unlock();
//        return s;
//    }
//
//};