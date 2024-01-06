#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;
 
enum Subcommands{
    timer,
    start,
    stop
};

enum Message_type{
    create,
    create_asw,
    exec,
    exec_asw,
    heartbit,
    heartbit_answ,
    error,
    die
};

struct Message{
    Message_type type;
    vector<int> data;
};