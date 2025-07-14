#pragma once

#include <docs/api/api.hpp>
#include <string>
#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/io/chrono.hpp>
#include "models/article.hpp"

namespace real_medium::dto {
struct Article final {
    static Article Parse(const models::TaggedArticleWithProfile& model);
    static Article Parse(const models::FullArticleInfo& info, std::optional<std::string> authUserId);
    std::string slug;
    std::string title;
    std::string body;
    std::string description;
    std::optional<std::vector<std::string>> tags;
    userver::storages::postgres::TimePointTz createdAt;
    userver::storages::postgres::TimePointTz updatedAt;
    std::int64_t favoritesCount{};
    bool isFavorited{false};
    handlers::Profile profile;
};

userver::formats::json::Value
Serialize(const Article& data, userver::formats::serialize::To<userver::formats::json::Value>);
}  // namespace real_medium::dto
