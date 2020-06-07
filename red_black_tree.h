#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#include "Exception.h"
#include <iostream>
#include <queue>
#include <stack>

using std::cout;
using std::queue;
using std::stack;

const bool red = false;
const bool black = true;
const int sentinel = 0x3f3f3f3f;

template <class T>
struct rb_node
{ 
    T data;
    int key; //key of a black sentinel is 0x3f3f3f3f
    bool color; 
    rb_node<T> *p_parent;
    rb_node<T> *p_left;
    rb_node<T> *p_right;
    rb_node() 
    {
        color = red; 
        p_parent = p_left = p_right = NULL; 
    } 
};

template <class T>
class rb_tree
{
public:
    rb_tree() {node_num = 0; p_root = NULL;}
    ~rb_tree();
    bool empty() const {return node_num == 0;}
    rb_node<T> * get_root() const {return p_root;}
    void insert(const T &data, int key);
    rb_node<T> * find(int key) const;
    void erase(int key);
    void visit_node(rb_node<T> *p_node) const;
    void level_walk(rb_node<T> *p_node) const;
    rb_node<T> * minimum(rb_node<T> *p_node) const;
    rb_node<T> * maximum(rb_node<T> *p_node) const;
    int height(rb_node<T> *p_node) const;
protected:
    void left_rotate(rb_node<T> *p_node);
    void right_rotate(rb_node<T> *p_node);
    void transplant(rb_node<T> *p_u, rb_node<T> *p_v);
    void rb_insert_fixup(rb_node<T> *p_node);
    void rb_erase_fixup(rb_node<T> *p_node);
protected:
    int node_num;
    rb_node<T> *p_root;
};

template <class T>
rb_tree<T>::~rb_tree()
{ 
    rb_node<T> *p_curr = p_root;
    queue<rb_node<T> *> node_queue;
    if (p_curr != NULL)
        node_queue.push(p_curr);
    while (!node_queue.empty()) {
        p_curr = node_queue.front();
        node_queue.pop();
        if (p_curr->p_left != NULL)
            node_queue.push(p_curr->p_left);
        if (p_curr->p_right != NULL)
            node_queue.push(p_curr->p_right);
        delete p_curr;
    }
}

template <class T>
void rb_tree<T>::visit_node(rb_node<T> *p_node) const
{ 
    if (p_node != NULL) {
        cout << "(data: " << p_node->data << ", key: "
            << p_node->key << ", color: ";
        if (p_node->color == black)
            cout << "black);\n";
        else
            cout << "red);\n";
    }
}

template <class T>
void rb_tree<T>::left_rotate(rb_node<T> *p_node)
{ //for left rotate, p_node should be internal node of red-black tree.
    rb_node<T> *p_y = p_node->p_right;
    if (p_y->key == sentinel)
        throw Exception("invalid calling: right node is sentinel!");
    if (p_node == p_root) {
        p_y->p_parent = NULL;
        p_root = p_y;
    }
    else {
        if (p_node == (p_node->p_parent)->p_left)
            (p_node->p_parent)->p_left = p_y;
        else
            (p_node->p_parent)->p_right = p_y;
        p_y->p_parent = p_node->p_parent;
    }
    p_node->p_right = p_y->p_left;
    (p_y->p_left)->p_parent = p_node;
    p_y->p_left = p_node;
    p_node->p_parent = p_y;
}

template <class T>
void rb_tree<T>::right_rotate(rb_node<T> *p_node)
{ //p_node should be internal node.
    rb_node<T> *p_x = p_node->p_left;
    if (p_x->key == sentinel)
        throw Exception("invalid calling: left node is sentinel!");
    if (p_node == p_root) {
        p_x->p_parent = NULL;
        p_root = p_x;
    }
    else {
        if (p_node == (p_node->p_parent)->p_left)
            (p_node->p_parent)->p_left = p_x;
        else
            (p_node->p_parent)->p_right = p_x;
        p_x->p_parent = p_node->p_parent;
    }
    p_node->p_left = p_x->p_right;
    (p_x->p_right)->p_parent = p_node;
    p_x->p_right = p_node;
    p_node->p_parent = p_x;
}

