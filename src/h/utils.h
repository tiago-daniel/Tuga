//
// Created by metta on 10/6/24.
//

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdint>

using namespace std;

namespace utils {

    inline uint64_t Bit(uint8_t n) {
        return 1ULL << n;
    }

};



#endif //UTILS_H
