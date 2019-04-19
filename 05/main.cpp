//Valentine Shilov
#include <thread>
#include <condition_variable>
#include <iostream>
#include <mutex>

void thread1(std::condition_variable &c, bool &state, std::mutex &m, int N) // thread 1
{
    std::unique_lock<std::mutex> lock(m);
    for(int i=0;i<N;++i)
    {
        while(!state)
            c.wait(lock);
        std::cout <<"ping" <<std::endl;   //<<i <<std::endl;
        state=!state;
        c.notify_one();
    }
}

void thread2(std::condition_variable &c, bool &state, std::mutex &m, int N) // thread 2
{
    std::unique_lock<std::mutex> lock(m);

    for(int i=0;i<N;++i)
    {
        while(state)
             c.wait(lock);
        std::cout <<"pong" <<std::endl; // <<i  <<std::endl;
        state=!state;
        c.notify_one();
    }
}

int main()
{
    std::condition_variable c;
    std::mutex m;
    bool state(true);
    int N(500000);
    std::thread th1(thread1, std::ref(c), std::ref(state), std::ref(m), N);
    std::thread th2(thread2, std::ref(c), std::ref(state), std::ref(m), N);
   
    th2.join();
    th1.join();
    return 0;
}