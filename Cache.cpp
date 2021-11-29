//
// Created by Jade on 24-Nov-21.
//

#include "Cache.h"
#include "DatMem.h"
#include <cstring>
#include <iostream>
#include <string.h>

Cache& getCache() {
    static Cache cache;

    return cache;
}

void Cache::reset() {
    Cache &cache = getCache();

    cache.CLO = 0;
    cache.enabled = false;
    cache.hasValidData = false;
    for (int i = 0; i < 8; i++) {
        cache.written[i] = false;
    }
}

void Cache::enable() {
    Cache &cache = getCache();
    cache.enabled = true;
}

void Cache::disable() {
    Cache &cache = getCache();
    cache.enabled = false;
}

void Cache::dump() {
    Cache &cache = getCache();
    printf("CLO        : 0x%02X\n", cache.CLO);
    printf("cache data :");
    for (int i = 0; i < 8; i++) {
        printf(" 0x%02X", cache.cData[i]);
    }
    printf("\n");
    printf("Flags      :");
    for (int i = 0; i < 8; i++) {
        if (cache.written[i]) {
            printf("   W ");
        } else {
            if (cache.enabled) {
                printf("   V ");
            } else {
                printf("   I ");
            }
        }
    }
}

void Cache::startTick() {
    Cache &cache = getCache();
    if (cache.state = WAIT) {
        if (cache.memDone) {
            //data has been loaded into cache array
            cache.hasValidData = true;
            memcpy(cache.dataPtr, &cache.cData[cache.address % 8], cache.count);
            *cache.memDonePtr = true;
        }
    }
}

void Cache::memFetch(unsigned int address, unsigned int count, unsigned char *answerPtr, bool *fetchDonePtr) {
    Cache &cache = getCache();
    DataMemory &memory = getDataMemory();

    //first, check for cache hit/miss
    int line = address / 8;
    if (address == 0xFF) {
        cache.hasValidData = false;
        *answerPtr = 0;
        *fetchDonePtr = true;
    }
    if (line == cache.CLO && cache.hasValidData) {
        //cache hit!
        memcpy(answerPtr, &cache.cData[address % 8], count);
        *fetchDonePtr = true;
    } else {
        //cache miss :(
        cache.CLO = line;
        cache.address = address;
        cache.count = count;
        cache.dataPtr = answerPtr;
        cache.memDonePtr = fetchDonePtr;
        memory.startFetch(cache.CLO * 0x8, 8, cache.cData, &cache.memDone);
    }
}

void Cache::cacheParse(FILE* inFile) {
    Cache &cache = getCache();

    char* cmd;
    fscanf(inFile, "%ms", &cmd);
    if (!strcmp("reset", cmd)) {
        cache.reset();
    } else if (!strcmp("on", cmd)) {
        cache.enable();
    } else if (!strcmp("off", cmd)) {
        cache.disable();
    } else if (!strcmp("dump", cmd)) {
        cache.dump();
    }

    free(cmd);
}