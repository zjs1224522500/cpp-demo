#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <unistd.h>

using namespace std;

/**
 *                USER VIEW
 <-- PID 43 --> <----------------- PID 42 ----------------->
                     +---------+
                     | process |
                    _| pid=42  |_
                  _/ | tgid=42 | \_ (new thread) _
       _ (fork) _/   +---------+                  \
      /                                        +---------+
+---------+                                    | process |
| process |                                    | pid=44  |
| pid=43  |                                    | tgid=42 |
| tgid=43 |                                    +---------+
+---------+
 <-- PID 43 --> <--------- PID 42 --------> <--- PID 44 --->
                     KERNEL VIEW
 * 
*/

void f1(int n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread[F1] std::thread::get_id " << std::this_thread::get_id() << 
            " gettid " << gettid() << " getpid " << getpid() << " executing with N = " << n << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));
    }
}

void f2(int& n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread[F2] std::thread::get_id " << std::this_thread::get_id() << 
            " gettid " << gettid() << " getpid " << getpid() <<  " executing with N = " << n << std::endl;
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));
    }
}

/**
 * 1. When you create a new process,  a new pid and tgid would be assigned, and pid = tgid.
 * 2. When you create a new thread, a new pid would be assigned, but tgid is same with previous one.
 * 3. The pid for process is process id (getpid), and for thread is tid (gettid).
 * 4. The pid for process is same with the tid of process.
 * 5. std::this_thread::get_id() returns an id object, not a simple number.  
*/
int main() {
    std::cout << "Hello htop !!!" << std::endl;
    int n = 0;
    std::cout << "Thread[Main] std::thread::get_id " << std::this_thread::get_id() << 
            " gettid " << gettid() << " getpid " << getpid() <<  " executing with N = " << n << std::endl;


    std::thread t1; // t1 is not a thread
    std::thread t2(f1, n + 1); // pass by value
    std::thread t3(f2, std::ref(n)); // pass by reference
    std::thread t4(std::move(t3)); // t4 is now running f2(). t3 is no longer a thread
    t2.join();
    t4.join();
    std::cout << "Final value of n is " << n << '\n';

    return 0;    
}