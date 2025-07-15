WITH profile AS (
  SELECT * FROM real_medium.users WHERE username = $1
)
SELECT profile.username, profile.bio, profile.image,
       CASE WHEN EXISTS (
         SELECT 1 FROM real_medium.followers
         WHERE followed_user_id = profile.user_id AND follower_user_id = $2
       ) THEN true ELSE false END AS following
FROM profile
