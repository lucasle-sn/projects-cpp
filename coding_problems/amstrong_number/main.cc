#include <errno.h>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>

/**
 * @brief Check if a number is Amstrong number
 * 
 * An Amstrong number is a number that is equal sum of each digit powered by the
 * number of digits. For example: [abc = a^3 + b^3 + c^3] [abcd = a^4 + b^4 +
 * c^4 + d^4]
 *
 * @param number Number that need checked
 * @return bool
 */
bool is_amstrong_number(int number) {
  if (number <= 0) {
    return false;
  }

  auto original_number = number;

  std::array<u_int8_t, 100> digits{0};
  size_t digit_idx = 0;
  while (number > 0) {
    digits[digit_idx] = number % 10;
    digit_idx++;
    number = static_cast<int>(number / 10);
  }

  int sum = 0;
  for (size_t i = 0; i < digit_idx; i++) {
    sum += pow(digits[i], digit_idx);
  }

  return (original_number == sum ? true : false);
}

/**
 * @brief Parse integer parameter
 *
 * @param args argument
 * @param end_args remaining argument
 * @param out Output value
 * @return bool
 */
bool integer_argparsing(const char *args, char *end_args, int &out) {
  int number = static_cast<int>(std::strtol(args, &end_args, 0));

  if (args == end_args) {
    fprintf(stderr, "Number : %d Invalid (no digits found, 0 returned)\n",
            number);
    return false;
  } else if (errno == ERANGE) {
    fprintf(stderr, "Number : %d Invalid (Overflow/Underflow occured)\n",
            number);
    return false;
  } else if (errno != 0 && number == 0) {
    printf("Number : %d Invalid (unspecified error occurred)\n", number);
    return false;
  } else if (errno == 0 && end_args && *end_args != 0) {
    printf("Number : %d Valid (but additional characters remain)\n", number);
    return false;
  }

  out = number;
  return true;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Exact one integer required\n");
    return EXIT_FAILURE;
  }

  /* reset errno to 0 before call */
  errno = 0;

  int number{0};
  char *endptr{nullptr};
  if (!integer_argparsing(argv[1], endptr, number)) {
    return EXIT_FAILURE;
  }

  if (is_amstrong_number(number)) {
    fprintf(stdout, "%d is Amstrong number\n", number);
  } else {
    fprintf(stdout, "%d is not Amstrong number\n", number);
  }
  return EXIT_SUCCESS;
}
