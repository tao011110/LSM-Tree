#include "cache.h"

Cache::Cache()
{
//    head = new Node;
//    cachePrev = head;
//    head->next = nullptr;
//    cacheNext = new Node;
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

Cache::Node ::~Node()
{
    delete next;
    next = nullptr;
    delete[] index;
    index = nullptr;
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
        //std::cout << "new level " << level <<std::endl;
        Node *head = new Node;
        head->next = addition;
        addition->next = nullptr;
        levelVec.push_back(head);
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

    uint64_t beginIndex = getOffsetIndex(key, flag);
    if(flag == false){
        return "";
    }
    else{
        uint32_t beginOffset = index[beginIndex].second;
        uint32_t  endOffset = 0;
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
        //std::cout << "result  " <<result <<std::endl;
        if(result == "~DELETED~"){
            return "";
        }
        else{
            return result;
        }
    }
}

std::string Cache::get(uint64_t key)
{
    //Node *tmp = head->next;
    //std::cout << "key  " << key << std::endl;
    uint64_t levelVec_size = levelVec.size();
    for(uint64_t i = 0; i < levelVec_size; i++){
        Node *tmp = levelVec[i]->next;
        while(tmp != nullptr){
            bool flag = true;
            std::string result = tmp->nodeGet(key, flag);
            if(flag == false){
                tmp = tmp->next;
                continue;
            }
            else{
                if(result != ""){
                    return result;
                }
            }
        }
    }

    return "";
}

bool Cache::del(uint64_t key)
{
    std::string result = get(key);
    if(result == ""){
        return false;
    }

    return true;
}

void Cache::reset()
{
//    Node *p = head->next;
//    while(p){
//        Node *tmp = p->next;
//        delete p;
//        p = tmp;
//    }
//    head->next = nullptr;
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

//std::pair<int, int> Cache::compLevel0(std::vector<Node*> &curvec)
//{
//    curvec.push_back(head->next);
//    curvec.push_back(head->next->next);
//    Node *tmp = head->next->next->next;
//    curvec.push_back(tmp);
//    std::pair<int, int> range = findRange(curvec);
//    head->next = tmp->next;
//    cachePrev = head;
//
//    //return range;
//}

std::pair<int, int> Cache::compCurrentLevel(std::vector<Node *> &curvec, std::vector<std::string> &find, int level)
{
    Node *tmp = levelVec[level]->next;
    std::vector<Node *> remain;
    if(tmp == nullptr){
        //std::cout << "nullll" << std::endl;
    }
    while(tmp != nullptr){
        uint64_t find_size = find.size();
        if(find_size == 0){
            break;
        }
        //std::cout << tmp->path << "  " <<find.size() << std::endl;
        uint64_t j = 0;
        for(j = 0; j < find_size; j++){
            if(tmp->path == find[j]){
                curvec.push_back(tmp);
                //std::cout << tmp->num<<std::endl;
                find.erase(find.begin() + j);
                //tmp->next = del->next;
                break;
            }
        }

        if(j == find_size) {
            remain.push_back(tmp);
            //std::cout << remain[remain.size() - 1]->path << "  and  "<< std::endl;
        }
        tmp = tmp->next;
        if(tmp != nullptr && j == find_size) {
            //std::cout << "  and  " << tmp->path << std::endl;
        }
    }
    levelVec[level]->next = nullptr;
    uint64_t remain_size = remain.size();
    for(uint64_t i = 0; i < remain_size; i++){
        //std::cout << "remain path  " << remain[i]->path << std::endl;
        remain[i]->next = nullptr;
    }
    for(uint64_t i = 1; i <= remain_size; i++){
        remain[remain_size - i]->next = levelVec[level]->next;
        levelVec[level]->next = remain[remain_size - i]->next;
    }
//    Node *head = new Node;
//    Node *n = head->next;
//    Node *del = levelVec[level]->next;
//    while(del != nullptr){
//        uint64_t find_size = find.size();
//        if(find_size == 0){
//            break;
//        }
//        std::cout << "path  " << del->path << std::endl;
//        uint64_t j = 0;
//        for(j = 0; j < find_size; j++){
//            if(del->path == find[j]){
//                curvec.push_back(del);
//                std::cout << del->num<<std::endl;
//                find.erase(find.begin() + j);
//                del = del->next;
//                break;
//            }
//        }
//        if(j == find_size){
//
//        }
//    }


    std::pair<int, int> range = findRange(curvec);

    return range;
}

std::vector<Cache::Node*> Cache::compNextLevel(int currentLevel)
{
    std::vector<Node*> handle;
    if(currentLevel < levelVec.size()) {
        Node *tmp = levelVec[currentLevel];
        while (tmp->next != nullptr) {
            Node *del = tmp->next;
            tmp->next = del->next;
            handle.push_back(del);
            //std::cout << "delete " << del->path<<std::endl;
        }
        levelVec[currentLevel]->next = nullptr;
    }

    return handle;
}

//std::pair<int, int> Cache::compaction(int currentLevel, std::vector<Node*> *curvec, std::vector<Node*> *nextvec)
//{
//    if(currentLevel == 0){
//        compLevel0(curvec);
//        compNextLevel(currentLevel, nextvec);
//    }
//    else{
//        compCurrentLevel(currentLevel, curvec);
//        compNextLevel(currentLevel, nextvec);
//    }
//}
