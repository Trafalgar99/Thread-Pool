#pragma once

#include <iostream>
#include <safequeue.h>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <vector>
#include <memory>
#include <future>
#include <safecout.h>
// 单任务队列 线程池
// 提交的任务：普通函数、匿名函数（lambda函数）、仿函数、类的成员函数、std::function/std::packaged_task
// 返回值不同，参数列表也不同

namespace hive
{
    class ThreadPool
    {
    public:
        ThreadPool();
        ThreadPool(int);
        ThreadPool(ThreadPool &) = delete;
        ThreadPool(ThreadPool &&) = delete;
        ThreadPool &operator=(ThreadPool &) = delete;
        ThreadPool &operator=(ThreadPool &&) = delete;
        template <typename Func, typename... Args>
        auto submitTask(Func &&func, Args... args);
        ~ThreadPool();

    private:
        using TaskType = std::function<void()>;
        SafeQueue<TaskType> m_TaskQueue;
        std::vector<std::thread> m_Threads;
        std::condition_variable m_CV;
        std::mutex m_Mutex;
        std::atomic<bool> m_RunningStatus;
        SafeCout cout;

        void initialize();
    };
}

template <typename Func, typename... Args>
auto hive::ThreadPool::submitTask(Func &&func, Args... args)
{
    using returnType = typename std::invoke_result<Func, Args...>::type;
    std::function<returnType()> taskWrapper1 = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    auto taskWrapper2 = std::make_shared<std::packaged_task<returnType()>>(taskWrapper1);
    TaskType wrapperFunction = [taskWrapper2]()
    {
        (*taskWrapper2)();
    };

    m_TaskQueue.push(wrapperFunction);
    m_CV.notify_one();

    return taskWrapper2->get_future();
}
