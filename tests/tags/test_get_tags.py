from http import HTTPStatus

from endpoints import create_article
from endpoints import get_tags
from endpoints import register_user
from models import ArticleList
from models import Profile
from models import User
from utils import get_user_token
from validators import validate_tags


async def test_get_tags(service_client):
    user = User(bio=None, image=None)

    response = await register_user(service_client, user)
    assert response.status == HTTPStatus.OK

    user_token = get_user_token(response)

    articleList = ArticleList(10, Profile(user))
    tags = set()
    for article in articleList.articles:
        response = await create_article(service_client, article, user_token)
        assert response.status == HTTPStatus.OK
        print(article.tagList)
        tags |= set(article.tagList)

    response = await get_tags(service_client)
    assert response.status == HTTPStatus.OK
    assert validate_tags(tags, response)
