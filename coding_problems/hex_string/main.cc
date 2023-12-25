#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

/**
 * @brief Size of HEX characters
 */
static constexpr size_t cHexCharSize{16};
/**
 * @brief List of HEX characters
 */
static const std::array<const char, cHexCharSize> cHexChar{
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/**
 * @brief Check if a character is in character set
 *
 * @tparam N
 * @param ch Character
 * @param character_set Character set
 * @return bool
 */
template <size_t N>
bool in_character_set(const char ch,
                      const std::array<const char, N> character_set) {
  auto it = std::find(character_set.begin(), character_set.end(), ch);
  if (it == character_set.end()) {
    return false;
  }
  return true;
}

/**
 * @brief Check if a string have all characters in character set
 *
 * @tparam N
 * @param text String
 * @param character_set Character set
 * @return bool
 */
template <size_t N>
bool in_character_set(const char *text,
                      const std::array<const char, N> character_set) {
  // If text is invalid if it is empty
  if ((text == nullptr) || (strlen(text) == 0)) {
    return false;
  }

  for (size_t i = 0; i < strlen(text); i++) {
    if (!in_character_set(std::toupper(text[i]), character_set)) {
      return false;
    }
  }
  return true;
}

bool is_hex_code(const char *text) {
  if ((text == nullptr) || (strlen(text) == 0)) {
    return false;
  }

  // A hex code will start with character '#'
  if (text[0] != '#') {
    return false;
  }
  return in_character_set(text + 1, cHexChar);
}

int main() {
  std::array<const char *, 6> texts{"",        "#",       "CD5C5C",
                                    "#CD5C5Z", "#eaecee", "#CD5C58F"};

  for (auto &text : texts) {
    if (is_hex_code(text)) {
      printf("%s is HEX code\n", text);
    } else {
      printf("%s is not HEX code\n", text);
    }
  }

  return EXIT_SUCCESS;
}
