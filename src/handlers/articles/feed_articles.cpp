#include "feed_articles.hpp"
#include <docs/api/api.hpp>
#include <sstream>
#include <userver/formats/serialize/common_containers.hpp>
#include "db/sql.hpp"
#include "dto/article.hpp"
#include "dto/filter.hpp"
#include "models/article.hpp"
#include "utils/errors.hpp"

namespace real_medium::handlers::articles::feed::get {

userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& /*request_json*/,
    userver::server::request::RequestContext& context
) const {
    handlers::FeedArticleFilterDTO filter;

    try {
        filter = dto::Parse<handlers::FeedArticleFilterDTO>(request);
    } catch (std::bad_cast& ex) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
        return utils::error::MakeError("filters", "invalid filters entered");
    }

    auto user_id = context.GetData<std::optional<std::string>>("id");
    auto data = GetArticlesCache().Get();
    auto articles = data->getFeed(filter, user_id.value());
    userver::formats::json::ValueBuilder builder;
    builder["articles"] = userver::formats::common::Type::kArray;
    for (auto& article : articles) builder["articles"].PushBack(dto::Article::Parse(*article, user_id));
    builder["articlesCount"] = articles.size();
    return builder.ExtractValue();
}

}  // namespace real_medium::handlers::articles::feed::get
