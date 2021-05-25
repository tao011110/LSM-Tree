#include "sstable.h"
#include "MurmurHash3.h"

sstable::sstable(std::vector<memTable::dataNode> &vec)
{
    //std::cout << "vecsize " << std::endl;
    uint64_t size = vec.size();
    header.time = 0;
    header.num = size;
    header.max = vec[size - 1].key;
    header.min = vec[0].key;
    uint32_t hash[4] = {0};
    index = new std::pair<uint64_t, uint32_t>[size];
    data = new std::string[size];
    uint32_t offset = 10272 + size * 12;
    uint32_t lastOffset = 0;
    for(uint64_t i = 0; i < size; i++){
        uint64_t pairKey = vec[i].key;
        std::string pairVal = vec[i].val;
        uint64_t key = pairKey;
        MurmurHash3_x64_128(&key, sizeof(pairKey), 1, hash);
        bf.setVal(hash);
        index[i].first = pairKey;
        offset += lastOffset;
        lastOffset = pairVal.length();
        //std::cout << pairVal<< "  lastOffset " <<lastOffset <<std::endl;
        index[i].second = offset;
        data[i] = pairVal;
    }
}

sstable::~sstable()
{
    delete[] index;
    delete[] data;
}

////使用二分查找在索引中找到对应的offset，若返回-1，则未找到。
//uint32_t sstable::get(uint64_t key)
//{
//    int left = 0, right = header.num, mid = (left + right) / 2;
//    while(left <= right){
//        if(index[mid].first == key){
//            return index[mid].second;
//        }
//        if(index[mid].first < key){
//            left = mid + 1;
//        }
//        else{
//            right = mid - 1;
//        }
//    }
//    return -1;
//}

void sstable::makeFileSST(std::string path)
{
    std::ofstream file(path, std::ios::out|std::ios::binary);
    file.write(reinterpret_cast<char*>(&header), 32);
    file.write(reinterpret_cast<char*>(&bf), 10240);
    for(uint64_t i = 0; i < header.num; i++){
        file.write(reinterpret_cast<char*>(&index[i].first), sizeof(index[i].first));
        file.write(reinterpret_cast<char*>(&index[i].second), sizeof(index[i].second));
    }
    for(uint64_t i = 0; i < header.num; i++){
        file.write(data[i].c_str(), data[i].length());
    }
    file.close();
}

void sstable::reset()
{

}

bool sstable::check(uint64_t key)
{
    uint32_t *hash[4] = {0};
    MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
    bf.setVal(*hash);
    return bf.match(*hash);
}
