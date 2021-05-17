#include "cache.h"

Cache::Cache()
{
    head = new Node();
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

void Cache::add(std::string &p, sstable &st)
{
    Node *addition = new Node(p, st);
    addition->next = head->next;
    head->next = addition;
}

std::string Cache::get(uint64_t key)
{
    Node *tmp = head->next;
    while(tmp != nullptr){
        bool flag = true;
        uint64_t beginIndex = tmp->getOffsetIndex(key, flag);
        if(flag == false){
            tmp = tmp->next;
            continue;
        }
        else{
            uint32_t beginOffset = tmp->index[beginIndex].second;
            uint32_t  endOffset = 0;
            std::fstream file(tmp->path, std::ios::in|std::ios::binary);
            if(beginIndex < tmp->num - 1){
                endOffset = tmp->index[beginIndex + 1].second;
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
            //std::cout << "result  " <<result <<std::endl;
            if(result == "~DELETED~"){
                return "";
            }
            else{
                return result;
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
    Node *p = head->next;
    while(p){
        Node *tmp = p->next;
        delete p;
        p = tmp;
    }
    head->next = nullptr;
}
