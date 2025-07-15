WITH comments AS (
  SELECT * FROM real_medium.comments WHERE article_id = $1
)
SELECT
    comments.comment_id,
    comments.created_at,
    comments.updated_at,
    comments.body,
    (
        SELECT
            ROW(users.username, users.bio, users.image,
            CASE WHEN EXISTS (SELECT 1 FROM real_medium.followers
            WHERE followers.followed_user_id = comments.user_id AND followers.follower_user_id = $2)
            THEN true ELSE false END)::real_medium.profile
        FROM real_medium.users
        WHERE user_id = comments.user_id
    ) AS author
FROM comments
