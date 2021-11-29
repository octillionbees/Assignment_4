//
// Created by Jade on 24-Nov-21.
//
#include <stdlib.h>
#include <stdio.h>
#ifndef ASSIGNMENT_4_CACHE_H
#define ASSIGNMENT_4_CACHE_H
using namespace std;

class Cache {
public:
    unsigned char CLO;
    bool hasValidData;
    bool enabled;
    unsigned char cData[8];
    bool written[8];
    int state;
    const int IDLE = 0;
    const int WAIT = 1;
    bool memDone;
    bool* memDonePtr;
    unsigned char* dataPtr;
    int count;
    unsigned int address;


    void reset();

    void enable();

    void disable();

    void dump();

    void cacheParse(FILE* inFile);

    void memFetch(unsigned int address, unsigned int count, unsigned char *answerPtr, bool *fetchDonePtr);

    void memStore(unsigned int address, unsigned int count, unsigned char *answerPtr, bool *fetchDonePtr);

    void startTick();

    void doCycleWork();

    bool moreCycleWorkNeeded();
};


#endif //ASSIGNMENT_4_CACHE_H

Cache &getCache();