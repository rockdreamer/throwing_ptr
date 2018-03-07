#include <throwing/unique_ptr.hpp>

int main() {
    // cannot assign from unique_ptr to another
    throwing::unique_ptr<int> from;
    throwing::unique_ptr<int> to;
    to = from;
    return 0;
}
