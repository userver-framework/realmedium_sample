UPDATE real_medium.articles
SET favorites_count=favorites_count - 1
WHERE article_id=$1
