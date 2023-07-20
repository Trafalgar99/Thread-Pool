#ifndef __SAFEQUEUE
#define __SAFEQUEUE
#include <iostream>
#include <queue>
#include <mutex>
namespace hive
{
    template <typename T>
    class SafeQueue
    {
    public:
        SafeQueue() = default;
        SafeQueue(const SafeQueue &other) = delete;
        SafeQueue(SafeQueue &&other) = delete;
        SafeQueue &operator=(const SafeQueue &other) = delete;
        SafeQueue &operator=(SafeQueue &&other) = delete;
        ~SafeQueue() = default;
        bool empty();
        int size();
        void push(T &&value);
        void push(T &value);
        bool pop(T &value);

    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;
    };
}
#endif

// 实现
template <typename T>
bool hive::SafeQueue<T>::empty()
{
    std::unique_lock<std::mutex> locker(m_mutex);
    return m_queue.empty();
}
template <typename T>
int hive::SafeQueue<T>::size()
{
    std::unique_lock<std::mutex> locker(m_mutex);
    return m_queue.size();
}
template <typename T>
void hive::SafeQueue<T>::push(T &value)
{
    std::unique_lock<std::mutex> locker(m_mutex);
    m_queue.emplace(value);
}
template <typename T>
void hive::SafeQueue<T>::push(T &&value)
{
    std::unique_lock<std::mutex> locker(m_mutex);
    m_queue.emplace(std::move(value));
}
template <typename T>
bool hive::SafeQueue<T>::pop(T &value)
{
    std::unique_lock<std::mutex> locker(m_mutex);
    if (m_queue.empty())
        return false;
    value = std::move(m_queue.front());
    m_queue.pop();
    return true;
}