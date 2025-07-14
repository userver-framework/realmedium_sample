#pragma once

#include "handlers/common.hpp"

namespace real_medium::handlers::profiles::get {

class Handler final : public Common {
public:
    static constexpr std::string_view kName = "handler-get-profile";

    using Common::Common;

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value&,
        userver::server::request::RequestContext& context
    ) const override;
};

}  // namespace real_medium::handlers::profiles::get
