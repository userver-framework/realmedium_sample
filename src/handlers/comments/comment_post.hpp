
#pragma once

#include <fmt/format.h>
#include <string>
#include <string_view>

#include "handlers/common.hpp"

namespace real_medium::handlers::comments::post {

class Handler final : public Common {
public:
    static constexpr std::string_view kName = "handler-comment-post";

    using Common::Common;

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& context
    ) const override;
};

}  // namespace real_medium::handlers::comments::post
