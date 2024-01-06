#pragma once
#include <iostream>
#include <chrono>

using namespace std;

using t_t = chrono::time_point<std::chrono::system_clock>;
using Clock = chrono::high_resolution_clock;
using ms= chrono::milliseconds; // can use int64_t


class Timer{
    public:
        Timer();

        int64_t times();
        void start();
        void stop();

        ~Timer();

    private:
        bool t_work;
        t_t begin, end;
        ms oper_time{0};
};