#include "user_put.hpp"
#include <docs/api/api.hpp>
#include <userver/crypto/hash.hpp>
#include "db/sql.hpp"
#include "models/user.hpp"
#include "utils/errors.hpp"
#include "utils/random.hpp"
#include "validators/validators.hpp"

namespace real_medium::handlers::users::put {

userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext& context
) const {
    auto user_id = context.GetData<std::optional<std::string>>("id");

    handlers::UserUpdateDTO user_change_data = request_json["user"].As<handlers::UserUpdateDTO>();

    try {
        validator::validate(user_change_data);
    } catch (const utils::error::ValidationException& err) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
        return err.GetDetails();
    }

    std::optional<std::string> password_hash = std::nullopt;
    std::optional<std::string> salt = std::nullopt;
    if (user_change_data.password) {
        salt = utils::random::GenerateSalt();
        password_hash = userver::crypto::hash::Sha256(user_change_data.password.value() + salt.value());
    }

    const auto result = GetPg().Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        sql::kUpdateUser,
        user_id,
        user_change_data.username,
        user_change_data.email,
        user_change_data.bio,
        user_change_data.image,
        password_hash,
        salt
    );

    auto user_result_data = result.AsSingleRow<real_medium::models::User>(userver::storages::postgres::kRowTag);

    userver::formats::json::ValueBuilder builder;
    builder["user"] = user_result_data;

    return builder.ExtractValue();
}

}  // namespace real_medium::handlers::users::put
