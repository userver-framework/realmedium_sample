SELECT * FROM real_medium.users
WHERE email = $1 AND password_hash = $2
