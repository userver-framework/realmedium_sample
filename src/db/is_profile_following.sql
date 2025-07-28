RETURN EXISTS (SELECT 1 FROM real_medium.followers WHERE follower_user_id = $1 AND followed_user_id = $2)
