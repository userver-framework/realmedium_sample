#pragma once

#include <docs/api/api.hpp>
#include <memory>
#include <unordered_map>

#include <userver/cache/base_postgres_cache.hpp>
#include <userver/storages/postgres/io/chrono.hpp>

#include "db/sql.hpp"
#include "dto/filter.hpp"
#include "models/article.hpp"

namespace real_medium::cache::articles_cache {

class ArticlesCacheContainer {
    using Timepoint = userver::storages::postgres::TimePointTz;
    using Slug = std::string;
    using UserId = std::string;

public:
    using Key = real_medium::models::ArticleId;
    using Article = real_medium::models::FullArticleInfo;
    using ArticlePtr = std::shared_ptr<const Article>;
    using AuthorName = std::string;
    void insert_or_assign(Key&& key, Article&& config);
    size_t size() const;

    ArticlePtr findArticleBySlug(const Slug& slug) const;
    std::vector<ArticlePtr> getRecent(real_medium::handlers::ArticleFilterDTO& filter_) const;
    std::vector<ArticlePtr> getFeed(real_medium::handlers::FeedArticleFilterDTO& filter_, UserId authId_) const;

private:
    struct TimepointedArticle {
        Timepoint created;
        Key articleId;
        bool operator<(const TimepointedArticle& other) const {
            return created != other.created ? created < other.created : articleId < other.articleId;
        }
        bool operator==(const TimepointedArticle& other) const {
            return created == other.created && articleId == other.articleId;
        }
        bool operator>(const TimepointedArticle& other) const { return !(*this == other) && !(*this < other); }
    };

    using RecentArticlesMap = std::map<TimepointedArticle /*created_at*/, ArticlePtr, std::greater<TimepointedArticle>>;
    std::unordered_map<Key, ArticlePtr> article_by_key_;
    std::unordered_map<Slug, ArticlePtr> article_by_slug_;
    std::unordered_map<UserId /*follower*/, std::unordered_map<Key, ArticlePtr>> articles_by_follower_;
    RecentArticlesMap recent_articles_;
};

struct ArticlesCachePolicy {
    static constexpr auto kName = "articles-cache";
    using ValueType = ArticlesCacheContainer::Article;
    using CacheContainer = ArticlesCacheContainer;
    static constexpr auto kKeyMember = &ValueType::articleId;
    static userver::storages::postgres::Query kQuery;
    static constexpr auto kUpdatedField = "updated_at";
    using UpdatedFieldType = userver::storages::postgres::TimePointTz;
};

using ArticlesCache = ::userver::components::PostgreCache<ArticlesCachePolicy>;

}  // namespace real_medium::cache::articles_cache
