#include <docs/api/api.hpp>

#include "articles_slug_put.hpp"
#include "db/sql.hpp"
#include "models/article.hpp"
#include "utils/errors.hpp"
#include "utils/slugify.hpp"
#include "validators/validators.hpp"

namespace real_medium::handlers::articles_slug::put {

userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext& context
) const {
    auto slug = request.GetPathArg("slug");
    auto update_request = request_json["article"].As<handlers::UpdateArticleRequest>();
    try {
        validator::validate(update_request);
    } catch (const real_medium::utils::error::ValidationException& ex) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
        return ex.GetDetails();
    }
    auto user_id = context.GetData<std::optional<std::string>>("id");

    std::string article_id;
    try {
        const auto new_slug =
            update_request.title
                ? std::make_optional<std::string>(real_medium::utils::slug::Slugify(*update_request.title))
                : std::nullopt;
        const auto res = GetPg().Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            real_medium::sql::kUpdateArticleBySlug,
            slug,
            user_id,
            update_request.title,
            new_slug,
            update_request.description,
            update_request.body
        );
        if (res.IsEmpty()) {
            request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
            return {};
        }
        article_id = res.AsSingleRow<std::string>();
    } catch (const userver::storages::postgres::UniqueViolation& ex) {
        const auto constraint = ex.GetServerMessage().GetConstraint();
        if (constraint == "uniq_slug") {
            request.SetResponseStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
            return real_medium::utils::error::MakeError("slug", "already exists");
        }
        throw;
    }
    const auto res = GetPg().Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        real_medium::sql::kGetArticleWithAuthorProfile,
        article_id,
        user_id
    );

    userver::formats::json::ValueBuilder builder;
    builder["article"] = res.AsSingleRow<real_medium::models::TaggedArticleWithProfile>();
    return builder.ExtractValue();
}
}  // namespace real_medium::handlers::articles_slug::put
