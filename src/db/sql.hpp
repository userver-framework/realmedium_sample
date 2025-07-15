#pragma once

#include <userver/utils/zstring_view.hpp>

// Codegenerated from *.sql files
#include <real_medium/sql_queries.hpp>

namespace real_medium::sql {

inline constexpr userver::utils::zstring_view kCreateArticle{R"~(
SELECT real_medium.create_article($1, $2, $3, $4, $5, $6)
)~"};

inline constexpr userver::utils::zstring_view kGetArticleWithAuthorProfile{R"~(
SELECT real_medium.get_article_with_author_profile($1, $2)
)~"};

inline constexpr userver::utils::zstring_view kGetArticleWithAuthorProfileBySlug{R"~(
SELECT real_medium.get_article_with_author_profile_by_slug($1, $2)
)~"};

inline constexpr userver::utils::zstring_view kInsertFavoritePair = R"~(
WITH tmp(article_id, user_id) AS (
    SELECT article_id, $1 FROM real_medium.articles WHERE slug=$2
)
INSERT INTO real_medium.favorites(article_id, user_id) (SELECT article_id, user_id FROM tmp)
ON CONFLICT DO NOTHING
RETURNING article_id
)~";

inline constexpr userver::utils::zstring_view kIncrementFavoritesCount = R"~(
UPDATE real_medium.articles
SET favorites_count=favorites_count + 1
WHERE article_id=$1
)~";

inline constexpr userver::utils::zstring_view kDeleteFavoritePair = R"~(
WITH tmp(article_id, user_id) AS (
    SELECT article_id, $1 FROM real_medium.articles WHERE slug=$2
)
DELETE FROM real_medium.favorites
WHERE (article_id, user_id) IN (SELECT article_id, user_id FROM tmp)
RETURNING article_id
)~";

inline constexpr userver::utils::zstring_view kDecrementFavoritesCount = R"~(
UPDATE real_medium.articles
SET favorites_count=favorites_count - 1
WHERE article_id=$1
)~";

inline constexpr userver::utils::zstring_view kFindArticlesByFollowedUsers = R"~(
SELECT real_medium.get_feed_articles($1, $2, $3)
)~";

inline constexpr userver::utils::zstring_view kGetArticleIdBySlug{R"~(
SELECT real_medium.get_article_id_by_slug($1)
)~"};

inline constexpr userver::utils::zstring_view kUpdateArticleBySlug{R"~(
SELECT real_medium.update_article_by_slug($1, $2, $3, $4, $5, $6)
)~"};

inline constexpr userver::utils::zstring_view kDeleteArticleBySlug{R"~(
SELECT real_medium.delete_article_by_slug($1, $2)
)~"};

inline constexpr userver::utils::zstring_view kFindArticlesByFilters{R"~(
SELECT real_medium.get_articles_by_filters($1, $2, $3, $4, $5, $6)
)~"};

}  // namespace real_medium::sql
