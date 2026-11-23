#include "user.hpp"

#include <gtest/gtest.h>

namespace real_medium::models {

TEST(User, SerializeDoesNotIssueToken) {
    User user{};
    user.id = "user-1";
    const auto json = userver::formats::json::ValueBuilder{user}.ExtractValue();
    EXPECT_FALSE(json.HasMember("token"));
}

}  // namespace real_medium::models
