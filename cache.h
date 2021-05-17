#ifndef CACHE_H
#define CACHE_H
#include <string>
#include <fstream>
#include "bloomfilter.h"
#include "sstable.h"

class Cache
{
private:
    struct Node{
        std::string path;
        uint64_t time;
        uint64_t num;
        uint64_t max;
        uint64_t min;
        BloomFilter bf;
        std::pair<uint64_t, uint32_t> *index;
        Node *next;
        Node(){}
        Node(std::string &p, sstable &st);
        uint64_t getOffsetIndex(uint64_t key, bool &flag);
    };
    Node *head;
public:
    Cache();
    std::string get(uint64_t key);
    bool del(uint64_t key);
    void add(std::string &p, sstable &st);
    void reset();
};

#endif // CACHE_H
