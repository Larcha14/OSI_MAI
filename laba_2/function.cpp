#include "./include/function.h"
#include "./include/threadss.h"
using namespace std;

int rnd(){
    // srand(time(NULL));
    int a =rand()%256;
    // pid_t x = syscall(__NR_gettid);
    // cout<<x<<' '<<a<<'\n';
    return a;
}

void* fill_mas(void* args){
    threads_shell *part = (threads_shell*) args;

    // for(int i=(*part).l;i<=(*part).r;++i){
    //     (*part).mass[i]=rnd();
    // }
    for(int i=0;i<=(*part).coords[0].r-(*part).coords[0].l;++i){
        (*part).mass[i]=rnd();
        (*part).output_mass[i]=(*part).mass[i];
    }
    pthread_exit(0);

}

void create_mass(int *mass, int *output_mass, int n, int m, int addit_line, int n_new, int m_new){
    for(int i=0;i<n;++i){
        srand(time(NULL)+i);
        for(int j=0; j<m;++j){
            // srand(time(NULL)+i);
            mass[(m_new*((addit_line/2)+i))+addit_line/2+j]=rnd();
            output_mass[(m_new*((addit_line/2)+i))+addit_line/2+j]=mass[(m_new*((addit_line/2)+i))+addit_line/2+j];
        }
    }
}

void print_correct_mass(int n_new, int m_new, int n, int m, int addit_line, int *mass){
    cout<<'\n';
    cout<<"Correct mass:\n";
    for(int i=0;i<n;++i){
        for(int j=0; j<m;++j){
            cout<<setw(3)<<mass[(m_new*((addit_line/2)+i))+addit_line/2+j]<<' ';
        }
        cout<<'\n';
    }
}

void print_full_mass(int n, int m, int *mass){
    cout<<'\n';
    cout<<"Full mass:\n";
    for(int i=0; i<n;++i){
        for(int j=0; j<m;++j){
            cout<<setw(3)<<mass[i*m+j]<<' ';
        }
        cout<<'\n';
    }
}

void *filt_by_cell(void* args){
    threads_shell *t_s = (threads_shell*) args;
    // int cur_j=t_s.coords[0].l;
    int addit_line_to2=(*t_s).k/2;
    int n_new=(*t_s).n+2*addit_line_to2;
    int m_new=(*t_s).m+2*addit_line_to2;
    for(int i=0; i<(*t_s).coords.size();++i){
        coord tmp=(*t_s).coords[i];
        for(int j=tmp.l; j<tmp.r;++j){
            // vector<int> tmp_store((*t_s).k*(*t_s).k);
            vector<int> tmp_store;
            int cur_m=j/m_new;
            int cur_n=j-cur_m*(m_new);
            for(int y=cur_m-addit_line_to2; y<=cur_m+addit_line_to2;++y){
                for(int x=cur_n-addit_line_to2; x<=cur_n+addit_line_to2;++x){
                    tmp_store.push_back((*t_s).mass[y*m_new+x]);
                    // tmp_store.push_back(y*m_new+x);
                }
            }
            sort(tmp_store.begin(), tmp_store.end());
            // cout<<(*t_s).mass[j]<<": "<<tmp_store.size()<<' ';
            // for(int z=0; z<tmp_store.size();++z){
            //     cout<<tmp_store[z]<<' ';
            // }
            // cout<<" Midle ="<<tmp_store[tmp_store.size()/2];
            // cout<<'\n';
            (*t_s).output_mass[j]=tmp_store[tmp_store.size()/2];
        }
        // cur_j=tmp.r; !! исправить тут, проверка на примерах k=3 и k=5
        // cout<<" +_+ ";
        
    }
    // cout<<"done\n";
}

// Дописать применение фильтра несколько раз(переприсваивание массивов), + исправить распределение потоков при 1 и 2

