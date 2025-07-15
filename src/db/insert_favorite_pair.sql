WITH tmp(article_id, user_id) AS (
    SELECT article_id, $1 FROM real_medium.articles WHERE slug=$2
)
INSERT INTO real_medium.favorites(article_id, user_id) (SELECT article_id, user_id FROM tmp)
ON CONFLICT DO NOTHING
RETURNING article_id
