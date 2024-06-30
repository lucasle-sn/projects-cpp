#ifndef UTILITIES_BYTESTREAM_H
#define UTILITIES_BYTESTREAM_H

#include <public_types/span.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <type_traits>

namespace unimelb {

/**
 * @brief Endianess option
 *
 * Representation of 0x1A2B3C4D5E6F7080 in big-endian and little-endian:
 *  BIG_END:      [1A|2B|3C|4D|5E|6F|70|80]
 *  LITTLE_END:   [80|70|6F|5E|4D|3C|2B|1A]
 */
enum class Endianess {
  BIG_END,
  LITTLE_END,
};

/**
 * @brief Constant byte size in bit
 */
static constexpr uint8_t cByteSize{8};

/**
 * @brief Bytestream takes a stream of bytes and extracts data
 */
class Bytestream {
 public:
  /**
   * @brief Delete default constructor
   */
  Bytestream() = delete;

  /**
   * @brief Delete copy constructor
   */
  Bytestream(const Bytestream &) = delete;

  /**
   * @brief Delete move constructor
   */
  Bytestream(Bytestream &&) = delete;

  /**
   * @brief Delete copy assignment
   */
  Bytestream &operator=(const Bytestream &) = delete;

  /**
   * @brief Delete move assignment
   */
  Bytestream &operator=(Bytestream &&) = delete;

  /**
   * @brief Construct a new Bytestream object
   *
   * @param buffer Byte buffer
   * @param size Length of buffer
   * @param endianess Endianess
   */
  explicit Bytestream(uint8_t *buffer, size_t size,
                      Endianess endianess = Endianess::BIG_END) noexcept
      : span_(buffer, size), endianess_(endianess){};

  /**
   * @brief Destructor
   */
  ~Bytestream() noexcept { reset(); };

  /**
   * @brief Reset bytestream
   */
  void reset() noexcept { cursor_ = 0; }

  /**
   * @brief Move cursor
   *
   * @param position Cursor position
   * @return bool
   */
  bool move(size_t position) noexcept {
    if (position >= span_.Size()) {
      return false;
    }
    cursor_ = position;
    return true;
  }

  /**
   * @brief Check if buffer is overflowed when increasing \p size bytes
   *
   * @param size Increasing size
   * @return bool
   */
  bool is_overflow(size_t size) const noexcept {
    return (cursor_ + size > span_.Size());
  }

  /**
   * @brief Get data type T from bytestream
   *
   * @tparam T
   * @param data Output data
   * @param data_len Output data length
   * @return bool
   */
  template <typename T>
  typename std::enable_if_t<std::is_integral<T>::value, bool> get(
      T &data, size_t data_len = sizeof(T)) noexcept {
    uint64_t dest{0};

    if (!get(dest, data_len)) {
      return false;
    }

    data = static_cast<T>(dest);
    cursor_ += data_len;
    return true;
  }

  /**
   * @brief Get data type T from bytestream
   *
   * @tparam T
   * @param data Output data
   * @return bool
   */
  template <typename T>
  typename std::enable_if_t<std::is_floating_point<T>::value, bool> get(
      T &data) noexcept {
    uint64_t dest{0};

    if (!get(dest, sizeof(T))) {
      return false;
    }

    memcpy(&data, &dest, sizeof(T));
    cursor_ += sizeof(T);
    return true;
  }

 private:
  /**
   * @brief Generic get data from bytestream
   *
   * @param data Output data
   * @param data_len Data length
   * @return bool
   */
  bool get(uint64_t &data, size_t data_len) noexcept {
    if (is_overflow(data_len)) {
      return false;
    }

    uint64_t dest{0};

    for (size_t i = 0; i < data_len; i++) {
      if (endianess_ == Endianess::BIG_END) {
        dest = (dest << cByteSize) | (span_[cursor_ + i] & 0xFFFFFFFFFFFFFFFFU);
      } else {
        dest |= ((span_[cursor_ + i] & 0xFFFFFFFFFFFFFFFFU) << (cByteSize * i));
      }
    }
    data = dest;
    return true;
  }

  /**
   * @brief Byte span
   */
  Span<uint8_t> span_;

  /**
   * @brief Cursor of bytestream
   */
  size_t cursor_{0};

  /**
   * @brief Endianess
   */
  Endianess endianess_{Endianess::BIG_END};
};

}  // namespace unimelb

#endif  // UTILITIES_BYTESTREAM_H
