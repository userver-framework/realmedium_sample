#include "users_login.hpp"

#include <docs/api/api.hpp>
#include <userver/crypto/hash.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

#include "db/sql.hpp"
#include "handlers/common.hpp"
#include "models/user.hpp"
#include "utils/errors.hpp"
#include "validators/validators.hpp"

namespace real_medium::handlers::users_login::post {

namespace {

class LoginUser final : public Common {
public:
    static constexpr std::string_view kName = "handler-login-user";

    using Common::Common;

    userver::formats::json::Value
    HandleRequestJsonThrow(const userver::server::http::HttpRequest& request, const userver::formats::json::Value& request_json, userver::server::request::RequestContext&)
        const override {
        auto&& user_login = request_json["user"].As<UserLoginDTO>();

        try {
            validator::validate(user_login);
        } catch (const utils::error::ValidationException& err) {
            request.SetResponseStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
            return err.GetDetails();
        }

        auto salt = GetPg().Execute(
            userver::storages::postgres::ClusterHostType::kMaster, sql::kGetSaltByEmail, user_login.email
        );
        if (salt.IsEmpty()) {
            auto& response = request.GetHttpResponse();
            response.SetStatus(userver::server::http::HttpStatus::kNotFound);
            return {};
        }
        auto password_hash =
            userver::crypto::hash::Sha256(user_login.password.value() + salt.AsSingleRow<std::string>());

        auto user_result = GetPg().Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            sql::kSelectUserByEmailAndPassword,
            user_login.email,
            password_hash
        );

        if (user_result.IsEmpty()) {
            auto& response = request.GetHttpResponse();
            response.SetStatus(userver::server::http::HttpStatus::kNotFound);
            return {};
        }

        auto user = user_result.AsSingleRow<models::User>(userver::storages::postgres::kRowTag);

        userver::formats::json::ValueBuilder response;
        response["user"] = user;

        return response.ExtractValue();
    }
};

}  // namespace

void AppendLoginUser(userver::components::ComponentList& component_list) { component_list.Append<LoginUser>(); }

}  // namespace real_medium::handlers::users_login::post
