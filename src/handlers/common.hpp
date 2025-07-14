#pragma once

#include <fmt/format.h>
#include <string_view>

#include <userver/components/component_fwd.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/postgres_fwd.hpp>
#include <userver/utils/assert.hpp>

#include "cache/articles_cache.hpp"

namespace real_medium::handlers {

class Common : public userver::server::handlers::HttpHandlerJsonBase {
public:
    Common(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& component_context
    );

    const real_medium::cache::articles_cache::ArticlesCache& GetArticlesCache() const noexcept {
        return articles_cache_;
    }

    userver::storages::postgres::Cluster& GetPg() const noexcept {
        UASSERT(pg_cluster_);
        return *pg_cluster_;
    }

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
    const real_medium::cache::articles_cache::ArticlesCache& articles_cache_;
};

}  // namespace real_medium::handlers
