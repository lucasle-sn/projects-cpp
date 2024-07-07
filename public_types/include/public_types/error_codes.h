#ifndef PUBLIC_TYPES_ERROR_CODES_H
#define PUBLIC_TYPES_ERROR_CODES_H

namespace qle {

/**
 * @brief ErrorCode
 */
enum class ErrorCode {
  SUCCESS,  ///< Success
  ERROR,    ///< Error
  INVALID,  ///< Invalid
};

class ErrorHandler {
 public:
  /**
   * @brief Convert ErrorCode to string
   *
   * @param code ErrorCode
   * @return const char *
   */
  static const char *error_code_to_string(ErrorCode code) {
    switch (code) {
      case ErrorCode::SUCCESS:
        return "Success";
      case ErrorCode::ERROR:
        return "Error";
      case ErrorCode::INVALID:
      default:
        return "Invalid";
    }
  }
};

}  // namespace qle

#endif  // PUBLIC_TYPES_ERROR_CODES_H
