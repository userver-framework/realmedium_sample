WITH profile AS (
  SELECT * FROM real_medium.users WHERE user_id = $1
), following AS (
  DELETE FROM real_medium.followers WHERE followed_user_id = $1 AND follower_user_id = $2
  RETURNING *
)
SELECT
  profile.username,
  profile.bio,
  profile.image,
  CASE WHEN EXISTS (SELECT 1 FROM following) THEN FALSE ELSE TRUE END
FROM profile
