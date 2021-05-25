#ifndef CACHE_H
#define CACHE_H
#include <string>
#include <fstream>
#include <cmath>
#include "bloomfilter.h"
#include "sstable.h"

class Cache
{
public:
    struct Node{
        std::string path;
        uint64_t time;
        uint64_t num;
        uint64_t max;
        uint64_t min;
        BloomFilter bf;
        std::pair<uint64_t, uint32_t> *index;       //索引区，用来存储有序的索引数据，包含所有的键及对应的值在文件中的 offset (无符号 32 位整型)。
        Node *next;
        Node(){
            index = new std::pair<uint64_t, uint32_t>;
            next = nullptr;
        }
        Node(std::string &p, sstable &st);
        ~Node();
        uint64_t getOffsetIndex(uint64_t key, bool &flag);
        std::string nodeGet(uint64_t key,  bool &flag);
    };
    //Node *head;
    Cache();
    ~Cache();
    std::string get(uint64_t key);
    bool del(uint64_t key);
    void add(std::string &p, sstable &st, int level);
    void reset();
    std::pair<int, int> compaction(int currentLevel, std::vector<Node*> &curvec, std::vector<Node*> &nextvec);
    std::pair<int, int> findRange(std::vector<Node*> &);
    std::pair<int, int> compLevel0(std::vector<Node*> &curvec);
    std::pair<int, int> compCurrentLevel(std::vector<Node*> &curvec, std::vector<std::string> &find, int level);
    std::vector<Cache::Node*> compNextLevel(int currentLevel);
    void addNode(Node *addition, int level);
//    void resetPrev(){
//        cachePrev = head;
//    }

private:
//    Node *cachePrev;
//    Node *cacheNext;
    std::vector<Node*> levelVec;
};

#endif // CACHE_H
