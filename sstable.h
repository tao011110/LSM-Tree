#ifndef SSTABLE_H
#define SSTABLE_H
#include <string>
#include <vector>
#include <fstream>
#include "bloomfilter.h"

class sstable
{
private:
    std::pair<uint64_t, uint32_t> *index = new std::pair<uint64_t, uint32_t>[header.num];
    std::string *data = new std::string[header.num];
    struct Header{
        uint64_t time;
        uint64_t num;
        uint64_t max;
        uint64_t min;
    };

public:
    sstable(std::vector<std::pair<uint64_t, std::string>>);
    uint32_t get(uint64_t key);
    void reset();
    bool check(uint64_t key);
    void setTime(uint64_t time){
        header.time = time;
    }
    void setNum(uint64_t num){
        header.num = num;
    }
    void setMax(uint64_t max){
        header.max = max;
    }
    void setMin(uint64_t min){
        header.min = min;
    }
    uint64_t getTime(){
        return header.time;
    }
    uint64_t getNum(){
        return header.num;
    }
    uint64_t getMax(){
        return header.max;
    }
    uint64_t getMin(){
        return header.min;
    }
    void makeFileSST(std::string path);
    void getIndex(std::pair<uint64_t, uint32_t>*newindex){
        for(uint64_t i = 0; i < header.num; i++){
            newindex[i].first = index[i].first;
            newindex[i].second = index[i].second;
        }
    }
    void getBf(BloomFilter *newbf){
        bf.getBf(newbf);
    }
    Header header;
    BloomFilter bf;
};

#endif // SSTABLE_H
