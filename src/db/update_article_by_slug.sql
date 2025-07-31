UPDATE real_medium.articles
SET
        title = COALESCE($3, title),
        slug = COALESCE($4, slug),
        description = COALESCE($5, description),
        body = COALESCE($6, body),
        updated_at = NOW()
WHERE slug = $1 AND user_id = $2
RETURNING article_id
