#ifndef UTILS_H
#define UTILS_H
#include <iostream>

template <class dtype>
void btoa(uint8_t* char_array, dtype value, uint64_t size)
{
    uint64_t mask = 0xFF;
    for (uint64_t i = 0; i < size; i++)
    {
        char_array[i] = (mask & value)>>(8*i);
        mask <<= 8;
    }

}

#endif