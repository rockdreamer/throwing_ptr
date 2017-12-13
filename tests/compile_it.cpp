#include "throwing/shared_ptr.hpp"

int main(int argc, char** argv)
{
    // Have one instance of each class in throwing::
    throwing::shared_ptr<int> ptr;
    ptr.reset();
    return 0;
}