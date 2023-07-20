#ifndef __SAFECOUT
#define __SAFECOUT
#include <iostream>
#include <mutex>
#include <sstream>

// namespace hive
// {
//     class SafeCout
//     {
//     public:
//         template <typename T>
//         SafeCout &operator<<(const T &);
//         SafeCout &operator<<(std::ostream &(*)(std::ostream &));

//     private:
//         static std::mutex mtx;
//     };
// }
// #endif
// template <typename T>
// hive::SafeCout &hive::SafeCout::operator<<(const T &data)
// {
//     std::lock_guard<std::mutex> locker(mtx);
//     std::cout << data;
//     return *this;
// }
namespace hive
{
    class SafeCout
    {
    public:
        template <typename T>
        SafeCout &operator<<(const T &data)
        {
            std::lock_guard<std::mutex> locker(mtx);
            std::cout << data;
            return *this;
        }

        SafeCout &operator<<(std::ostream &(*func)(std::ostream &))
        {
            std::lock_guard<std::mutex> locker(mtx);
            func(std::cout);
            return *this;
        }

    private:
        static std::mutex mtx;
    };
}
#endif