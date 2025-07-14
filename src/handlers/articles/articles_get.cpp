#include "articles_get.hpp"
#include <docs/api/api.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include "db/sql.hpp"
#include "dto/article.hpp"
#include "dto/filter.hpp"
#include "models/article.hpp"
#include "utils/make_error.hpp"

namespace real_medium::handlers::articles::get {

userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& /*request_json*/,
    userver::server::request::RequestContext& context
) const {
    handlers::ArticleFilterDTO filter;

    try {
        filter = dto::Parse<handlers::ArticleFilterDTO>(request);
    } catch (std::bad_cast& ex) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
        return utils::error::MakeError("filters", "invalid filters entered");
    }

    auto user_id = context.GetData<std::optional<std::string>>("id");
    auto data = GetArticlesCache().Get();
    auto recent_articles = data->getRecent(filter);
    userver::formats::json::ValueBuilder builder;
    builder["articles"] = userver::formats::common::Type::kArray;
    for (auto& article : recent_articles) builder["articles"].PushBack(dto::Article::Parse(*article, user_id));
    builder["articlesCount"] = recent_articles.size();
    return builder.ExtractValue();
}

}  // namespace real_medium::handlers::articles::get
