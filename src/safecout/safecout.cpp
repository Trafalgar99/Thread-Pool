#include "safecout.h"

namespace hive
{
    std::mutex SafeCout::mtx;
    // SafeCout &SafeCout::operator<<(std::ostream &(*func)(std::ostream &))
    // {
    //     std::lock_guard<std::mutex> locker(mtx);
    //     func(std::cout);
    //     return *this;
    // }

}