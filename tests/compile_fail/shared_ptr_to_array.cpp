#include <throwing/shared_ptr.hpp>

int main() {
    int *p = new int[10];
    throwing::shared_ptr<int[]> sp(p);
    return *p = sp[0];
}
