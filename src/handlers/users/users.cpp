#include <string>

#include "users.hpp"

#include <docs/api/api.hpp>
#include <userver/crypto/hash.hpp>

#include "db/sql.hpp"
#include "models/user.hpp"
#include "utils/errors.hpp"
#include "utils/make_error.hpp"
#include "utils/random.hpp"
#include "validators/validators.hpp"

namespace real_medium::handlers::users::post {

userver::formats::json::Value RegisterUser::
    HandleRequestJsonThrow(const userver::server::http::HttpRequest& request, const userver::formats::json::Value& request_json, userver::server::request::RequestContext&)
        const {
    handlers::UserRegistrationDTO user_register = request_json["user"].As<handlers::UserRegistrationDTO>();
    ;

    try {
        validator::validate(user_register);
    } catch (const utils::error::ValidationException& err) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
        return err.GetDetails();
    }

    auto salt = utils::random::GenerateSalt();
    auto hash_password = userver::crypto::hash::Sha256(user_register.password.value() + salt);
    models::User result_user;
    try {
        auto query_result = GetPg().Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            sql::kInsertUser.c_str(),
            user_register.username,
            user_register.email,
            user_register.bio,
            user_register.image,
            hash_password,
            salt
        );
        result_user = query_result.AsSingleRow<models::User>(userver::storages::postgres::kRowTag);
    } catch (const userver::storages::postgres::UniqueViolation& ex) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
        return utils::error::MakeError(ex.GetServerMessage().GetConstraint(), ex.GetServerMessage().GetDetail());
    }

    userver::formats::json::ValueBuilder builder;
    builder["user"] = result_user;
    return builder.ExtractValue();
}

}  // namespace real_medium::handlers::users::post
