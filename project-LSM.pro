TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        bloomfilter.cpp \
        cache.cpp \
        correctness.cc \
        kvstore.cc \
        main.cpp \
        memtable.cpp \
        persistence.cc \
        sstable.cpp

DISTFILES += \
    Makefile \
    README.md

HEADERS += \
    MurmurHash3.h \
    bloomfilter.h \
    cache.h \
    kvstore.h \
    kvstore_api.h \
    memtable.h \
    sstable.h \
    test.h \
    utils.h
