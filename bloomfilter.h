#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <cstdint>
#include <string.h>
#include <iostream>
#include <fstream>

class BloomFilter
{
private:
    uint32_t set[2560];     //Because the Bloom Filter is 10280 bytes, the size of the set is 10280 / 4 = 2560

public:
    BloomFilter();
    bool match(uint32_t hashVal[]);
    void setVal(uint32_t hashVal[]);
    void getBf(BloomFilter *newbf){
        for(int i = 0; i < 2560; i++){
            newbf->set[i] = set[i];
        }
    }
    uint32_t getSet(int i){
        return set[i];
    }
    void getFromFile(std::ifstream &file);
};

#endif // BLOOMFILTER_H
