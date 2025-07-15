SELECT a.article_id AS articleId,
       a.title,
       a.slug,
       a.description,
       a.body,
       ARRAY(
              SELECT tag_name
              FROM real_medium.tag_list tl
              JOIN real_medium.article_tag at ON tl.tag_id = at.tag_id
              WHERE at.article_id = a.article_id
       ) AS tags,
       a.created_at AS createdAt,
       a.updated_at AS updatedAt,
       ARRAY(
              SELECT user_id
              FROM real_medium.favorites f
              WHERE f.article_id = a.article_id
       ) AS article_favorited_by_user_ids,

       ARRAY(
              SELECT u.username
              FROM real_medium.favorites f
              JOIN real_medium.users u ON f.user_id = u.user_id
              WHERE f.article_id = a.article_id
       ) AS article_favorited_by_usernames,
       ARRAY(
              SELECT follower_user_id
              FROM real_medium.followers fl
              WHERE fl.followed_user_id = a.user_id
       ) AS author_followed_by_user_ids,
       ROW(u.user_id, u.username, u.email, u.bio, u.image, u.password_hash, u.salt)::real_medium.user AS author_info
FROM real_medium.articles a
JOIN real_medium.users u ON a.user_id = u.user_id
