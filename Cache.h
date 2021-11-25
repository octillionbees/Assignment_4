//
// Created by Jade on 24-Nov-21.
//

#ifndef ASSIGNMENT_4_CACHE_H
#define ASSIGNMENT_4_CACHE_H


class Cache {
public:
    unsigned char CLO;
    bool hasValidData;
    bool enabled;
    unsigned char cData[8];


    void reset();

    void enable();

    void disable();

    void dump();


};


#endif //ASSIGNMENT_4_CACHE_H

Cache &getCache();