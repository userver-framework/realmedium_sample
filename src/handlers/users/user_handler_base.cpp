#include "user_handler_base.hpp"

#include <userver/components/component_context.hpp>
#include <userver/formats/json/value_builder.hpp>

#include "auth/jwt.hpp"
#include "models/user.hpp"

namespace real_medium::handlers {

UserHandlerBase::UserHandlerBase(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : Common(config, context), jwt_(context.FindComponent<auth::Jwt>()) {}

userver::formats::json::Value UserHandlerBase::MakeUserResponseWithToken(const models::User& user) const {
    userver::formats::json::ValueBuilder response;
    response["user"] = user;
    response["user"]["token"] = jwt_.MakeToken(user.id);
    return response.ExtractValue();
}

}  // namespace real_medium::handlers
