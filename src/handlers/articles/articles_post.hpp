#pragma once

#include <userver/storages/postgres/cluster.hpp>

#include "handlers/common.hpp"

namespace real_medium::handlers::articles::post {

class Handler final : public Common {
public:
    static constexpr std::string_view kName{"handler-create-article"};

    using Common::Common;

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value&,
        userver::server::request::RequestContext& request_context
    ) const override;
};

}  // namespace real_medium::handlers::articles::post
