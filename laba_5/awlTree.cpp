#include "awlTree.h"


AWL_tree::AWL_tree(){
    // node* new_node= new node;
    // new_node->ID=0;
    node_cnt=0;
    depth=0;
    root=NULL;
}

int AWL_tree::check_depth(){
    return depth;
}

int AWL_tree::cnt(){
    return node_cnt;
}

node* AWL_tree::find(node* current_node,int ID){
    if(current_node==NULL){
        return NULL;
    } 
    if(current_node->ID==ID){
        return current_node;
    } else if(ID>current_node->ID){
        return find(current_node->right_son, ID);
    }
    return find(current_node->left_son, ID);
}

bool AWL_tree::is_in_tree(int ID){
    node* cur_node=find(root, ID);
    if(cur_node==NULL){
        return false;
    }
    return true;
}

void AWL_tree::add(node* cur_node, node* new_node, int step){
    if(new_node->ID>cur_node->ID){
        if(cur_node->right_son==NULL){
            cur_node->right_son=new_node;
            new_node->level=step+1;
            return;
        } else{
            add(cur_node->right_son, new_node, step+1);
        }
    } else {
        if(cur_node->left_son==NULL){
            cur_node->left_son=new_node;
            new_node->level=step+1;
            return;
        } else{
            add(cur_node->left_son, new_node, step+1);
        }
    }
}

void AWL_tree::balancing(){

}

bool AWL_tree::insert(int ID){
    if(is_in_tree(ID)){
        return false;
    }
    node* new_node= new node;
    new_node->left_son=NULL;
    new_node->right_son=NULL;
    new_node->ID=ID;
    if (node_cnt==0){
        root=new_node;
        root->level=1;
        depth=1;
    } else{
        add(root, new_node, 1);
        if(depth < new_node->level){
            depth=new_node->level;
        }
    }

    node_cnt++;
    // balancing();
    return true;
}

node* AWL_tree::get(int ID){
    node* find_node=find(root, ID);
    return find_node;
}

node* AWL_tree::get_root(){
    return root;
}

void AWL_tree::draw_node(node* cur_node, int level){
    if (cur_node==NULL){
        return;
    }
    draw_node(cur_node->right_son, level+1);
    for(int i=0; i<level; ++i){
        cout<<"|===";
    }
    cout<<cur_node->ID<<'\n';
    draw_node(cur_node->left_son, level+1);

}

void AWL_tree::draw_tree(){
    draw_node(root, 0);
}

int AWL_tree::find_parent(node* cur_node, int child_id){
    if(cur_node->ID<child_id){
        if(cur_node->right_son!=NULL){
            if(cur_node->right_son->ID==child_id){
                return cur_node->ID;
            } else{
                return find_parent(cur_node->right_son, child_id);
            }
        }
    } else if(cur_node->ID>child_id){
        if(cur_node->left_son!=NULL){
            if(cur_node->left_son->ID==child_id){
                return cur_node->ID;
            } else{
                return find_parent(cur_node->left_son, child_id);
            }
        }
    } 
    return -1;

}

int AWL_tree::parent_id(int child_id){
    if(!is_in_tree(child_id)){
        return -1;
    }
    return find_parent(root, child_id);

}

bool AWL_tree::is_available(int ID){
    node* cur_node=find(root, ID);
    if(cur_node==NULL){
        return false;
    } 
    return cur_node->available;
}

void AWL_tree::change_availability(int ID, bool status){
    node* cur_node=find(root, ID);
    if(cur_node!=NULL){
        cur_node->available=status;
    }
}

void AWL_tree::bypass(node* cur_node, vector<int> &unavailable){
    if(!cur_node->available){
        unavailable.push_back(cur_node->ID);
    }
    if(cur_node->left_son!=NULL){
        bypass(cur_node->left_son, unavailable);
    }
    if(cur_node->right_son!=NULL){
        bypass(cur_node->right_son, unavailable);
    }
}

void AWL_tree::bypass_reset(node* cur_node){
    cur_node->available=false;
    if(cur_node->left_son!=NULL){
        bypass_reset(cur_node->left_son);
    }
    if(cur_node->right_son!=NULL){
        bypass_reset(cur_node->right_son);
    }
}

AWL_tree::~AWL_tree(){

}


int main(){
    AWL_tree tree;
    vector<int> data;
    tree.insert(2);
    tree.insert(7);
    tree.insert(1);
    tree.insert(10);
    tree.insert(4);
    tree.change_availability(2, true);
    tree.bypass(tree.get_root(), data);
    node* n=tree.get(4);
    cout<<"hui\n";
    tree.draw_tree();
    cout<<'\n';
    cout<<tree.parent_id(2);
    cout<<"\n"<<tree.check_depth();
    cout<<"hui hui\n";
    cout<<"Size = "<<data.size()<<" Tree cnt = "<<tree.cnt();
}