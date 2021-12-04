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

void Cache::doCycleWork() {
    Cache &cache = getCache();
    if (cache.memDone) {
        if (cache.readWrite = READ) {
            //data has been loaded into cache array
            cache.hasValidData = true;
            memcpy(cache.dataPtr, &cache.cData[cache.address % 8], cache.count);
            *cache.memDonePtr = true;
        } else if (cache.readWrite = WRITE) {
            //data has been stored from cache array
            for (int i = 0; i < 8; i++) {
                written[i] = 0;
            }
            cache.CLO = cache.address / 8;
            memcpy(&cache.cData[cache.address % 8], cache.dataPtr, cache.count);
            *cache.memDonePtr = true;
        }
    }
}

void Cache::memFetch(unsigned int address, unsigned int count, unsigned char *answerPtr, bool *fetchDonePtr) {
    Cache &cache = getCache();
    DataMemory &memory = getDataMemory();

    //first, check that cache is enabled
    if (!cache.enabled) {
        memory.startFetch(address, count, answerPtr, fetchDonePtr);
        return;
    }

    //then, check for cache hit/miss
    int line = address / 8;
    if (address == 0xFF) {
        cache.hasValidData = false;
        *answerPtr = 0;
        *fetchDonePtr = true;
        return;
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
        cache.readWrite = READ;
    }
}

void Cache::memStore(unsigned int address, unsigned int count, unsigned char *answerPtr, bool *storeDonePtr) {
    Cache &cache = getCache();
    DataMemory &memory = getDataMemory();

    if (!cache.enabled) {
        memory.startStore(address, count, answerPtr, storeDonePtr);
        return;
    }

    //check for cache hit/miss
    int line = address / 8;
    if (address == 0xFF) {
        bool needFlush = false;
        for (int i = 0; i < 8; i++) {
            if (cache.written[i]) {
                needFlush = true;
                break;
            }
        }
        if (needFlush) {
            memory.cacheStore(cache.CLO * 0x8, cache.written, cache.cData, storeDonePtr);
        } else {
            *storeDonePtr = true;
        }
        return;
    }
    if (cache.CLO == line || !cache.hasValidData) {
        memcpy(&cache.cData[address % 8], answerPtr, 1);
        *storeDonePtr = true;
        cache.written[address % 8] = true;
    } else {
        //cache miss
        bool needFlush = false;
        for (int i = 0; i < 8; i++) {
            if (cache.written[i]) {
                needFlush = true;
                break;
            }
        }
        memory.cacheStore(cache.CLO * 0x8, cache.written, cache.cData, &cache.memDone);
        cache.readWrite = WRITE;
        cache.address = address;
        cache.dataPtr = answerPtr;
        cache.memDonePtr = storeDonePtr;
    }

}

void Cache::flush() {
    DataMemory &memory = getDataMemory();
    Cache &cache = getCache();

    memory.cacheStore(cache.CLO * 0x8, cache.written, cache.cData, cache.memDonePtr);
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