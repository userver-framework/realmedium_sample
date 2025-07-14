#include "profiles.hpp"
#include <docs/api/api.hpp>
#include <string>
#include "db/sql.hpp"
#include "models/profile.hpp"
#include "utils/make_error.hpp"

#include <userver/formats/yaml/value_builder.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

using namespace userver::formats;
using namespace userver::server::http;
using namespace userver::server::request;
using namespace userver::storages::postgres;

namespace real_medium::handlers::profiles::get {

json::Value Handler::HandleRequestJsonThrow(const HttpRequest& request, const json::Value&, RequestContext& context)
    const {
    auto user_id = context.GetData<std::optional<std::string>>("id");
    const auto& username = request.GetPathArg("username");
    auto res = GetPg().Execute(ClusterHostType::kMaster, sql::kGetProfileByUsername.c_str(), username, user_id);
    if (res.IsEmpty()) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kNotFound);
        return utils::error::MakeError("username", "There is no user with this nickname.");
    }

    auto profile = res.AsSingleRow<handlers::Profile>(userver::storages::postgres::kRowTag);

    userver::formats::json::ValueBuilder builder;
    builder["profile"] = profile;

    return builder.ExtractValue();
}

}  // namespace real_medium::handlers::profiles::get
