#pragma once

#include <fmt/format.h>
#include <string_view>

#include <userver/server/handlers/http_handler_json_base.hpp>

#include <userver/storages/postgres/postgres_fwd.hpp>
#include <userver/components/component_fwd.hpp>

#include "cache/articles_cache.hpp"

namespace real_medium::handlers {

class Common : public userver::server::handlers::HttpHandlerJsonBase {
 public:
  Common(const userver::components::ComponentConfig& config,
          const userver::components::ComponentContext& component_context);

 protected:
  userver::storages::postgres::ClusterPtr pg_cluster_;
  const real_medium::cache::articles_cache::ArticlesCache& cache_;
};

}  // namespace real_medium::handlers
