#include "kvstore.h"
#include "utils.h"
#include <string>

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
    currentLevel = 0;
    currentNum = 0;
}
KVStore::~KVStore()
{
    //std::vector<std::pair<uint64_t, std::string>> vec = mt.writeBack();
    //mt.~memTable();
    //然后写回sstable

}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
    //sstable大小限定为2MB，减去时间戳和Bloom Filter剩下2086880B，超过这个大小，则先转化为sstable
    if(mt.getByteSize() +  sizeof(s) + 12 > 2086880){
        std::vector<std::pair<uint64_t, std::string>> vec = mt.writeBack();
        mt.reset();
        sstable st(vec);
        st.setTime(time);
        time++;
        currentNum++;
        std::string path = "data/level-";
        //本函数中的level文件夹划分有误,!!!
        if(currentNum > pow(2, currentLevel + 1)){
            currentNum = 1;
            currentLevel++;
            path += std::to_string(currentLevel);
            const char *newDir = path.data();
            utils::mkdir(newDir);
        }
        path += ("/" + std::to_string(currentNum) + ".sst");

        st.makeFileSST(path);
        cache.add(path, st);
    }
    mt.put(key, s);
}

void KVStore::make()
{
    std::vector<std::pair<uint64_t, std::string>> vec = mt.writeBack();
    mt.reset();
    sstable st(vec);
    st.setTime(time);
    time++;
    std::string path = "data/level-0/1.sst";
    st.makeFileSST(path);

    cache.add(path, st);
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
    std::string result = "";
    bool isDelete = false;
    result = mt.get(key, isDelete);
    if(isDelete == true){
        return "";
    }
    else{
        if(result == ""){           //如果memtable中未找到，则到sstable中进行寻找
            //std::cout << "search in sstable" << std::endl;
            result = cache.get(key);
        }
    }

    return result;
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
    bool isDeleted = false;
    bool flag = mt.del(key, isDeleted);
    if(isDeleted == true){
        return false;
    }
    if(flag == false && isDeleted == false){
        flag = cache.del(key);
    }

    return flag;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
    mt.~memTable();
    int num = currentNum;
    int level = currentLevel;
    while(level >= 0){
        int nextNum = num / 2;
        std::string path = "data/level-";
        path += std::to_string(level);
        const char *newDir = path.data();
        while(num > 0){
            num--;
            std::string newPath = path;
            newPath + ("/" + std::to_string(currentNum) + ".sst");
            const char *delPath = newPath.data();
            utils::rmfile(delPath);
        }
        utils::rmdir(newDir);
        num = nextNum;
        level--;
    }
    cache.reset();
    currentLevel = 0;
    currentNum = 0;
}

//首先，读出所需sstable的index，有相交？两边按照迭代合并index；然后，得到header，再有bf，最后有data；最后放入
void KVStore::compaction()
{

}
