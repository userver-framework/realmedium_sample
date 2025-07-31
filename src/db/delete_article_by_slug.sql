DELETE FROM real_medium.articles
WHERE slug = $1 AND user_id = $2
RETURNING article_id
