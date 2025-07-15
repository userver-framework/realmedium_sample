#include "articles_slug_delete.hpp"
#include "db/sql.hpp"
#include "dto/article.hpp"
#include "models/article.hpp"
#include "utils/slugify.hpp"

namespace real_medium::handlers::articles_slug::del {

userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value&,
    userver::server::request::RequestContext& context
) const {
    const auto& slug = request.GetPathArg("slug");
    const auto user_id = context.GetData<std::optional<std::string>>("id");
    auto res = GetPg().Execute(
        userver::storages::postgres::ClusterHostType::kMaster, real_medium::sql::kGetArticleIdBySlug, slug
    );
    if (res.IsEmpty()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        return {};
    }
    res = GetPg().Execute(
        userver::storages::postgres::ClusterHostType::kMaster, real_medium::sql::kDeleteArticleBySlug, slug, user_id
    );

    if (res.IsEmpty()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kForbidden);
        return {};
    }

    return {};
}

}  // namespace real_medium::handlers::articles_slug::del
