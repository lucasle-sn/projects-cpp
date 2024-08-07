#ifndef PUBLIC_TYPES_SPAN_H
#define PUBLIC_TYPES_SPAN_H

#include <cstddef>

namespace qle {

/**
 * @brief Span library
 *
 * @tparam T
 */
template <typename T>
class Span {
 public:
  /**
   * @brief Construct a new Span object
   *
   * @param ptr Data
   * @param count Size
   */
  explicit Span(T *ptr, std::size_t count) : data(ptr), size(count) {}

  /**
   * @brief Default constructor deleted
   */
  Span() = delete;

  /**
   * @brief Copy constructor deleted
   */
  Span(const Span &) = delete;

  /**
   * @brief Move constructor deleted
   */
  Span(Span &&) = delete;

  /**
   * @brief Copy assignment deleted
   */
  Span &operator=(const Span &) = delete;

  /**
   * @brief Move assignment deleted
   */
  Span &operator=(Span &&) = delete;

  /**
   * @brief Destroy the Span object
   */
  ~Span() { reset(); }

  /**
   * @brief Reset span object
   */
  void reset() { size = 0; }

  /**
   * @brief Element access
   *
   * @param index Index
   * @return T&
   */
  T &operator[](std::size_t index) const { return data[index]; }

  /**
   * @brief Element access
   *
   * @param index Index
   * @return T&
   */
  T &at(std::size_t index) const { return data[index]; }

  /**
   * @brief Get span size
   *
   * @return std::size_t
   */
  std::size_t Size() const { return size; }

  /**
   * @brief Get data
   *
   * @return T*
   */
  T *Data() const { return data; }

 private:
  /**
   * @brief Span data
   */
  T *data;

  /**
   * @brief Span size
   */
  std::size_t size;
};

}  // namespace qle

#endif  // PUBLIC_TYPES_SPAN_H
