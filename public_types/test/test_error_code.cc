#include <public_types/error_codes.h>

#include <gtest/gtest.h>

using ErrorCode = qle::ErrorCode;
using ErrorCodeHandler = qle::ErrorCodeHandler;

namespace {

class TestErrorCode : public ::testing::Test {};

TEST_F(TestErrorCode, TestToString) {
  EXPECT_TRUE(strcmp(ErrorCodeHandler::error_code_to_string(ErrorCode::SUCCESS),
                     "Success") == 0);
  EXPECT_TRUE(strcmp(ErrorCodeHandler::error_code_to_string(ErrorCode::ERROR),
                     "Error") == 0);
  EXPECT_TRUE(strcmp(ErrorCodeHandler::error_code_to_string(ErrorCode::INVALID),
                     "Invalid") == 0);
}

}  // namespace
