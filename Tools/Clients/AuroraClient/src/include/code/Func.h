#pragma once

#include <inttypes.h>

namespace code {
    template <typename ret, uint32_t addr, typename... Args>
    class Func {
    public:
        ret operator()(Args... args) {
            return ((ret(*)(Args...))(addr))(args...);
        }
    };
}