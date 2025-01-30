#pragma once

#include <optional>
#include <string>
#include <tuple>

#include <userver/formats/json.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/server/http/http_request.hpp>
namespace real_medium::dto {

template <typename T>
T Parse(const userver::server::http::HttpRequest& request);
}  // namespace real_medium::dto
