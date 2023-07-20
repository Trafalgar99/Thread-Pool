#include <threadpool.h>
#include <safecout.h>
#include <semaphore>

using namespace std;


int very_time_consuming_task(int a, int b)
{
    this_thread::sleep_for(chrono::seconds(1));
    return a + b;
}

int main()
{
    hive::ThreadPool tp(12);
    hive::SafeCout cout;
    int taskNum = 30;
    vector<future<int>> result(taskNum);
    cout << "Start to submit tasks ... \n";
    for (int i = 0; i < taskNum; i++)
    {
        result[i] = tp.submitTask(very_time_consuming_task, i, i + 1);
    }
    cout << "End to submit tasks ...\n";
    cout << "Main thread do somethings else...\n";
    // do something else
    this_thread::sleep_for(chrono::seconds(5));
    //
    cout << "Main thread task finished... \n";

    cout << "Try getting threadpool task result...\n";
    for (int i = 0; i < taskNum; i++)
    {
        cout << "result[ " << i << " ]: " << result[i].get() << "\n";
    }
    cout << "End of getting result \n";

    cout << __cplusplus << "\n";
}