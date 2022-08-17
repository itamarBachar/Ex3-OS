// 318781630 Itamar Bachar.
#include <iostream>
#include <thread>
#include <chrono>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <mutex>
#include <queue>
#include <iostream>
#include <semaphore.h>

using namespace std;
class QueueUnBounded {
public:
    queue<string> Queue;
    mutex mtx;
    int size;
    sem_t is_full;

    QueueUnBounded() {
        sem_init(&this->is_full, 0, 0);
    }

    void my_push(std::string article) {
        this->mtx.lock();       /* enter critical section */
        this->Queue.push(article);   /* modify shared buffer pool */
        this->mtx.unlock();        /* leave critical section */
        sem_post(&this->is_full);        /* one more full  buffer  */
    }
    string my_pop() {
        std:: string article;
        sem_wait(&this->is_full);
        this->mtx.lock();
        string s = Queue.front();
        Queue.pop();
        this->mtx.unlock();
        return s;
    }

};
class QueueBounded {
public:
    queue<string> Queue;
    mutex mtx;
    int size;
    sem_t is_full;
    sem_t is_empty;

    QueueBounded(int a) {
        this->size = a;
        sem_init(&this->is_empty, 0, a);
        sem_init(&this->is_full, 0, 0);
    }

    void my_push(std::string article) {
        sem_wait(&this->is_empty);        /* one less empty buffer  */
        this->mtx.lock();       /* enter critical section */
        this->Queue.push(article);   /* modify shared buffer pool */
        this->mtx.unlock();        /* leave critical section */
        sem_post(&this->is_full);        /* one more full  buffer  */
    }

    string my_pop() {
        sem_wait(&this->is_full);
        this->mtx.lock();
        string s = Queue.front();
        Queue.pop();
        this->mtx.unlock();
        sem_post(&this->is_empty);
        return s;
    }

};
string Reports[3] = {"Sport", "News", "Weather"};
vector<QueueBounded *> dispatcher_array;
QueueUnBounded *news;
QueueUnBounded *sport;
QueueUnBounded *weather;
QueueBounded *for_screen;

void produce_article(int produce_number, int amount_of_article) {
    for (int i = 0; i <= amount_of_article; i++) {
        if (i == amount_of_article) {
            dispatcher_array[produce_number]->my_push("-1");
            break;
        }
        int j = i % 3;
        dispatcher_array[produce_number]->my_push(to_string(produce_number) + " " + Reports[j] + " " + to_string(i));
    }
}

void dispatcher(int size_of_produce) {
    int flag = size_of_produce;
    int index;
    int pos = 0;
    int flag2 = 0;
    vector<int> j;
    while (flag) {
        for (int i = 0; i < size_of_produce; i++) {
            for (int k = 0; k < j.size(); k++) {
                if (i == j[k]) {
                    flag2 = 1;
                }
            }
            if (flag2 == 1) {
                flag2 = 0;
                continue;
            }
            string s = dispatcher_array[i]->my_pop();
            int res = s.compare("-1");
            if (res != 0) {
                if ((index = s.find("Sport", pos)) != string::npos) {
                    sport->my_push(s);
                }
                if ((index = s.find("News", pos)) != string::npos) {
                    news->my_push(s);
                }
                if ((index = s.find("Weather", pos)) != string::npos) {
                    weather->my_push(s);
                }
            } else {
                flag--;
                j.push_back(i);
            }
        }
    }
    weather->my_push("-1");
    sport->my_push(("-1"));
    news->my_push("-1");

}

void reporterSport() {
    string s = sport->my_pop();
    int res = s.compare("-1");
    if (res == 0) {
        for_screen->my_push("-1");
        return;
    }
    s += " After co-edit3";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for_screen->my_push(s);
    while (res != 0) {
        s = sport->my_pop();
        res = s.compare("-1");
        if (res == 0) {
            for_screen->my_push("-1");
            break;
        }
        s += " After co-edit3";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        for_screen->my_push(s);
    }
}

void reporterNews() {
    string s = news->my_pop();
    int res = s.compare("-1");
    if (res == 0) {
        for_screen->my_push("-1");
        return;
    }
    s += " After co-edit2";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for_screen->my_push(s);
    while (res != 0) {
        s = news->my_pop();
        res = s.compare("-1");
        if (res == 0) {
            for_screen->my_push("-1");
            break;
        }
        s += " After co-edit2";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        for_screen->my_push(s);
    }
}

void print_to_screen() {
    int flag = 1;
    int counter = 0;
    while (flag) {
        if (counter == 3) {
            flag = 0;
            break;
        }
        string s = for_screen->my_pop();
        int res = s.compare("-1");
        if (res == 0) {
            counter++;
        } else {
            s += " Now publish";
            cout << s << endl;
        }
    }
}

void reporterWeather() {
    string s = weather->my_pop();
    int res = s.compare("-1");
    if (res == 0) {
        for_screen->my_push("-1");
        return;
    }
    s += " After co-edit1";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for_screen->my_push(s);
    while (res != 0) {
        s = weather->my_pop();
        res = s.compare("-1");
        if (res == 0) {
            for_screen->my_push("-1");
            break;
        }
        s += " After co-edit1";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        for_screen->my_push(s);
    }
}

int main(int argc, char **argv) {
    vector<string> input;
    fstream new_file;
    string path = "./";
    // replace to ./ !!!!!!!!!!!!!!!!!!!!!!
    path.append(argv[1]);
    new_file.open(path, ios::in);
    string s2;
    while ((getline(new_file, s2))) {
        if (s2 != "") {
            input.push_back(s2);
        }
        if (!std::getline(new_file, s2)) {
            break;
        }
        if (s2 != "") {
            input.push_back(s2);
        }
    }
    news = new QueueUnBounded();
    weather = new QueueUnBounded();
    sport = new QueueUnBounded();
    int size_of_produce = input.size() / 3;
    stringstream geek(s2);
    int x = 0;
    geek >> x;
    for_screen = new QueueBounded(x);
    std::vector<thread> threads(size_of_produce);
    for (int i = 0; i < size_of_produce; i++) {
        int size_queue2 = 0;
        int j = i * 3;
        int k = i * 3;
        for (; j < k + 3; j++) {
            if (j == k + 2) {
                string convert = input[j];
                stringstream geek2(convert);
                geek2 >> size_queue2;
            }
        }
        QueueBounded *q = new QueueBounded(size_queue2);
        dispatcher_array.push_back(q);
    }
    for (int i = 0; i < threads.size(); i++) {
        int j = i * 3;
        int k = i * 3;
        int amount2 = 0;
        for (; j < k + 3; j++) {
            if (j == k + 1) {
                string convert = input[j];
                stringstream geek2(convert);
                geek2 >> amount2;
            }
        }
        threads[i] = thread(produce_article, i, amount2);
    }
    thread thread1(dispatcher, size_of_produce);
    thread tr(reporterNews);
    thread tr2(reporterSport);
    thread tr3(reporterWeather);
    thread tr4(print_to_screen);
    for (auto &th: threads) {
        th.join();
    }
    thread1.join();
    tr.join();
    tr2.join();
    tr3.join();
    tr4.join();

    return 0;
}
