#include <gtest/gtest.h>
#include <throwing/shared_ptr.hpp>

TEST(Dereference, Default) {
    throwing::shared_ptr<int> nothing;
    EXPECT_THROW(nothing.get(), std::runtime_error);
}
