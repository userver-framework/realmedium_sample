#pragma once

#include <userver/formats/json/serialize_container.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "handlers/common.hpp"

namespace real_medium::handlers::articles_favorite::post {

class Handler final : public Common {
public:
    static constexpr std::string_view kName = "handler-articles-favorite-post";

    using Common::Common;

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value&,
        userver::server::request::RequestContext& context
    ) const override;
};

}  // namespace real_medium::handlers::articles_favorite::post
