#pragma once

#include <userver/storages/postgres/cluster.hpp>

#include "handlers/common.hpp"

namespace real_medium::handlers::articles::feed::get {

class Handler final : public Common {
public:
    static constexpr std::string_view kName = "handler-feed-articles";
    using Common::Common;
    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& context
    ) const override;
};

}  // namespace real_medium::handlers::articles::feed::get
