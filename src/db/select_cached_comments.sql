SELECT c.comment_id,
       c.created_at AS createdAt,
       c.updated_at AS updatedAt,
       c.body,
       c.user_id,
       a.slug,
       ROW(u.user_id, u.username, u.email, u.bio, u.image, u.password_hash, u.salt)::real_medium.user AS author_info,
            ARRAY(
    SELECT follower_user_id
    FROM real_medium.followers fl
    WHERE fl.followed_user_id = c.user_id
    ) AS following
FROM real_medium.comments c
JOIN real_medium.articles a ON a.article_id = c.article_id
JOIN real_medium.users u ON c.user_id = u.user_id
