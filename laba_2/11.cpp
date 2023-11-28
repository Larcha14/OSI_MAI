// VAriant 11
#include "./include/function.h"
using namespace std;

int main(int argc, char* argv[]){
    if(argc!=2){
        perror("Incorrect number of arguments");
        exit(-1);
    }
    int cnt_thread=atoi(argv[1]);
    int n,m,q,k;
    cout<<"Input matrix size\n"<<"n=";
    cin>>n;
    cout<<"\nm=";
    cin>>m;
    cout<<"\nInput count of filter overlay=";
    cin>>q;
    cout<<"\nInput filter matrix size k*k (it should be odd number, and k>1)\n";
    cout<<"k=";
    cin>>k;
    while(k%2==0 and k>1){
        cout<<"\nSorry, you should input odd size and >1. Try again.\nk=";
        cin>>k;
    }
    int addit_line=(k/2)*2; //*2!!!!
    int n_new=n+addit_line;
    int m_new=m+addit_line;

    int *mass=new int[n_new*m_new];
    int *output_mass= new int[n_new*m_new];
    // int mass[n_new*m_new];
    // int output_mass[n_new*m_new];
    for(int i=0; i<n_new*m_new;++i){
        mass[i]=0;
        output_mass[i]=0;
    }
    create_mass(mass, output_mass, n,m, addit_line, n_new, m_new);
    cout<<"Begin mass:";
    print_full_mass(n_new, m_new, mass);
    print_correct_mass(n_new, m_new, n, m, addit_line, mass);

    const auto t_begin=chrono::high_resolution_clock::now();
    filter_overlay(mass, output_mass, n, m, q, k, cnt_thread);
    const auto t_ending=chrono::high_resolution_clock::now();
    
    cout<<"\noutput_mass: ";
    print_full_mass(n_new, m_new, output_mass);
    print_correct_mass(n_new, m_new, n, m, addit_line, output_mass);


    chrono::duration<double> t_work=t_ending - t_begin;
    cout<<"\nTime: "<< t_work.count()<<"\n";
}

