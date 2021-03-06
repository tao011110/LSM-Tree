#include "cache.h"

Cache::Cache()
{

}

Cache::~Cache()
{
    reset();
}

Cache::Node ::Node(std::string &p, sstable &st)
{
    path = p;
    time = st.getTime();
    num = st.getNum();
    max = st.getMax();
    min = st.getMin();
    index = new std::pair<uint64_t, uint32_t>[num];
    st.getBf(&bf);
    st.getIndex(index);
    next = nullptr;
}

Cache::Node ::Node(std::string &p)
{
    std::ifstream file;
    path = p;
    //std::cout << "path  " << p << std::endl;
    file.open(p, std::ios::in | std::ios::binary);
    file.read((char *)&time, 8);
    //std::cout << "time  " << time << std::endl;
    file.read((char *)&num, 8);
    //std::cout << "num  " << num << std::endl;
    file.read((char *)&min, 8);
    //std::cout << "min  " << min << std::endl;
    file.read((char *)&max, 8);
    //std::cout << "max  " << max << std::endl;
    bf.getFromFile(file);

    index = new std::pair<uint64_t, uint32_t>[num];
    for(int i = 0; i < num; i++){
        uint64_t key = 0;
        uint32_t offset = 0;
        file.read((char*)&key, 8);
        file.read((char*)&offset, 4);
        index[i] = std::pair<uint64_t, uint32_t>(key, offset);
    }

    file.close();
    //std::cout << "num  " << num << std::endl;
}

Cache::Node ::~Node()
{
    delete[] index;
    index = nullptr;
}

void Cache::fileToNode(int level, std::string p)
{
    Node *addition = new Node(p);
    addition->next = nullptr;
    addNode(addition, level);
}

void Cache::add(std::string &p, sstable &st, int level)
{
    Node *addition = new Node(p, st);
    addition->next = nullptr;
    addNode(addition, level);
}

void Cache::addNode(Node *addition, int level)
{
    if(level >= levelVec.size()){
        while(level >= levelVec.size()) {
            //std::cout << "new level " << level <<std::endl;
            Node *head = new Node;
            levelVec.push_back(head);
        }
        levelVec[levelVec.size() - 1]->next = addition;
        addition->next = nullptr;
    }
    else{
        addition->next = levelVec[level]->next;
        levelVec[level]->next = addition;
    }
}

uint64_t Cache::Node ::getOffsetIndex(uint64_t key, bool &flag)
{
    int left = 0, right = num - 1, mid = 0;
    while(left <= right){
        mid = (left + right) / 2;
        if(index[mid].first == key){
            //std::cout << "find " << std::endl;
            return mid;
        }
        if(index[mid].first < key){
            left = mid + 1;
        }
        else{
            right = mid - 1;
        }
    }
    flag = false;
    return 0;
}

std::string Cache::Node ::nodeGet(uint64_t key, bool &flag)
{
    if(key > max || key < min){
//        if(key >= 65338 && key <= 65462 && isF == true)
//            std::cout <<  key<< "  " << "max " << max << "  min   " << min << std::endl;
        flag = false;
        return "";
    }
    //std::cout << "key  wc" << key << std::endl;
    uint32_t hashVal[4] = {0};
    uint64_t tmpKey = key;
    MurmurHash3_x64_128(&tmpKey, sizeof(tmpKey), 1, hashVal);
//    for(int i = 0; i < 4; i++){
//        std::cout << "hash " << hashVal[i] << std::endl;
//    }
    if(!bf.match(hashVal)){
//        if(key >= 65338 && key <= 65462 && isF == true)
//            std::cout <<key <<  "not bf" << std::endl;
        flag = false;
        return "";
    }
//    if(key >= 65300) {
//        std::cout <<key << "  may nwe" << std::endl;
//    }
    uint64_t beginIndex = getOffsetIndex(key, flag);
    //std::cout << "key  " << key << std::endl;
    if(flag == false){
        return "";
    }
    else{
        uint32_t beginOffset = index[beginIndex].second;
        uint32_t endOffset = 0;
        std::fstream file(path, std::ios::in|std::ios::binary);
        if(beginIndex < num - 1){
            endOffset = index[beginIndex + 1].second;
        }
        else{
            file.seekg(0, std::ios::end);
            endOffset = file.tellg();
        }
        //std::cout << " beginOff " << beginOffset <<" endOff " << endOffset << std::endl;
        uint32_t size = endOffset - beginOffset;
        file.seekg(beginOffset, std::ios::beg);
        char *str = new char[size + 1];
        memset(str, '\0', size + 1);
        file.read(str, size);
        file.seekg(10280, std::ios::beg);
        uint32_t s;

        file.read(reinterpret_cast<char*>(&s), 4);
        //std::cout << "my  " << s << std::endl;
        file.close();
        std::string result = str;
        delete[] str;
//        if(result == ""){
//            std::cout << key << "   dont why" << std::endl;
//        }
        //std::cout << "result  " <<result <<std::endl;
//        if(result == "~DELETED~"){
//            return "";
//        }
//        else{
//            return result;
//        }
//        if(key >= 65300) {
//            std::cout << key << "    there might be    " << result.length() << std::endl;
//        }
        return result;
    }
}

std::string Cache::get(uint64_t key)
{
    uint64_t levelVec_size = levelVec.size();
    std::string result = "";
    for(uint64_t i = 0; i < levelVec_size; i++){
        Node *tmp = levelVec[i]->next;
        while(tmp != nullptr){
            bool flag = true;
            //std::cout << "tmp  " << tmp->path;
            result = tmp->nodeGet(key, flag);
            if(flag == false){
                tmp = tmp->next;
                continue;
            }
            else{
                if(result != ""){
//                    if(key >= 65300) {
//                        std::cout << key << "   " << result.length() << result[0] << result[1] << std::endl;
//                    }
                    return result;
                }
                else{
                    tmp = tmp->next;
                }
            }
        }
    }
//    if(key >= 65300) {
//        std::cout << key << "   " << result.length() << std::endl;
//    }

    return "";
}

bool Cache::del(uint64_t key)
{
    std::string result = get(key);
    if(result == "" || result == "~DELETED~"){
        return false;
    }

    return true;
}

void Cache::reset()
{
    uint64_t levelVec_size = levelVec.size();
    for(uint64_t i = 0; i < levelVec_size; i++){
        Node *del = levelVec[i];
        while(del != nullptr){
            Node *tmp = del->next;
            delete del;
            del = tmp;
        }
    }

    std::vector<Node*>().swap(levelVec);
}

std::pair<int, int> Cache::findRange(std::vector<Node*> &vec)
{
    std::pair<int, int> result;
    uint64_t max = 0, min =  UINT64_MAX - 1;
    for(int i = 0; i < (int)vec.size(); i++){
        if(max < vec[i]->max){
            max = vec[i]->max;
        }
        if(min > vec[i]->min){
            min = vec[i]->min;
        }
    }
    result.first = min;
    result.second = max;

    return result;
}

std::vector<Cache::Node*> Cache::compLevel(int currentLevel, std::pair<int, int> &range)
{
    std::vector<Node*> handle;
    if(currentLevel < levelVec.size()) {
        Node *tmp = levelVec[currentLevel]->next;
        while (tmp != nullptr) {
            Node *del = tmp;
            tmp = del->next;
            handle.push_back(del);
        }
        levelVec[currentLevel]->next = nullptr;
    }
    range = findRange(handle);
    uint64_t handle_size = handle.size();
    for(int i = 0; i < handle_size; i++){
        handle[i]->next = nullptr;
    }

    return handle;
}
