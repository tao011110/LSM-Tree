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
    mt.~memTable();
    cache.~Cache();
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
    if(mt.getByteSize() +  s.length() + 12 > 2086880){
        std::vector<memTable::dataNode> vec = mt.writeBack();
        currentLevel = 0;
        makeSST(vec);
        std::vector<memTable::dataNode>().swap(vec);
        mt.reset();

        checkCompaction();
    }

    mt.put(key, s);
}

void KVStore::checkCompaction()
{
    std::string path = "data/level-";
    int i = 0;
    std::string tmppath = path + std::to_string(i);
    while(utils::dirExists(tmppath)){
        std::vector<std::string> ret;
        int size = utils::scanDir(tmppath, ret);
        if(size > pow(2, i + 1)){
            sort(ret.begin(), ret.end());
            currentLevel = i;
            compaction(ret);
        }
        i++;
        tmppath = path + std::to_string(i);
        std::vector<std::string>().swap(ret);
    }
}

void KVStore::makeSST(std::vector<memTable::dataNode> &vec)
{
    sstable st(vec);
    vec.clear();
    std::vector<memTable::dataNode>().swap(vec);
    if(isCompaction == false) {
        st.setTime(time);
    }
    else{
        st.setTime(compactionTime);
    };
    time++;
    std::string path = "data/level-";

    std::string tmppath = path + std::to_string(currentLevel);
    currentNum++;

    if(utils::dirExists(tmppath)){
        tmppath += ("/" + std::to_string(currentNum) + ".sst");
        st.makeFileSST(tmppath);
        cache.add(tmppath, st, currentLevel);
    }
    else{
        const char *newDir = tmppath.data();
        utils::mkdir(newDir);
        tmppath += ("/" + std::to_string(currentNum) + ".sst");
        st.makeFileSST(tmppath);
        cache.add(tmppath, st, currentLevel);
    }
}