template <class T>
void rb_tree<T>::transplant(rb_node<T> *p_u, rb_node<T> *p_v)
{ 
    if (p_u == NULL || p_v == NULL)
        throw Exception("invalid argument: null pointer!");
    if (p_u == p_root) {
        if (p_v->key == sentinel) {
            p_root = NULL; //make whole tree be empty
            return;
        }
        p_root = p_v; //the updating if node_num handled by caller
        p_v->p_parent = NULL;
    }
    else {
        rb_node<T> *p_sentinel = new rb_node<T>;
        p_sentinel->key = sentinel;
        p_sentinel->color = black;
        if (p_v == (p_v->p_parent)->p_left)
            (p_v->p_parent)->p_left = p_sentinel;
        else
            (p_v->p_parent)->p_right = p_sentinel;
        p_sentinel->p_parent = p_v->p_parent;
        if (p_u == (p_u->p_parent)->p_left)
            (p_u->p_parent)->p_left = p_v;
        else
            (p_u->p_parent)->p_right = p_v;
        p_v->p_parent = p_u->p_parent; 
        p_u->p_parent = NULL;
    }
}

template <class T>
void rb_tree<T>::rb_insert_fixup(rb_node<T> *p_node)
{
    rb_node<T> *p_curr = p_node; //point to node newly inserted.
    rb_node<T> *p_dad = p_curr->p_parent;
    rb_node<T> *p_grandpa = NULL;
    rb_node<T> *p_uncle = NULL;
    while (p_dad != NULL && p_dad->color == red) {
        p_grandpa = p_dad->p_parent; 
        if (p_dad == p_grandpa->p_left) {
            p_uncle = p_grandpa->p_right;
            if (p_curr == p_dad->p_right) { //do job of pretreatment
                left_rotate(p_dad);
                p_curr = p_dad;
                p_dad = p_curr->p_parent; //reassign pointer p_curr and p_dad
            }
            if (p_uncle->color == red) {
                p_dad->color = black;
                p_uncle->color = black;
                p_grandpa->color = red;
                p_curr = p_grandpa; //if p_curr = p_root, while-loop finishs.
                p_dad = p_curr->p_parent; //updates p_curr and p_dad simultaneouly.
            }
            else {
                p_dad->color = black;
                p_grandpa->color = red;
                right_rotate(p_grandpa); //After this part, the red-black property get fixed up.
            }                
        }
        else {
            p_uncle = p_grandpa->p_left;
            if (p_curr == p_dad->p_left) {
                right_rotate(p_dad);
                p_curr = p_dad;
                p_dad = p_curr->p_parent;
            }
            if (p_uncle->color == red) {
                p_dad->color = black;
                p_uncle->color = black;
                p_grandpa->color = red;
                p_curr = p_grandpa;
                p_dad = p_curr->p_parent;
            }
            else {
                p_dad->color = black;
                p_grandpa->color = red;
                left_rotate(p_grandpa);
            }
        }
    }
    p_root->color = black;
}

