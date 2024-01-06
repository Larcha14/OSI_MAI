#pragma once
#include <iostream>
#include <vector>
using namespace std;

struct node{
    int ID;
    node* left_son;
    node* right_son;
    int hight;
    bool available;
};

class AWL_tree{
    public:
        AWL_tree();

        bool is_in_tree(int ID);
        bool is_available(int ID);

        void change_availability(int ID, bool status);
        void bypass(node* cur_node, vector<int> &unavailable); //Bypassing tree and add to vector unavailable nodes
        void bypass_reset(node* cur_node);//make all nodes unavailable

        node* get(int ID);
        node* get_root();
        int parent_id(int child_id); 
        int cnt();
        void draw_tree();
        void balancing();
        bool remove(int ID);
        bool insert(int ID);
        int check_depth();
        


        ~AWL_tree();
    private:
        void add(node* cur_node, node* new_node, int step);
        void draw_node(node* cur_node, int level);
        void rot(node* cur_node);
        int bf(node* cur_node);
        int calHight(node* cur_node);
        int find_parent(node* cur_node, int ID);
        node* find(node* current_node ,int ID);
        int depth;
        node* root;
        int node_cnt;
};