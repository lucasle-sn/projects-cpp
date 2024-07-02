#include <gtest/gtest.h>
#include <public_types/span.h>

namespace {

class TestSpan : public ::testing::Test {
 protected:
  /**
   * @brief Generic assertion on all span elements
   *
   * @tparam T
   * @tparam N
   * @param span Span
   */
  template <typename T, size_t N>
  void assert_span_elements(const qle::Span<T> &span, const T (&buffer)[N]) {
    ASSERT_EQ(span.Size(), N);
    for (size_t i = 0; i < span.Size(); i++) {
      ASSERT_EQ(span[i], buffer[i]);
      ASSERT_EQ(span.at(i), buffer[i]);
    }
  }

  /**
   * @brief Generic test of span type T
   *
   * @tparam T
   */
  template <typename T>
  void test_span_type() {
    T buffer[2]{static_cast<T>(1),
                static_cast<T>(2)};  // 1 and 2 are just random values
    qle::Span<T> span(buffer, sizeof(buffer) / sizeof(buffer[0]));
    assert_span_elements(span, buffer);
  }
};

TEST_F(TestSpan, TestBasic) {
  uint8_t buffer[10]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  size_t buffer_len{sizeof(buffer) / sizeof(buffer[0])};

  qle::Span<uint8_t> span(&buffer[0], buffer_len);

  ASSERT_EQ(span.Size(), buffer_len);

  // Test <T &operator[](std::size_t index)> and <T &at(std::size_t index)>
  assert_span_elements(span, buffer);

  // Test T *Data()
  ASSERT_TRUE(span.Data() == buffer);

  span.reset();
  ASSERT_TRUE(span.Size() == 0);
}

TEST_F(TestSpan, TestVariousTypes) {
  test_span_type<uint8_t>();
  test_span_type<uint16_t>();
  test_span_type<uint32_t>();
  test_span_type<uint64_t>();
  test_span_type<int8_t>();
  test_span_type<int16_t>();
  test_span_type<int32_t>();
  test_span_type<int64_t>();
  test_span_type<float>();
  test_span_type<double>();
}

}  // namespace