template <class T>
void rb_tree<T>::rb_erase_fixup(rb_node<T> *p_node)
{ 
    if (node_num == 0)
        return;
    rb_node<T> *p_x = p_node;
    while (p_x != p_root && p_x->color == black) {
        rb_node<T> *p_w = NULL; 
        if (p_x == (p_x->p_parent)->p_left) {
            p_w = (p_x->p_parent)->p_right; //p_w cannot be sentinel!
            if (p_w->color == red) {
                (p_x->p_parent)->color = red;
                p_w->color = black;
                left_rotate(p_x->p_parent);
                p_w = (p_x->p_parent)->p_right;
            }
            if ((p_w->p_left)->color == black &&
                (p_w->p_right)->color == black) {
                    p_w->color = red;
                    p_x = p_x->p_parent; 
            }     //if color of new p_x is red, while loop finishes                    
            else {
                if ((p_w->p_right)->color == black) {
                    p_w->color = red;
                    (p_w->p_left)->color = black;
                    right_rotate(p_w);
                    p_w = (p_x->p_parent)->p_right;
                }
                p_w->color = (p_x->p_parent)->color;
                (p_x->p_parent)->color = black;
                left_rotate(p_x->p_parent);
                p_x = p_root; //the red-black properties have been fixed up after this step.
            }
        }
        else {
            p_w = (p_x->p_parent)->p_left;
            if (p_w->color == red) {
                (p_x->p_parent)->color = red;
                p_w->color = black;
                right_rotate(p_x->p_parent);
                p_w = (p_x->p_parent)->p_left;
            }
            if ((p_w->p_left)->color == black &&
                (p_w->p_right)->color == black) {
                p_w->color = red;
                p_x = p_x->p_parent;
            }
            else {
                if ((p_w->p_left)->color == black) {
                    p_w->color = red;
                    (p_w->p_right)->color = black;
                    left_rotate(p_w);
                    p_w = (p_x->p_parent)->p_left;
                }
                p_w->color = (p_x->p_parent)->color;
                (p_x->p_parent)->color = black;
                right_rotate(p_x->p_parent);
                p_x = p_root;
            }
        }
    }
    p_x->color = black;
}

template <class T>
void rb_tree<T>::insert(const T &data, int key)
{ 
    rb_node<T> *p_node = new rb_node<T>;
    p_node->data = data; p_node->key = key;
    p_node->p_left = new rb_node<T>;
    (p_node->p_left)->p_parent = p_node;
    (p_node->p_left)->key = sentinel;
    (p_node->p_left)->color = black;
    p_node->p_right = new rb_node<T>;
    (p_node->p_right)->p_parent = p_node;
    (p_node->p_right)->key = sentinel;
    (p_node->p_right)->color = black;
    if (node_num == 0) { //treatment of empty tree.
        p_root = p_node;
        p_root->color = black;
        node_num = 1;
        return;
    }
    rb_node<T> *p_curr = p_root;
    rb_node<T> *p_prev = NULL;
    while (p_curr->key != sentinel) {
        p_prev = p_curr;
        if (key <= p_curr->key) 
            p_curr = p_curr->p_left;
        else
            p_curr = p_curr->p_right;
    }
    delete p_curr; p_curr = NULL;
    if (key <= p_prev->key)
        p_prev->p_left = p_node;
    else 
        p_prev->p_right = p_node;
    p_node->p_parent = p_prev;
    ++node_num;
    rb_insert_fixup(p_node); //the red-black properties get broken at site of new inserted node
}

template <class T>
rb_node<T> * rb_tree<T>::find(int key) const
{ 
    if (node_num == 0)
        return NULL;
    rb_node<T> *p_curr = p_root;
    while (p_curr->key != sentinel && p_curr->key != key) {
        if (key < p_curr->key)
            p_curr = p_curr->p_left;
        if (key > p_curr->key)
            p_curr = p_curr->p_right;
    }
    if (key == p_curr->key)
        return p_curr;
    else {
        cout << "no nodes found in tree!\n";
        return NULL;
    }
}

template <class T>
void rb_tree<T>::erase(int key)
{ 
    if (node_num == 0)
        throw Exception("invalid calling: empty tree!");
    rb_node<T> *p_y = find(key);
    if (p_y == NULL)
        return;   
    bool y_original_color = p_y->color;
    rb_node<T> *p_x = NULL; 
    if ((p_y->p_left)->key == sentinel) { 
        p_x = p_y->p_right; //p_x could be sentinel.
        transplant(p_y, p_x);
        delete p_y->p_left; p_y->p_left = NULL;
        delete p_y; p_y = NULL;
    }
    else if ((p_y->p_right)->key == sentinel) {
        p_x = p_y->p_left; 
        transplant(p_y, p_x);
        delete p_y->p_right; p_y->p_right = NULL;
        delete p_y; p_y = NULL;
    }
    else {
        rb_node<T> *p_node = p_y;
        p_y = minimum(p_node->p_right);
        y_original_color = p_y->color;
        p_x = p_y->p_right; //p_x could be sentinel.
        if (p_y->p_parent != p_node) {
            transplant(p_y, p_x);
            p_y->p_right = p_node->p_right;
            (p_node->p_right)->p_parent = p_y;
        }
        transplant(p_node, p_y);
        p_y->color = p_node->color;
        delete p_y->p_left; p_y->p_left = NULL;
        p_y->p_left = p_node->p_left;
        (p_node->p_left)->p_parent = p_y;
        delete p_node; p_node = NULL;
    }
    --node_num;
    if (y_original_color == black)
        rb_erase_fixup(p_x); //repair the red-black properties from p_x node
}

