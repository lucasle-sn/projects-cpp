#include <gtest/gtest.h>
#include <utilities/bytestream.h>
#include <cmath>

using Bytestream = qle::Bytestream;

namespace {

class TestBytestream : public ::testing::Test {
 protected:
  /**
   * @brief Assert Bytestream::get() with various output types
   *
   * @tparam T
   * @param endianess Bytestream endianess
   */
  template <typename T>
  void assert_get_types(qle::Endianess endianess = qle::Endianess::BIG_END) {
    // use a fix buffer
    uint8_t buffer[]{0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    size_t buffer_len = sizeof(buffer) / sizeof(buffer[0]);
    // Equivalent buffer memory in Big Endian
    const uint64_t cBufferU64BE{0x0123456789ABCDEF};
    // Equivalent buffer memory in Little Endian
    const uint64_t cBufferU64LE{0xEFCDAB8967452301};

    Bytestream bs(buffer, buffer_len, endianess);

    for (size_t i = 0; i <= buffer_len - sizeof(T); i++) {
      uint64_t expected_tmp{0};
      if (endianess == qle::Endianess::BIG_END) {
        expected_tmp = static_cast<T>(
            (cBufferU64BE << (i * qle::cByteSize) >>
             (sizeof(cBufferU64BE) - sizeof(T)) * qle::cByteSize) &
            0xFFFFFFFFFFFFFFFFU);
      } else {
        expected_tmp = static_cast<T>(
            cBufferU64LE << ((sizeof(cBufferU64LE) - sizeof(T) - i) *
                             qle::cByteSize) >>
            ((sizeof(cBufferU64LE) - sizeof(T)) * qle::cByteSize));
      }

      T dest{0};
      ASSERT_TRUE(bs.get(dest));

      T expected{0};
      memcpy(&expected, &expected_tmp, sizeof(T));

      if (std::is_integral<T>::value) {
        // Check when T is integral type
        EXPECT_EQ(dest, expected);
      } else if (std::is_floating_point<T>::value) {
        // Check when T is floating type
        (void)memcmp(&dest, &expected, sizeof(T));
      }

      bs.move(i + 1);
    }
  }
};

/**
 * @brief Test funcdamental functions of Bytestream class
 */
TEST_F(TestBytestream, TestBasic) {
  uint8_t buffer[]{0x00, 0x01, 0x02, 0x03};
  size_t buffer_len = sizeof(buffer) / sizeof(buffer[0]);

  // Test constructor
  Bytestream bs(buffer, buffer_len);

  // Test move()
  for (size_t i = 0; i < buffer_len; i++) {
    ASSERT_TRUE(bs.move(i));
  }
  ASSERT_FALSE(bs.move(buffer_len));  // Suppose to return overflow

  // Test reset()
  bs.reset();
  ASSERT_TRUE(!bs.is_overflow(buffer_len));  // Suppose to return overflow
}

/**
 * @brief Test Bytestream::get() with various output types
 */
TEST_F(TestBytestream, TestGetVariousTypes) {
  // Endiness::BIG_END
  assert_get_types<uint8_t>();
  assert_get_types<uint16_t>();
  assert_get_types<uint32_t>();
  assert_get_types<uint64_t>();
  assert_get_types<int8_t>();
  assert_get_types<int16_t>();
  assert_get_types<int32_t>();
  assert_get_types<int64_t>();
  assert_get_types<float>();
  assert_get_types<double>();

  // Endiness::LITTLE_END
  assert_get_types<uint8_t>(qle::Endianess::LITTLE_END);
  assert_get_types<uint16_t>(qle::Endianess::LITTLE_END);
  assert_get_types<uint32_t>(qle::Endianess::LITTLE_END);
  assert_get_types<uint64_t>(qle::Endianess::LITTLE_END);
  assert_get_types<int8_t>(qle::Endianess::LITTLE_END);
  assert_get_types<int16_t>(qle::Endianess::LITTLE_END);
  assert_get_types<int32_t>(qle::Endianess::LITTLE_END);
  assert_get_types<int64_t>(qle::Endianess::LITTLE_END);
  assert_get_types<float>(qle::Endianess::LITTLE_END);
  assert_get_types<double>(qle::Endianess::LITTLE_END);
}

}  // namespace
