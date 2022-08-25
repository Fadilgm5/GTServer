#pragma once
#include <stdint.h>
#include <string>

namespace proton::utils {
    inline uint32_t hash(const char *data, int32_t len) {
        if (!data) return 0;
        unsigned char *n = (unsigned char*)data;
        uint32_t acc = 0x55555555;

        if (len == 0) {
            while (*n)
                acc = (acc >> 27) + (acc << 5) + *n++;
            return acc;
        }
        for (int32_t i=0; i < len; i++)
            acc = (acc >> 27) + (acc << 5) + *n++;
        return acc;
    }
};