#pragma once
#include <iostream>
#include <time.h>
#include <pthread.h>
#include <cstdlib>
#include <chrono>
#include <ratio>
#include <iomanip>

int rnd();
void create_mass(int *mass, int *output_mass, int n, int m, int addit_line, int n_new, int m_new);
void print_full_mass(int n, int m, int *mass);
void print_correct_mass(int n_new, int m_new, int n, int m, int addit_line, int *mass);
void filter_overlay(int *mass, int *output_mass, int n, int m, int q, int k, int threads_cnt);
void *filt_by_cell(void* args);
void* fill_mas(void* args);
