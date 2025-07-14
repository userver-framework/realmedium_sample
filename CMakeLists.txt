cmake_minimum_required(VERSION 3.12)
project(realmedium_sample CXX)


# Adding userver dependency
find_package(userver COMPONENTS core postgresql chaotic QUIET)
if(NOT userver_FOUND)  # Fallback to subdirectory usage
    # Enable userver libraries that are needed in this project
    set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

    # Compatibility mode: some systems don't support these features
    set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)

    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/userver)
        message(STATUS "Using userver framework from third_party/userver")
        add_subdirectory(third_party/userver)
    else()
        message(FATAL_ERROR "Either install the userver or provide a path to it")
    endif()
endif()

userver_setup_environment()


set(CPP_JWT_BUILD_TESTS OFF)
set(CPP_JWT_BUILD_EXAMPLES OFF)
add_subdirectory(third_party/cpp-jwt)


option(JWT_BUILD_EXAMPLES OFF)

# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
		src/cache/articles_cache.hpp
		src/cache/articles_cache.cpp
        src/cache/comments_cache.cpp
        src/cache/comments_cache.hpp
		src/handlers/common.cpp
		src/handlers/common.hpp
		src/handlers/profiles/profiles.cpp
		src/handlers/profiles/profiles.hpp
		src/handlers/users/users.cpp
		src/handlers/users/users.hpp
		src/handlers/tags/tags.cpp
		src/handlers/tags/tags.hpp
		src/handlers/users/users_login.cpp
		src/handlers/users/users_login.hpp
		src/handlers/users/user_get.cpp
		src/handlers/users/user_get.hpp
		src/handlers/users/user_put.cpp
		src/handlers/users/user_put.hpp
		src/handlers/profiles/profiles_follow.cpp
		src/handlers/profiles/profiles_follow.hpp
		src/handlers/profiles/profiles_follow_delete.cpp
		src/handlers/profiles/profiles_follow_delete.hpp                
		src/handlers/articles/articles_slug_delete.cpp
		src/handlers/articles/articles_slug_delete.hpp
		src/handlers/articles/articles_slug_get.cpp
		src/handlers/articles/articles_slug_get.hpp
		src/handlers/articles/articles_slug_put.cpp
		src/handlers/articles/articles_slug_put.hpp
		src/handlers/articles/articles_get.cpp
		src/handlers/articles/articles_get.hpp
		src/handlers/articles/articles_post.cpp
		src/handlers/articles/articles_post.hpp
		src/handlers/articles/feed_articles.hpp
		src/handlers/articles/feed_articles.cpp
		src/handlers/articles/articles_favorite.cpp
		src/handlers/articles/articles_favorite.hpp
		src/handlers/comments/comment_delete.cpp
		src/handlers/comments/comment_delete.hpp
		src/handlers/comments/comment_post.cpp
		src/handlers/comments/comment_post.hpp
		src/handlers/comments/comments_get.cpp
		src/handlers/comments/comments_get.hpp
		src/handlers/auth/auth_bearer.hpp
		src/handlers/auth/auth_bearer.cpp
		src/dto/article.cpp
		src/dto/article.hpp
		src/dto/comment.hpp
		src/dto/comment.cpp
		src/db/sql.hpp
		src/models/user.hpp
		src/models/user.cpp
		src/models/comment.hpp
		src/models/comment.cpp
		src/models/article.hpp
		src/models/article.cpp
		src/models/profile.hpp
		src/validators/user_validators.hpp
		src/validators/user_validators.cpp
		src/validators/length_validator.hpp
		src/validators/length_validator.cpp
		src/utils/random.hpp
		src/utils/random.cpp
		src/utils/errors.hpp
		src/utils/errors.cpp
		src/utils/make_error.hpp
		src/utils/make_error.cpp
		src/utils/jwt.hpp
		src/utils/jwt.cpp
		src/utils/slugify.hpp
		src/utils/slugify.cpp
		src/handlers/articles/articles_unfavorite.cpp
		src/handlers/articles/articles_unfavorite.hpp
		src/dto/filter.hpp
		src/dto/filter.cpp
		src/validators/validators.cpp
        src/validators/article_validators.cpp
        src/validators/article_validators.hpp

)

include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver::core userver::postgresql)

file(GLOB_RECURSE SCHEMAS ${CMAKE_CURRENT_SOURCE_DIR}/docs/*.yaml)
userver_target_generate_chaotic(${PROJECT_NAME}-chgen
    LAYOUT "/components/schemas/([^/]*)/=real_medium::handlers::{0}"
    GENERATE_SERIALIZERS
    OUTPUT_DIR
        ${CMAKE_CURRENT_BINARY_DIR}/src
    SCHEMAS
        ${SCHEMAS}
    RELATIVE_TO
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC ${PROJECT_NAME}-chgen)

target_include_directories(${PROJECT_NAME}_objs PUBLIC cpp-jwt)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC cpp-jwt)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
find_package(ICU 4.4 COMPONENTS io i18n REQUIRED)
target_include_directories(${PROJECT_NAME}_objs PUBLIC ${ICU_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_objs PUBLIC ${ICU_LIBRARIES})

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

# Unit Tests
add_executable(${PROJECT_NAME}_unittest
	src/utils/slugify_test.cpp
	src/validators/validator_test.cpp
)

target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver::utest)
add_google_tests(${PROJECT_NAME}_unittest)

# Functional Tests
userver_testsuite_add_simple(
  REQUIREMENTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/requirements.txt
)

# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

set(CONFIG_JWT ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/jwt_config.json)

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
