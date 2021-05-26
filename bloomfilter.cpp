#include "bloomfilter.h"

BloomFilter::BloomFilter()
{
    memset(set, 0, sizeof(set));
}

bool BloomFilter::match(uint32_t hashVal[])
{
    bool flag = true;
    for(int i = 0; i < 4; i++){
        uint32_t val = hashVal[i];
        uint32_t tmpval = val % 81920;
        uint32_t index = tmpval / 32;
        uint32_t offset = tmpval % 32;
        uint32_t mask = ((uint32_t)1) << offset;
        flag = flag && (mask == (set[index] & mask));
    }

    return flag;
}

void BloomFilter::setVal(uint32_t hashVal[])
{
    for(int i = 0; i < 4; i++){
        uint32_t val = hashVal[i];
        uint32_t tmpval = val % 81920;
        uint32_t index = tmpval / 32;
        uint32_t offset = tmpval % 32;
        uint32_t num = ((uint32_t)1) << offset;
        set[index] = num | set[index];
    }
}
