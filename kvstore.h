#pragma once

#include "kvstore_api.h"
#include "memtable.h"
#include "sstable.h"
#include "cache.h"
#include <fstream>
#include <cmath>

class KVStore : public KVStoreAPI {
	// You can add your implementation here
private:
    memTable mt;
    uint64_t time = 1;
    Cache cache;
    int currentLevel = 0;
    int currentNum = 0;

public:
	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

    void compaction();

//    int getCurrentLevel(){
//        return currentLevel;
//    }

//    int getCurrentNum(){
//        return currentNum;
//    }

//    void setCurrentLevel(int level){
//        currentLevel = level;
//    }

//    void setCurrentNum(int num){
//        currentNum = num;
//    }
    void make();
};
