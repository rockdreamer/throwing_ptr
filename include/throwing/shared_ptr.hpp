#pragma once
#include <memory>

namespace throwing {

template <typename T> class shared_ptr {
public:
    shared_ptr() = default;

    T *get() {
        if (!p)
            throw std::runtime_error("nullptr access");
        return p.get();
    }

private:
    std::shared_ptr<T> p;
};
}