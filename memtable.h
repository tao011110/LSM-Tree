#ifndef MEMTABLE_H
#define MEMTABLE_H
#include <cstdint>
#include <string>
#include <iostream>
#include <vector>

class memTable
{
private:
    struct Node{
        uint64_t key;
        std::string value;
        Node *right, *down, *up;
        Node(Node* R, Node *D, Node *U, uint64_t k, std::string v){
            right = R;
            down = D;
            up = U;
            key = k;
            value = v;
        }
        Node(): key(-1), right(nullptr), down(nullptr), up(nullptr), value("") {}
        ~Node(){}
    };
    Node *head, *tail, *tophead;

    int size = 0;
    int byteSize = 0;

public:
    memTable();
    ~memTable();
    void put(uint64_t key, const std::string &s);
    Node* search(uint64_t key);
    std::string get(uint64_t key, bool &isDelete);
    bool del(uint64_t key, bool &isDeleted);
    void reset();
    bool empty(){
        if(size == 0){
            return true;
        }
        else return false;
    }
    int getByteSize(){
        return byteSize;
    }

    struct dataNode{
        uint64_t key;
        std::string val;
        dataNode(){}
    };

    std::vector<dataNode> writeBack();
};

#endif // MEMTABLE_H