void filter_overlay(int *mass, int *output_mass, int n, int m, int q, int k, int threads_cnt){

    int addit_line=(k/2)*2; //*2!!!!
    int n_new=n+addit_line;
    int m_new=m+addit_line;
    
    int work_cell_count=n*m;
    int main_step=work_cell_count/threads_cnt;
    if(main_step<=0){
        main_step=1;
        threads_cnt=work_cell_count;
    }
    int step=main_step;
    pthread_t tid[threads_cnt];
    threads_shell parts[threads_cnt];
    int current_i=m_new*(addit_line/2)+addit_line/2; 
    int thread_numb=0;
    int current_n=addit_line/2;
    int incomp_step=0;
    bool wait_flag=false;
    while(thread_numb<threads_cnt){
        parts[thread_numb].mass=mass;
        parts[thread_numb].output_mass=output_mass;
        parts[thread_numb].k=k;
        parts[thread_numb].m=m;
        parts[thread_numb].n=n;
        if (thread_numb==threads_cnt-1){
            // main_step=(n_new-addit_line/2)*m_new-current_i-(n_new-addit_line/2-1)*addit_line;
            if(main_step==step ){
                step=(n_new-addit_line/2)*m_new-current_i-(n_new-addit_line/2-current_n-1)*addit_line -addit_line/2;
            }
            main_step=(n_new-addit_line/2)*m_new-current_i-(n_new-addit_line/2-current_n-1)*addit_line -addit_line/2;
        }

        coord tmp;
        tmp.l=current_i;
        // cout<<'\n'<<(current_n+1)*m_new-addit_line/2<<'\n';
        if(current_i+step>(current_n+1)*m_new-addit_line/2){
            tmp.r=(current_n+1)*m_new-addit_line/2;
            parts[thread_numb].coords.push_back(tmp);
            step=step-(tmp.r-tmp.l);
            current_n++;
            current_i=(current_n)*m_new+addit_line/2;

        }else if(current_i+step==(current_n+1)*m_new-addit_line/2){
            tmp.r=(current_n+1)*m_new-addit_line/2;
            parts[thread_numb].coords.push_back(tmp);
            current_n++;
            current_i=(current_n)*m_new+addit_line/2;
            thread_numb++;
            step=main_step;

        } else{
            current_i+=step;
            tmp.r=current_i;
            parts[thread_numb].coords.push_back(tmp);
            thread_numb++;
            step=main_step;
        }

    }
    // cout<<'\n';
    // for(int i=0; i<threads_cnt;++i){
    //     cout<<"i= "<<i;
    //     for(int j=0; j<parts[i].coords.size();++j){
    //         cout<<" l= "<<parts[i].coords[j].l<<' '<<" r= "<<parts[i].coords[j].r;
    //     }
    //     cout<<'\n';
    // }
 
    int *tmp_mass= new int[n_new*m_new];
    for(int i=0; i<n_new*m_new;++i){
        tmp_mass[i]=mass[i];
    }
    // cout<<"mass:";
    // print_full_mass(n_new, m_new, mass);
    // cout<<"tmp_mass:";
    // print_full_mass(n_new, m_new, tmp_mass);
    // cout<<"\noutput_mass";
    // print_full_mass(n_new, m_new, output_mass);

    for(int u=0; u<q;++u){
        for(int j=0; j<threads_cnt;++j){
            if(pthread_create(&tid[j], NULL, filt_by_cell, &parts[j])!=0){
                perror("Create thread error ");
            }

        }

        for(int i=0; i<threads_cnt;++i){
            pthread_join(tid[i], NULL);
        }
        // cout<<"tmp_mass:";
        // print_full_mass(n_new, m_new, tmp_mass);
        // cout<<"\noutput_mass";
        // print_full_mass(n_new, m_new, output_mass);

        int *tmp_pointer=tmp_mass;
        tmp_mass=output_mass;
        output_mass=tmp_pointer;
        for(int i=0;i<threads_cnt;++i){
            parts[i].mass=tmp_mass;
            parts[i].output_mass=output_mass;
        }
    }

    // cout<<"tmp_mass:";
    // print_full_mass(n_new, m_new, tmp_mass);
    // cout<<"\noutput_mass";
    // print_full_mass(n_new, m_new, output_mass);

    int *tmp_pointer=tmp_mass;
    tmp_mass=output_mass;
    output_mass=tmp_pointer;
    // delete tmp_mass;
    
}

