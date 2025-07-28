WITH article AS (
  SELECT article_id FROM real_medium.articles WHERE slug = $2
)
SELECT * FROM real_medium.comments
JOIN article ON article.article_id = real_medium.comments.article_id
WHERE comment_id = $1
