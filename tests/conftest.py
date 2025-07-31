import os
import sys

import pytest

from pytest_userver.plugins import coverage
from testsuite.databases.pgsql import discover

sys.path.append(os.path.join(os.path.dirname(__file__), 'helpers'))


pytest_plugins = ['pytest_userver.plugins.postgresql']


@pytest.fixture(scope='session')
def pgsql_local(service_source_dir, pgsql_local_create):
    """Create schemas databases for tests"""
    databases = discover.find_schemas(
        'pg_service_template',  # service name that goes to the DB connection
        [service_source_dir.joinpath('postgresql/schemas')],
    )
    return pgsql_local_create(list(databases.values()))


@pytest.fixture
def on_uncovered():
    """
    Will be called when the coverage is incomplete.
    """

    def _on_uncovered(uncovered_statements):
        used_in_caches_sqls = {
            'select_full_article_info',
            'select_cached_comments',
        }
        uncovered_statements = set(uncovered_statements) - used_in_caches_sqls
        if uncovered_statements:
            msg = f'Uncovered SQL/YQL statements: {uncovered_statements}'
            raise coverage.UncoveredError(msg)

    return _on_uncovered
