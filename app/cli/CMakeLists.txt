cmake_minimum_required(VERSION 3.10)

project(yalk VERSION 1.0.0)

set(CLI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rd/cli")

include_directories(
    ${CLI_INCLUDE_DIR}
    ../../sdk/earth
    ../../sdk/math
    ../../sdk/utils
)

set(SOURCE_FILES
    main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    yalgo_earth
    yalgo_math
    yalgo_utils
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if(WIN32)
    configure_file(${CMAKE_SOURCE_DIR}/cmake/templates/yalk_app_version.rc.in
        ${CMAKE_BINARY_DIR}/resources/yalk_version.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_BINARY_DIR}/resources/yalk_version.rc
    )
endif()