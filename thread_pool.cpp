//
// Created by Administrator on 2024/6/4.
//
#pragma once
#include <mutex>
#include <queue>
#include <memory>
#include <thread>
#include <cstdint>
#include <future>
#include <functional>
#include <iostream>
#include <condition_variable>

class ThreadPool {
protected:
    struct TaskFunc {
        TaskFunc(uint64_t expireTime) : _expireTime(expireTime)
        {}

        uint64_t _expireTime = 0;
        std::function<void()> _func;
    };
    typedef std::shared_ptr<TaskFunc> TaskFuncPtr;

public:
    ThreadPool() {}

    virtual ~ThreadPool() {
        stop();
    }

    // 停止线程
    void stop() {
        if (isTerminate()) {
            return;
        }
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _isTerminate = true;
            _condition.notify_all();
        }
        for (size_t i = 0; i < _threads.size(); i++) {
            if (_threads[i]->joinable()) {
                _threads[i]->join();
            }
            {
                std::unique_lock<std::mutex> lock(_mutex);
                delete _threads[i];
                _threads[i] = NULL;
            }
        }
        std::unique_lock<std::mutex> lock(_mutex);
        _threads.clear();
        std::cout << "线程池停止" << std::endl;
    }

    // 启动线程池
    void start(size_t num) {
        std::unique_lock<std::mutex> lock(_mutex);
        if (!_threads.empty()) {
            std::cout << "thread poll has start!" << std::endl;
            exit(1);
        }
        _threadNum = num;
        _isTerminate = false;
        for (size_t i = 0; i < _threadNum; ++i) {
            _threads.push_back(new std::thread(&ThreadPool::run, this));
        }
    }

    // 启动任务
    template<class F, class... Args>
    auto exec(F&& f, Args &&... args) -> std::future<decltype(f(args...))>
    {
        // 构建函数
        using RetType = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        // 构建func 所在task ptr
        TaskFuncPtr funcPtr = std::make_shared<TaskFunc>(0);
        funcPtr->_func = [task](){
            (*task)();
        };

        // 添加任务
        std::unique_lock<std::mutex> lock(_mutex);
        _tasks.push(funcPtr);
        _condition.notify_one();    // 唤醒一个线程执行任务
        return task->get_future();
    }

    // 是否中断了
    bool isTerminate() {
        std::unique_lock<std::mutex> lock(_mutex);
        return _isTerminate;
    }

protected:
    // 运行
    void run() {
        while (!isTerminate()) {
            TaskFuncPtr task;
            if (get(task)) {
                ++_atomic;
                try {
                    task->_func();
                } catch (...)
                {
                    std::cout << "thread run error!" << std::endl;
                }
                --_atomic;

                // 唤醒所有线程
                std::unique_lock<std::mutex> lock(_mutex);
                if (_atomic == 0 && _tasks.empty()) {
                    _condition.notify_all();
                }
            }
        }
    }

    // 获取任务状态
    bool get(TaskFuncPtr &funcPtr) {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_tasks.empty()) {
            _condition.wait(lock, [&]{ return _isTerminate || !_tasks.empty(); });
        }
        if (_isTerminate) {
            return false;
        }
        if (!_tasks.empty()) {
            funcPtr = std::move(_tasks.front());
            _tasks.pop();
            return true;
        }
        return false;
    }

private:
    // 任务队列
    std::queue<TaskFuncPtr> _tasks;

    // 工作线程
    std::vector<std::thread*> _threads;

    // 锁
    std::mutex _mutex;
    std::condition_variable _condition;

    // 线程数量
    size_t _threadNum;

    // 原子计数器
    std::atomic<int> _atomic{ 0 };

    // 是否中断了
    bool _isTerminate = false;
};

void testFunction(int i) {
    printf("thread start %d\n", i);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("thread finish %d\n", i);
}

int main() {
    ThreadPool threadPool;
    for (int i = 0; i < 8; ++i) {
        threadPool.exec(testFunction, i);
    }
    threadPool.start(3);

    std::this_thread::sleep_for(std::chrono::seconds(20));
    std::cout << "thread finish all!" << std::endl;
    threadPool.stop();
    return 0;
}