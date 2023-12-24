#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <functional>

/**
 * @brief Calculate product of 1 input
 *
 * @tparam T
 * @param a Input
 * @return std::enable_if_t<std::is_same<T, int>::value, int>
 */
template <typename T>
typename std::enable_if_t<std::is_same<T, int>::value, int> product(T a) {
  return a;
}

/**
 * @brief Calculate product of various number of input
 *
 * @tparam T
 * @tparam Args
 * @param a 1st input
 * @param args Other inputs
 * @return std::enable_if_t<std::is_same<T, int>::value, int>
 */
template <typename T, typename... Args>
typename std::enable_if_t<std::is_same<T, int>::value, int> product(
    T a, Args... args) {
  return a * product(args...);
}

int main() {
  printf("Product of {2,3}: %d\n", product(2, 3));
  printf("Product of {2,3,4}: %d\n", product(2, 3, 4));
  printf("Product of {2,3,6,8}: %d\n", product(2, 3, 6, 8));
  printf("Product of {2,3,6,8,10}: %d\n", product(2, 3, 6, 8, 10));
  printf("Product of {2,3,6,8,10,-1}: %d\n", product(2, 3, 6, 8, 10, -1));

  return EXIT_SUCCESS;
}
