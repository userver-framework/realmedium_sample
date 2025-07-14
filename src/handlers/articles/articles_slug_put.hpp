#pragma once

#include <userver/storages/postgres/cluster.hpp>

#include "handlers/common.hpp"

namespace real_medium::handlers::articles_slug::put {

class Handler final : public Common {
public:
    static constexpr std::string_view kName{"handler-update-article"};

    using Common::Common;

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value&,
        userver::server::request::RequestContext& request_context
    ) const override final;
};

}  // namespace real_medium::handlers::articles_slug::put
