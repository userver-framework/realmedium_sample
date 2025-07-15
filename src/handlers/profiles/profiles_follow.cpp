#include <docs/api/api.hpp>

#include "db/sql.hpp"
#include "models/profile.hpp"
#include "profiles_follow.hpp"
#include "utils/make_error.hpp"

#include <userver/formats/yaml/value_builder.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

using namespace userver::formats;
using namespace userver::server::http;
using namespace userver::server::request;
using namespace userver::storages::postgres;

namespace real_medium::handlers::profiles::post {

userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value&,
    userver::server::request::RequestContext& context
) const {
    auto user_id = context.GetData<std::optional<std::string>>("id");
    const auto& username = request.GetPathArg("username");
    if (username.empty()) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kNotFound);
        return utils::error::MakeError("username", "It is null.");
    }

    const auto res_find_id_username =
        GetPg().Execute(userver::storages::postgres::ClusterHostType::kSlave, sql::kFindUserIdByUsername, username);
    if (res_find_id_username.IsEmpty()) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kNotFound);
        return utils::error::MakeError("username", "There is no user with this nickname.");
    }

    std::string username_id = res_find_id_username.AsSingleRow<std::string>();

    if (username_id == user_id) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return utils::error::MakeError("username", "Username is author of the request.");
    }

    const auto res_following = GetPg().Execute(
        userver::storages::postgres::ClusterHostType::kSlave, sql::kFollowingUser, username_id, user_id
    );

    const auto profile = res_following.AsSingleRow<handlers::Profile>(userver::storages::postgres::kRowTag);

    if (!profile.following) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return utils::error::MakeError("user_id", "has already followed");
    }

    userver::formats::json::ValueBuilder builder;
    builder["profile"] = profile;

    return builder.ExtractValue();
}
}  // namespace real_medium::handlers::profiles::post
