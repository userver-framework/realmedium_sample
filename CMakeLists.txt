cmake_minimum_required(VERSION 3.12)
project(realmedium_sample CXX)


# Adding userver dependency
find_package(userver COMPONENTS core postgresql chaotic QUIET)
if(NOT userver_FOUND)  # Fallback to subdirectory usage
    # Enable userver libraries that are needed in this project
    set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

    # Compatibility mode: some systems don't support these features
    set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)

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

# Common sources
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]pp)
file(GLOB_RECURSE UNIT_TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*_test.cpp)
list(REMOVE_ITEM SOURCES ${UNIT_TEST_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(${PROJECT_NAME}_objs OBJECT ${SOURCES})
include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver::postgresql)

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
target_link_libraries(${PROJECT_NAME}-chgen userver::postgresql)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC ${PROJECT_NAME}-chgen)

userver_add_sql_library(
    ${PROJECT_NAME}_sql
    NAMESPACE real_medium
    OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}
    SQL_FILES src/db/*.sql
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC ${PROJECT_NAME}_sql)

target_link_libraries(${PROJECT_NAME}_objs PUBLIC cpp-jwt)

find_package(ICU COMPONENTS uc i18n REQUIRED)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC ICU::uc ICU::i18n)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

# Unit Tests
add_executable(${PROJECT_NAME}_unittest ${UNIT_TEST_SOURCES})
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

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
