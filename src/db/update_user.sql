UPDATE real_medium.users SET
  username = COALESCE($2, username),
  email = COALESCE($3, email),
  bio = COALESCE($4, bio),
  image = COALESCE($5, image),
  password_hash = COALESCE($6, password_hash),
  salt = COALESCE($7, salt)
WHERE user_id = $1
RETURNING *
