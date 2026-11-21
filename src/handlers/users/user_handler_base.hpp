#pragma once

#include "handlers/common.hpp"

namespace real_medium::auth {
class Jwt;
}

namespace real_medium::models {
struct User;
}

namespace real_medium::handlers {

class UserHandlerBase : public Common {
public:
    UserHandlerBase(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );

protected:
    userver::formats::json::Value MakeUserResponseWithToken(const models::User& user) const;

private:
    const auth::Jwt& jwt_;
};

}  // namespace real_medium::handlers
