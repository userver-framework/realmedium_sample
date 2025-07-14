#pragma once

#include <userver/storages/postgres/io/io_fwd.hpp>
#include <userver/storages/postgres/io/pg_types.hpp>

#include <docs/api/api.hpp>

namespace userver::storages::postgres::io {

template <>
struct CppToUserPg<real_medium::handlers::Profile> {
    static constexpr DBTypeName postgres_name{"real_medium.profile"};
};

}  // namespace userver::storages::postgres::io
