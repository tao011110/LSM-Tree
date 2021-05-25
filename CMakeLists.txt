cmake_minimum_required(VERSION 3.19)
project(LSM_2)

set(CMAKE_CXX_STANDARD 14)
file(GLOB SOURCES
        bloomfilter.h
        bloomfilter.cpp
        cache.h
        cache.cpp
        correctness.cc
        kvstore.h
        kvstore.cc
        kvstore_api.h
        memtable.h
        memtable.cpp
        MurmurHash3.h
        sstable.h
        sstable.cpp
        test.h
        utils.h
        # persistence.cc
        # main.cpp
        )

add_executable(LSM_2correctness.cc ${SOURCE_FILES}${SOURCES})