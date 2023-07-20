#include "./threadpool.h"

namespace hive
{
    ThreadPool::ThreadPool() : m_Threads(std::thread::hardware_concurrency()), m_RunningStatus(true)
    {
        initialize();
    } 
    ThreadPool::ThreadPool(int threadNum) : m_Threads(threadNum), m_RunningStatus(true)
    {
        initialize();
    }
    ThreadPool::~ThreadPool()
    {
        m_RunningStatus = false;
        m_CV.notify_all();
        for (auto &m_thread : m_Threads)
            if (m_thread.joinable())
                m_thread.join();
    }
    void ThreadPool::initialize()
    {
        for (int i = 0; i < m_Threads.size(); i++)
        {
            auto worker = [this, i]()
            {
                while (this->m_RunningStatus)
                {
                    TaskType task;
                    bool isSuccess = false;
                    {
                        std::unique_lock<std::mutex> locker(this->m_Mutex);
                        if (this->m_TaskQueue.empty())
                        {
                            this->m_CV.wait(locker);
                        }
                        isSuccess = this->m_TaskQueue.pop(task);
                    }
                    if (isSuccess)
                    {
                        cout << "Start running task in worker:[ID]" << i << "\n";
                        task();
                        cout << "End running task in worker:[ID] " << i << "\n";
                    }
                }
            };
            m_Threads[i] = std::thread(worker);
        }
    }

}