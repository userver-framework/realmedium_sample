#include "articles_cache.hpp"

#include <userver/utils/numeric_cast.hpp>

namespace real_medium::cache::articles_cache {

userver::storages::postgres::Query ArticlesCachePolicy::kQuery = real_medium::sql::kSelectFullArticleInfo;

void ArticlesCacheContainer::insert_or_assign(Key&& key, Article&& article) {
    auto article_ptr = std::make_shared<const Article>(std::move(article));
    auto old_value = article_by_key_.find(key);
    if (old_value != article_by_key_.end()) {
        article_by_slug_.erase(old_value->second->slug);
        for (const auto& oldFollower : old_value->second->authorFollowedByUsersIds)
            if (article_ptr->authorFollowedByUsersIds.find(oldFollower) == article_ptr->authorFollowedByUsersIds.end())
                articles_by_follower_[oldFollower].erase(article_ptr->articleId);
    }

    article_by_key_.insert_or_assign(key, article_ptr);
    article_by_slug_.insert_or_assign(article_ptr->slug, article_ptr);
    for (const auto& follower : article_ptr->authorFollowedByUsersIds)
        articles_by_follower_[follower].insert_or_assign(article_ptr->articleId, article_ptr);
    recent_articles_.insert_or_assign({article_ptr->createdAt, article_ptr->articleId}, article_ptr);
}

size_t ArticlesCacheContainer::size() const { return article_by_key_.size(); }

ArticlesCacheContainer::ArticlePtr ArticlesCacheContainer::findArticleBySlug(const Slug& slug) const {
    auto it = article_by_slug_.find(slug);
    if (it == article_by_slug_.end()) return nullptr;
    return it->second;
}

std::vector<ArticlesCacheContainer::ArticlePtr> ArticlesCacheContainer::getRecent(
    real_medium::handlers::ArticleFilterDTO& filter
) const {
    std::vector<ArticlePtr> articles;
    int offset = 0;
    for (const auto& it : recent_articles_) {
        if (filter.limit && articles.size() >= userver::utils::numeric_cast<std::size_t>(filter.limit)) break;

        const auto& tags = it.second->tags;
        if (filter.tag && it.second->tags.find(filter.tag.value()) == tags.end()) continue;

        if (filter.author && it.second->authorInfo.username != filter.author) continue;

        const auto& favorited = it.second->articleFavoritedByUsernames;
        if (filter.favorited && favorited.find(filter.favorited.value()) == favorited.end()) continue;

        if (filter.offset && offset < filter.offset) {
            ++offset;
            continue;
        }
        articles.push_back(it.second);
    }
    return articles;
}
std::vector<ArticlesCacheContainer::ArticlePtr>
ArticlesCacheContainer::getFeed(real_medium::handlers::FeedArticleFilterDTO& filter, UserId auth_id) const {
    auto followed_articles_umap = articles_by_follower_.find(auth_id);
    if (followed_articles_umap == articles_by_follower_.end()) return {};

    RecentArticlesMap followedArticlesOrdered;
    for (const auto& it : followed_articles_umap->second)
        followedArticlesOrdered.insert_or_assign({it.second->createdAt, it.second->articleId}, it.second);

    std::vector<ArticlePtr> articles;

    int offset = 0;
    for (const auto& it : followedArticlesOrdered) {
        if (filter.limit && articles.size() >= userver::utils::numeric_cast<std::size_t>(filter.limit)) break;
        if (filter.offset && offset < filter.offset) {
            ++offset;
            continue;
        }
        articles.push_back(it.second);
    }
    return articles;
}

}  // namespace real_medium::cache::articles_cache
