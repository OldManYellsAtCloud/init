#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <array>
#include <iterator>
#include <future>
#include "inittask.h"

void removeTrailingNewlines(char* chr, size_t maxLength){
    for (size_t i = 1; i <= maxLength; ++i){
        if (chr[i] == 0) {
            if (chr[i - 1] == '\n' || chr[i - 1] == '\r')
                chr[i - 1] = 0;
            return;
        }
    }
}

void removeTrailingNewlines(char* begin, char* end){
    removeTrailingNewlines(begin, end - begin);
}

bool isResultReady(std::future<init::TASK_STATUS> const& f){
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

#endif // UTILS_H
