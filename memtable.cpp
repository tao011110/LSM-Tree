#include "memtable.h"

memTable::memTable()
{
    head = new Node;
    tail = new Node;
    head->right = tail;
    head->down = nullptr;
    head->up = nullptr;
    tail->right = nullptr;
    tail->down = nullptr;
    tail->up = nullptr;
    tophead = head;
}

memTable::~memTable()
{
    //writeBack();
    reset();
}

void memTable::put(uint64_t key, const std::string &s)
{
    byteSize += (s.length() + 12);
    //std::cout << byteSize << std::endl;
    Node *q = new Node;
    if(empty()){
        q->right = head->right;
        head->right = q;
        q->down = nullptr;
        q->up = nullptr;
        q->key = key;
        q->value = s;
    }
    else{
        Node *p = tophead;
        while(p){
            while(p->right && p->right->key <= key){
                p = p->right;
            }
            if(p->key == key){
                while(p != nullptr){
                    p->value = s;
                    p = p->down;
                }
                break;
            }
            if(p->down == nullptr || p->right->key > key){
                break;
            }
            p = p->down;
        }
        if(p != nullptr){
            q->right = p->right;
            p->right = q;
            q->down = nullptr;
            q->up = nullptr;
            q->key = key;
            q->value = s;
        }
    }
    size++;
    Node * h = head;
    Node * t = tail;

    while(rand() % 1){
        Node *tmp = new Node;
        tmp->key = q->key;
        tmp->value = q->value;
        tmp->down = q;
        q->up = tmp;
        tmp->up = nullptr;
        if(h->up == nullptr){
            Node * headtower = new Node;
            headtower->down = h;
            headtower->up = nullptr;
            h->up = headtower;
            headtower->right = tmp;
            Node * tailtower = new Node;
            tailtower->down = t;
            tailtower->up = nullptr;
            t->up = tailtower;
            tmp->right = tailtower;
            tophead = headtower;
        }
        else{
            Node *p = h->up;
            while(p->right != t->up && p->right->key < tmp->key){
                p = p->right;
            }
            tmp->right = p->right;
            p->right = tmp;
        }
        h = h->up;
        t = t->up;
    }
}

memTable::Node* memTable::search(uint64_t key)
{
    Node *p = tophead;
    while(p){
        while(p->right && p->right->key <= key){
            p = p->right;
        }
        if(p->key == key){
            break;
        }
        p = p->down;
    }
    return p;
}

std::string memTable::get(uint64_t key, bool &isDelete)
{
    std::string val = "";
    if(empty()){
        return val;
    }
    Node* p = search(key);
    if(p != nullptr){
        if(p->value != "~DELETED~"){
            val = p->value;
        }
        else{
            isDelete = true;
        }
    }

    return val;
}

bool memTable::del(uint64_t key, bool &isDeleted)
{
    Node *p = search(key);
    std::string d = "~DELETED~";
    if(p != nullptr){
        if(p->value != d){
            while(p != nullptr){
                p->value = d;
                p = p->down;
            }
        }
        else{
            isDeleted = true;
        }
        return true;
    }
    else{
        put(key, d);
        return false;
    }
}

void memTable::reset()
{
    size = 0;
    Node *tmp = head;
    while(tmp != nullptr){
        Node *delp = tmp->up;
        while(delp != nullptr){
            Node *delq =delp->up;
            delete delp;
            delp = delq;
        }
        delp = tmp->right;
        delete tmp;
        tmp = delp;
    }
    head = new Node;
    tail = new Node;
    head->right = tail;
    head->down = nullptr;
    head->up = nullptr;
    tail->right = nullptr;
    tail->down = nullptr;
    tail->up = nullptr;
    tophead = head;
    byteSize = 0;
    size = 0;
}

std::vector<std::pair<uint64_t, std::string>> memTable::writeBack()
{
    //从head开始遍历，将所有数据写入sstable
    std::vector<std::pair<uint64_t, std::string>> vec;
    Node *p = head->right;
    while(p != tail && p != nullptr){
        vec.push_back(std::pair<uint64_t, std::string>(p->key, p->value));
        p = p->right;
    }

    return vec;
}
