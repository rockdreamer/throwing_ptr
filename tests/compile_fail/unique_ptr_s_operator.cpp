#include <throwing/unique_ptr.hpp>

int main() {
    // operator[] is not present in single element unique_ptr
    throwing::unique_ptr<int> up;
    return up[0];
}
