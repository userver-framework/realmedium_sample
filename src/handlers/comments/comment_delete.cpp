#include "comment_delete.hpp"

#include <userver/utils/from_string.hpp>

#include "db/sql.hpp"
#include "utils/make_error.hpp"

namespace real_medium::handlers::comments::del {

userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& /*request_json*/,
    userver::server::request::RequestContext& context
) const {
    auto user_id = context.GetData<std::optional<std::string>>("id");
    const auto& comment_id = userver::utils::FromString<int, std::string>(request.GetPathArg("id"));
    const auto& slug = request.GetPathArg("slug");

    const auto result_find_comment = GetPg().Execute(
        userver::storages::postgres::ClusterHostType::kMaster, sql::kFindCommentByIdAndSlug, comment_id, slug
    );

    if (result_find_comment.IsEmpty()) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kNotFound);
        return utils::error::MakeError("comment_id", "Invalid comment_id.");
    }

    const auto result_delete_comment = GetPg().Execute(
        userver::storages::postgres::ClusterHostType::kMaster, sql::kDeleteCommentById, comment_id, user_id
    );

    if (result_delete_comment.IsEmpty()) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kForbidden);
        return utils::error::MakeError("user_id", "This user does not own this comment.");
    }

    return {};
}

}  // namespace real_medium::handlers::comments::del
