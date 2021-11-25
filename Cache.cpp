//
// Created by Jade on 24-Nov-21.
//

#include "Cache.h"

Cache& getCache() {
    static Cache cache;

    return cache;
}

