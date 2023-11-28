#pragma once
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <vector>
#include <algorithm>
using namespace std;

// pthread_mutex_t mutex;

typedef struct coord{
    int l;
    int r;
}coord;

struct threads_shell{
    int *mass;
    vector<coord> coords;
    int *output_mass;
    int k;
    int n;
    int m;
};

// struct threads_shell{
//     int *mass;
//     int l;
//     int r;
// };