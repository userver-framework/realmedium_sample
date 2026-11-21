#pragma once

#include "handlers/users/user_handler_base.hpp"

namespace real_medium::handlers::users::put {

class Handler final : public UserHandlerBase {
public:
    static constexpr std::string_view kName = "handler-user-put";

    using UserHandlerBase::UserHandlerBase;

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& context
    ) const override;
};

}  // namespace real_medium::handlers::users::put
