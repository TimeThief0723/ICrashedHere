#pragma
#include <thread>
#include <future>
#include <functional>
#include <vector>
#include <utility>
#include "BlockingQueue.hpp"

using namespace std;

class ThreadPool{
public:
    ThreadPool(int thread_num = 32){
        thread_num_ = thread_num;
        for(int i = 0; i < thread_num_; i++){
            threads_.emplace_back(&ThreadPool::loop, this);
        }
    }

    void enqueue(function<int(void)> func){
        tasks_.push(func);
    }

private:
    void loop(){
        while(!finished){
            auto task = tasks_.pop();
            task();
        }
    }
    BlockingQueue<function<int(void)> > tasks_;
    size_t thread_num_;
    vector<thread> threads_;
    bool finished = false;
};