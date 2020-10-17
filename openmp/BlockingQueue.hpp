#pragma once
#include<iostream>
#include<condition_variable>
#include<mutex>
#include<queue>
#include<thread>
#include<chrono>
#include<atomic>
#include<algorithm>

using namespace std;

template<class T>
class BlockingQueue {
public:
	BlockingQueue(size_t max_size = 1024) : m_max_size(max_size) {}

	void push(T &value) {
		unique_lock<mutex> lock(m_mutex);
		m_cv.wait(lock, [&]() { return m_queue.size() < m_max_size; });
		m_queue.emplace_back(move(value));
		m_cv.notify_one();
	}

	T pop() {
		unique_lock<mutex> lock(m_mutex);
		m_cv.wait(lock, [&]() { return m_queue.size() > 0; });
		T ret = move(m_queue.front());
		m_queue.pop_front();
		m_cv.notify_one();
		return ret;
	}

    bool offer(T &value){
        unique_lock<mutex> lock(m_mutex);
        if( m_queue.size() >= m_max_size ){
            return false;
        }
        m_queue.emplace_back(move(value));
		m_cv.notify_one();
        return true;
    }

private:
	size_t m_max_size;
	mutex m_mutex;
	deque<T> m_queue;
	condition_variable m_cv;
};