//首先，读出所需sstable的index，有相交？两边按照迭代合并index；然后，得到header，再有bf，最后有data；最后放入
void KVStore::compaction(std::vector<std::string> &v)
{
    isCompaction = true;
    std::vector<Cache::Node*> curvec, nextvec, comp, curremain, remain;
    std::pair<int, int> range;
    std::string path = "data/level-" + std::to_string(currentLevel);

    uint64_t vsize = v.size();
    uint64_t curSize = pow(2, currentLevel + 1);
    std::vector<std::string> find;
    if(currentLevel == 0){
        for(uint64_t i = 0; i < vsize; i++){
            std::string tmppath = path + "/" + v[i];
            find.push_back(tmppath);
        }
    }
    else{
        for(uint64_t i = 0; vsize - i > curSize; i++){
            std::string tmppath = path + "/" + v[i];
            find.push_back(tmppath);
        }
    }
    std::vector<Cache::Node*> curtmp = cache.compNextLevel(currentLevel);
    uint64_t curtmp_size = curtmp.size();
    for(uint64_t i = 0; i < curtmp_size; i++){
        uint64_t j = 0;
        uint64_t find_size = find.size();
        for(j = 0; j < find_size; j++){
            if(curtmp[i]->path == find[j]){
                curvec.push_back(curtmp[i]);
                find.erase(find.begin() + j);
                if(curtmp[i]->time > compactionTime){
                    compactionTime = curtmp[i]->time;
                }
                break;
            }
        }
        if(j == find_size){
            curremain.push_back(curtmp[i]);
        }
    }
    uint64_t curremain_size = curremain.size();
    for(uint64_t i = 1; i <= curremain_size; i++){
        cache.addNode(curremain[curremain_size - i], currentLevel);
    }

    std::vector<std::string>().swap(find);

    std::vector<std::string>().swap(v);

    nextvec = cache.compNextLevel(currentLevel + 1);
    uint64_t min = range.first;
    uint64_t max = range.second;
    uint64_t nextvec_size = nextvec.size();
    for(uint64_t i = 0; i < nextvec_size; i++){
        if((nextvec[i]->min < max && nextvec[i]->min > min)
                || (nextvec[i]->max < max && nextvec[i]->max > min)){
            comp.push_back(nextvec[i]);
        }
        else{
            remain.push_back(nextvec[i]);                   //这样能按照存放在nextvec里时间戳从大到小的顺序
        }
    }
    uint64_t remain_size = remain.size();
    for(uint64_t i = 1; i <= remain_size; i++){
        cache.addNode(remain[remain_size - i], currentLevel + 1);
    }

    std::vector<std::vector<node>> toMerge;
    uint64_t curvec_size = curvec.size();
    for(uint64_t i = 0; i < curvec_size; i++){
        uint64_t curtime = curvec[i]->time;
        std::vector<node> cur;
        uint64_t num = curvec[i]->num;
        for(uint64_t j = 0; j < num; j++){
            node tmp;
            tmp.time = curtime;
            tmp.key = curvec[i]->index[j].first;
            bool flag = true;
            tmp.val = curvec[i]->nodeGet(tmp.key, flag);
            tmp.size = (tmp.val).length() + 12;
            cur.push_back(tmp);
        }
        toMerge.push_back(cur);
    }

    while(toMerge.size() > 1){
        std::vector<std::vector<node>> tmpMerge;
        while(!toMerge.empty()){
            std::vector<node> result;
            std::vector<node> tmp1 = toMerge[toMerge.size() - 1];
            toMerge.pop_back();
            std::vector<node> tmp2 = toMerge[toMerge.size() - 1];
            toMerge.pop_back();
            uint64_t i = 0, j = 0;
            uint64_t tmp1_size = tmp1.size();
            uint64_t tmp2_size = tmp2.size();
            while(i < tmp1_size){
                while(j < tmp2_size){
                    if(i == tmp1_size && j < tmp2_size){
                        result.insert(result.end(), tmp2.begin() + j, tmp2.end());
                        break;
                    }
                    if(tmp1[i].key > tmp2[j].key){
                        result.push_back(tmp2[j]);
                        j++;
                    }
                    else{
                        if(tmp1[i].key < tmp2[j].key){
                            result.push_back(tmp1[i]);
                            i++;
                        }
                        else{
                            if(tmp1[i].time < tmp2[j].time){
                                result.push_back(tmp2[j]);
                            }
                            else{
                                result.push_back(tmp1[i]);
                            }
                            i++;
                            j++;
                        }
                    }
                }
                if(i < tmp1_size && j == tmp2_size){
                    result.insert(result.end(), tmp1.begin() + i, tmp1.end());
                    break;
                }
            }
            tmpMerge.push_back(result);

            //剩下一个
            if(toMerge.size() == 1){
                std::vector<node> tmp = toMerge[0];
                tmpMerge.push_back(tmp);
                std::vector<node>().swap(tmp);
                toMerge.pop_back();
            }
        }
        toMerge = tmpMerge;
        std::vector<std::vector<node>>().swap(tmpMerge);
    }
    std::vector<node> toMerge1 = toMerge[0];
    toMerge.pop_back();

    uint64_t comp_size = comp.size();
    for(uint64_t i = 0; i < comp_size; i++){
        uint64_t curtime = comp[i]->time;
        std::vector<node> cur;
        for(uint64_t j = 0; j < comp[i]->num; j++){
            node tmp;
            tmp.time = curtime;
            tmp.key = comp[i]->index[j].first;
            bool flag = true;
            tmp.val = comp[i]->nodeGet(tmp.key, flag);
            tmp.size = (tmp.val).length() + 12;
            cur.push_back(tmp);
        }

        toMerge.push_back(cur);
    }

    //删除已有的sstable
    for(uint64_t i = 0; i < curvec_size; i++){
        std::string s = curvec[i]->path;
        const char *delFile = s.data();
        utils::rmfile(delFile);
    }

    std::vector<node> mergeResult;
    uint64_t k = 0, i = 0;
    uint64_t toMerge_size = toMerge.size();
    uint64_t toMerge1_size = toMerge1.size();
    for(i = 0; i < toMerge_size; i++){
        std::vector<node> tmp = toMerge[i];
        if(k == toMerge1_size){
            mergeResult.insert(mergeResult.end(), tmp.begin(), tmp.end());
            continue;
        }
        uint64_t tmp_size = tmp.size();
        uint64_t j = 0;
        while(j < tmp_size){
            if(tmp[j].key < min){
                mergeResult.push_back(tmp[j]);
                j++;
                continue;
            }
            if(tmp[j].key > max){
                mergeResult.insert(mergeResult.end(), tmp.begin() + j, tmp.end());
                break;
            }
            if(tmp[j].key < toMerge1[k].key){
                mergeResult.push_back(tmp[j]);
                j++;
            }
            else{
                mergeResult.push_back(toMerge1[k]);
                if(tmp[j].key == toMerge1[k].key){
                    j++;
                }
                k++;
            }
            if(k == toMerge1_size && j < toMerge_size){
                mergeResult.insert(mergeResult.end(), tmp.begin() + j, tmp.end());
                break;
            }
        }

        std::vector<node>().swap(tmp);
    }
    if(k < toMerge1_size && i == toMerge_size){
        mergeResult.insert(mergeResult.end(), toMerge1.begin() + k, toMerge1.end());
    }

    uint64_t mergeResult_size = mergeResult.size();
    int totalsize = 0;
    int prev = 0;
    currentLevel++;

    for(i = 0; i < mergeResult_size; i++){
        int size = mergeResult[i].size;
        //std::cout << "size  " <<size <<std::endl;
        if(totalsize + size > 2086880){
            totalsize = 0;
            i--;
            std::vector<node> vec;
            std::vector<memTable::dataNode> v;
            vec.insert(vec.end(), mergeResult.begin() + prev, mergeResult.begin() + i);
            prev = i;

            uint64_t vec_size = vec.size();
            for(uint64_t j = 0; j < vec_size; j++){
                memTable::dataNode tmp;
                tmp.key = vec[j].key;
                tmp.val = vec[j].val;
                v.push_back(tmp);
            }
            std::vector<node>().swap(vec);

            makeSST(v);
            std::vector<memTable::dataNode>().swap(v);
            continue;
        }
        else{
            totalsize += size;
        }
        if(i == mergeResult_size - 1){
            std::vector<node> vec;
            std::vector<memTable::dataNode> v;
            vec.insert(vec.end(), mergeResult.begin() + prev, mergeResult.end());

            uint64_t vec_size = vec.size();
            for(uint64_t j = 0; j < vec_size; j++){
                memTable::dataNode tmp;
                tmp.key = vec[j].key;
                tmp.val = vec[j].val;
                v.push_back(tmp);
            }

            std::vector<node>().swap(vec);

            makeSST(v);
            std::vector<memTable::dataNode>().swap(v);
            break;
        }
    }

    std::vector<node>().swap(mergeResult);

    std::vector<Cache::Node*>().swap(curvec);

    std::vector<Cache::Node*>().swap(nextvec);

    comp_size = comp.size();
    for(uint64_t i = 0; i < comp_size; i++){
        comp[i]->next = nullptr;
        delete comp[i];
        comp[i] = nullptr;
    }
    std::vector<Cache::Node*>().swap(comp);

    std::vector<Cache::Node*>().swap(remain);

    std::vector<Cache::Node*>().swap(curtmp);

    std::vector<node>().swap(toMerge1);

    toMerge_size = toMerge.size();
    for(uint64_t i = 0; i < toMerge_size; i++){
        std::vector<node>().swap(toMerge[i]);
    }
    std::vector<std::vector<node>>().swap(toMerge);
    isCompaction = false;
    checkCompaction();
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
    mt.reset();
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
    std::string path = "data/level-";

    int i = 0;
    std::string tmpDir = path + std::to_string(i);
    while(utils::dirExists(tmpDir)){
        std::vector<std::string> ret;
        utils::scanDir(tmpDir, ret);
        i++;
        uint64_t ret_size = ret.size();
        for(uint64_t j = 0; j < ret_size; j++){
            std::string tmpPath = tmpDir + ("/" + ret[j]);
            const char *delPath = tmpPath.data();
            utils::rmfile(delPath);
        }
        std::vector<std::string>().swap(ret);
        const char *delDir = tmpDir.data();
        utils::rmdir(delDir);
        tmpDir = path + std::to_string(i);
    }
}
