#include <gtest/gtest.h>
#include <public_types/span.h>

class TestSpan : public ::testing::Test {};

TEST_F(TestSpan, TestBasic) {
  uint8_t buffer[10]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  size_t buffer_len{sizeof(buffer) / sizeof(buffer[0])};

  Span<uint8_t> span(buffer, buffer_len);

  ASSERT_EQ(span.Size(), buffer_len);

  // Test <T &operator[](std::size_t index)> and <T &at(std::size_t index)>
  for (size_t i = 0; i < span.Size(); i++) {
    ASSERT_EQ(span[i], buffer[i]);
    ASSERT_EQ(span.at(i), buffer[i]);
  }

  // Test T *Data()
  ASSERT_TRUE(span.Data() == buffer);

  span.reset();
  ASSERT_TRUE(span.Data() == nullptr);
  ASSERT_TRUE(span.Size() == 0);
}
