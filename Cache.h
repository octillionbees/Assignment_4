//
// Created by Jade on 24-Nov-21.
//
#include <stdlib.h>
#include <stdio.h>
#ifndef ASSIGNMENT_4_CACHE_H
#define ASSIGNMENT_4_CACHE_H
using namespace std;

class Cache {
    //State Definitions
    const int READ = 0;
    const int WRITE = 1;

public:
    unsigned char CLO;
    bool hasValidData;
    bool enabled;
    unsigned char cData[8];
    bool written[8];
    bool memDone;
    bool* memDonePtr;
    unsigned char* dataPtr;
    int count;
    unsigned int address;
    int readWrite;


    void reset();

    void enable();

    void disable();

    void dump();

    void cacheParse(FILE* inFile);

    void memFetch(unsigned int address, unsigned int count, unsigned char *answerPtr, bool *fetchDonePtr);

    void memStore(unsigned int address, unsigned int count, unsigned char *answerPtr, bool *storeDonePtr);

    void doCycleWork();

    void flush();
};


#endif //ASSIGNMENT_4_CACHE_H

Cache &getCache();