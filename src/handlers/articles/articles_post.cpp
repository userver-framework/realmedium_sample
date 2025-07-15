#include "articles_post.hpp"
#include <docs/api/api.hpp>
#include <userver/logging/log.hpp>

#include "db/sql.hpp"
#include "models/article.hpp"
#include "utils/errors.hpp"
#include "utils/slugify.hpp"
#include "validators/validators.hpp"

namespace real_medium::handlers::articles::post {

userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext& context
) const {
    auto create_article_request = request_json["article"].As<handlers::CreateArticleRequest>();
    try {
        validator::validate(create_article_request);
    } catch (const real_medium::utils::error::ValidationException& ex) {
        // userver doesn't yet support 422 HTTP error code, so we handle the
        // exception by ourselves. In general the exceptions are processed by the
        // framework
        request.SetResponseStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
        return ex.GetDetails();
    }

    const auto user_id = context.GetData<std::optional<std::string>>("id");

    std::string article_id;
    try {
        const auto slug = real_medium::utils::slug::Slugify(create_article_request.title.value());

        const auto res = GetPg().Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            real_medium::sql::kCreateArticle,
            create_article_request.title,
            slug,
            create_article_request.body,
            create_article_request.description,
            user_id,
            create_article_request.tags
        );

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
}  // namespace real_medium::handlers::articles::post