template <class T>
void rb_tree<T>::level_walk(rb_node<T> *p_node) const
{ 
    rb_node<T> *p_curr = p_node;
    queue<rb_node<T> *> node_queue;
    if (p_curr != NULL && p_curr->key != sentinel) {
        node_queue.push(p_curr);
        cout << "level walk of red-black tree:\n";
    }
    while (!node_queue.empty()) {
        p_curr = node_queue.front();
        node_queue.pop();
        visit_node(p_curr);
        if (p_curr->p_left != NULL && (p_curr->p_left)->key != sentinel)
            node_queue.push(p_curr->p_left);
        if (p_curr->p_right != NULL && (p_curr->p_right)->key != sentinel)
            node_queue.push(p_curr->p_right);
    }
    cout << "\n";
}

template <class T>
rb_node<T> * rb_tree<T>::minimum(rb_node<T> *p_node) const
{ 
    if (p_node == NULL || p_node->key == sentinel)
        throw Exception("invalid calling: empty tree!");
    rb_node<T> *p_curr = p_node;
    rb_node<T> *p_prev = NULL;
    while (p_curr->key != sentinel) {
        p_prev = p_curr;
        p_curr = p_curr->p_left;
    }
    return p_prev;
}

template <class T>
rb_node<T> * rb_tree<T>::maximum(rb_node<T> *p_node) const
{ 
    if (p_node == NULL || p_node->key == sentinel)
        throw Exception("invalid calling: empty tree!");
    rb_node<T> *p_curr = p_node;
    rb_node<T> *p_prev = NULL;
    while (p_curr->key != sentinel) {
        p_prev = p_curr;
        p_curr = p_curr->p_right;
    }
    return p_prev;
}

template <class T>
int rb_tree<T>::height(rb_node<T> *p_node) const
{ //some bugs in code.
    //following codes are recursive version
    if (p_node == NULL)
        throw Exception("invalid calling: null pointer!");
    if ((p_node->p_left)->key == sentinel &&
        (p_node->p_right)->key == sentinel) 
            return 0; //internal leaf of tree
    int left_height = -1, right_height = -1;
    if ((p_node->p_left)->key != sentinel)
        left_height = height(p_node->p_left);
    if ((p_node->p_right)->key != sentinel)
        right_height = height(p_node->p_right);
    if (left_height > right_height)
        return left_height + 1;
    else
        return right_height + 1;
    /*if (p_node == NULL || p_node->key == sentinel)
        throw Exception("invalid situation!");
    int max_deepth = -1, curr_deepth = 0;
    rb_node<T> *p_iter = p_node;
    rb_node<T> *p_last_visited = NULL;
    rb_node<T> *p_top = NULL;
    stack<rb_node<T> *> node_stack;
    node_stack.push(p_iter);
    while () {
        if ((p_iter->p_left)->key != sentinel && 
            (p_iter->p_left) != p_last_visited) {
            node_stack.push(p_iter);
            p_iter = p_iter->p_left;
            ++curr_deepth;
            if (curr_deepth > max_deepth)
                max_deepth = curr_deepth;
        }
        else if ((p_iter->p_right)->key != sentinel &&
            (p_iter->p_right) != p_last_visited) {
            if (p_iter != p_top)
                node_stack.push(p_iter);
            p_iter = p_iter->p_right;
            ++curr_deepth;
            if (curr_deepth > max_deepth)
                max_deepth = curr_deepth;
        }
        else {
            p_last_visited = p_iter;
            p_top = node_stack.top();
            node_stack.pop();
            --curr_deepth;
            p_iter = p_top;
        }
    }*/
}

#endif