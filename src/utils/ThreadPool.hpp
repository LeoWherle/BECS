/*
** EPITECH PROJECT, 2024
** CpProcess-thread [WSL: Ubuntu]
** File description:
** ThreadPool
*/

#pragma once

#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {

public:
    ThreadPool(size_t numThreads = std::thread::hardware_concurrency())
    {
        auto thead_func = [this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(_queueMutex);
                    _queueCondition.wait(lock, [this] {
                        return _stop || !_taskQueue.empty();
                    });
                    if (_stop && _taskQueue.empty()) {
                        return;
                    }
                    task = std::move(_taskQueue.front());
                    _taskQueue.pop();
                }
                task();
            }
        };

        for (size_t i = 0; i < numThreads; ++i) {
            _threads.emplace_back(thead_func);
        }
    }

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;

    ~ThreadPool()
    {
        {
            std::lock_guard<std::mutex> lock(_queueMutex);
            _stop = true;
        }
        _queueCondition.notify_all();
        for (std::thread &thread : _threads) {
            thread.join();
        }
    }

    template<typename F, typename... Args>
    void enqueue(F &&f, Args &&...args)
    {
        auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        {
            std::lock_guard<std::mutex> lock(_queueMutex);
            _taskQueue.emplace(task);
        }
        _queueCondition.notify_one();
    }

private:
    std::vector<std::thread> _threads;
    std::queue<std::function<void()>> _taskQueue;
    std::mutex _queueMutex;
    std::condition_variable _queueCondition;
    bool _stop = false;
};
