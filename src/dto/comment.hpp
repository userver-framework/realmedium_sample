#pragma once

#include <string>
#include <tuple>

#include <docs/api/api.hpp>
#include <userver/formats/json.hpp>
#include <userver/formats/parse/common_containers.hpp>

#include "models/comment.hpp"

namespace real_medium::dto {

struct Comment final {
    static Comment Parse(const real_medium::models::CachedComment& cachedComment, std::optional<std::string> userId);
    int32_t id;
    userver::storages::postgres::TimePointTz createdAt;
    userver::storages::postgres::TimePointTz updatedAt;
    std::string body;
    handlers::Profile author;
};

userver::formats::json::Value
Serialize(const Comment& comment, userver::formats::serialize::To<userver::formats::json::Value>);

}  // namespace real_medium::dto
