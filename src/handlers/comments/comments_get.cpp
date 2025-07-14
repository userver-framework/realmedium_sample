#include "comments_get.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "db/sql.hpp"
#include "dto/comment.hpp"
#include "utils/make_error.hpp"

namespace real_medium::handlers::comments::get {

Handler::Handler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : Common(config, component_context),
      comments_cache_(component_context.FindComponent<real_medium::cache::comments_cache::CommentsCache>()) {}

userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& /*request_json*/,
    userver::server::request::RequestContext& context
) const {
    auto user_id = context.GetData<std::optional<std::string>>("id");
    const auto& slug = request.GetPathArg("slug");
    const auto articles_data = GetArticlesCache().Get();
    userver::formats::json::ValueBuilder result = userver::formats::json::MakeObject();
    if (!articles_data->findArticleBySlug(slug)) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kNotFound);
        return utils::error::MakeError("slug", "Invalid slug");
    }
    const auto comments_data = comments_cache_.Get();
    const auto res_find_comments = comments_data->findComments(slug);

    userver::formats::json::ValueBuilder builder;
    builder["comments"] = userver::formats::common::Type::kArray;
    for (auto& comment : res_find_comments) builder["comments"].PushBack(dto::Comment::Parse(*comment.second, user_id));

    return builder.ExtractValue();
}

}  // namespace real_medium::handlers::comments::get
