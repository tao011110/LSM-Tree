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
//    if(mt.getByteSize() +  s.length() + 12 > 2086880){
//        std::vector<std::pair<uint64_t, std::string>> vec = mt.writeBack();
//        mt.reset();
//        sstable st(vec);
//        st.setTime(time);
//        time++;
//        currentNum++;
//        std::string path = "data/level-";
//        if(isLevel0 == true){
//            isLevel0 = false;
//            path += std::to_string(currentLevel);
//            const char *newDir = path.data();
//            utils::mkdir(newDir);
//        }
//        else{
//            if(currentNum > pow(2, currentLevel + 1)){
//                currentNum = 1;
//                std::vector<std::string> ret;
//                std::string m = path+std::to_string(currentLevel);
//                const char *newD = m.data();
//                std::cout << utils::scanDir(newD, ret) <<std::endl;
//                currentLevel++;
//                path += std::to_string(currentLevel);
//                const char *newDir = path.data();
//                utils::mkdir(newDir);
//            }
//            else{
//                path += std::to_string(currentLevel);
//            }
//        }
//        path += ("/" + std::to_string(currentNum) + ".sst");
//        //std::cout << "path is:  " << path << std::endl;
//        st.makeFileSST(path);
//        cache.add(path, st);
//    }
    //有问题啊，应该先写进去，等超出大小了再合并
        if(mt.getByteSize() +  s.length() + 12 > 2086880){
            std::vector<std::pair<uint64_t, std::string>> vec = mt.writeBack();
            mt.reset();
            sstable st(vec);
            st.setTime(time);
            time++;
            currentLevel = 0;
            std::string path = "data/level-";
            std::string tmppath = path + std::to_string(currentLevel);

            while(true){
                if(utils::dirExists(tmppath)){
                    std::vector<std::string> ret;
                    int size = 0;
                    if((size = utils::scanDir(tmppath, ret)) == pow(2, currentLevel + 1)){

                        currentNum = 1;
                        currentLevel++;
                        tmppath = path + std::to_string(currentLevel);
                        //std::cout << "currentLevel  " << tmppath << std::endl;
                    }
                    else{
                        currentNum = size + 1;
                        break;
                    }
                }
                else{
                    const char *newDir = tmppath.data();
                    //std::cout << "make  " << tmppath << std::endl;
                    utils::mkdir(newDir);
                    currentNum = 1;
                    break;
                }
            }

            tmppath += ("/" + std::to_string(currentNum) + ".sst");
            //std::cout << "path is:  " << tmppath << std::endl;
            st.makeFileSST(tmppath);
            cache.add(tmppath, st);
        }
    mt.put(key, s);
}


//首先，读出所需sstable的index，有相交？两边按照迭代合并index；然后，得到header，再有bf，最后有data；最后放入
void KVStore::compaction(std::vector<std::string> vec, int currentLevel)
{
    std::vector<Cache::Node*> curvec, nextvec, comp, remain;
    std::vector<std::pair<uint64_t, std::string>> result;
    if(currentLevel == 0){
        std::pair<int, int> range = cache.compLevel0(curvec);
        nextvec = cache.compNextLevel(0);
        uint64_t min = range.first;
        uint64_t max = range.second;
        for(int i = 0; i < (int)nextvec.size(); i++){
            if((nextvec[i]->min < max && nextvec[i]->min > min)
                    || (nextvec[i]->max < max && nextvec[i]->max > min)){
                comp.push_back(nextvec[i]);
            }
            else{
                remain.push_back(nextvec[i]);                   //这样能按照存放在nextvec里时间戳从大到小的顺序
            }
        }
        std::vector<std::pair<std::pair<uint64_t, std::string>, uint64_t>> cur;
        for(int i = 0; i < (int)curvec.size(); i++){
            uint64_t curtime = curvec[i]->time;
            for(uint64_t j = 0; j < curvec[i]->num; j++){
                std::pair<uint64_t, std::string> p;
                p.first = curvec[i]->index[j].first;
                p.second = get(p.first);                //此处要优化！改成从这个node里面找就好了！
                cur.push_back(std::pair<std::pair<uint64_t, std::string>, uint64_t>(p, curtime));
            }
        }
        sort(cur.begin(), cur.end());
        uint64_t k = 0;
        for(uint64_t i = 0; i < cur.size(); i++){
            std::string val1 = cur[i].first.second;
            while(i < cur.size() && cur[i].first.first == cur[i+1].first.first){
                if(cur[i].second < cur[i + 1].second){
                    val1 = cur[i+1].first.second;
                }
                i++;
            }
            for(uint64_t j = 0; j < comp[k]->num; j++){
                if(comp[k]->index[j].first < min){
                    continue;
                }
                else{
                    if(comp[k]->index[j].first > max){
                        k++;
                        break;
                    }
                    else{
                        if(curvec[i]->index[i].first <= comp[k]->index[j].first){
                            result.push_back(cur[i].first);
                        }
                        else{

                        }
                    }
                }
            }
            if(k == comp.size()){
                break;
            }
        }
    }

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
