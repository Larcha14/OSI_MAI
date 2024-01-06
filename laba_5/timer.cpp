#include "timer.h"
// #include<unistd.h>

Timer::Timer() {
    t_work=false;
}

Timer::~Timer(){

}


int64_t Timer::times(){
    if(t_work){
        return chrono::duration_cast<chrono::milliseconds>(Clock::now()-begin).count();

    } else{
        return oper_time.count();
    } 
}

void Timer::start(){
    t_work=true;
    begin=Clock::now();
}

void Timer::stop(){
    if(t_work){
        t_work=false;
        oper_time=chrono::duration_cast<chrono::milliseconds>(Clock::now()-begin);
    }
}

// int main(){
//     Timer t;
//     cout<<"Time: "<<t.times()<<'\n';
//     t.start();
//     sleep(2);
//     cout<<"Time: "<<t.times()<<'\n';
//     t.stop();
//     sleep(2);
//     cout<<"Time: "<<t.times()<<'\n';


// }
