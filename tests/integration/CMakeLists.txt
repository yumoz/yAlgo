# 模块间集成测试
add_executable(test_integration test_integration.cpp)
target_link_libraries(test_integration PRIVATE 
    yalgo_log 
    yalgo_utils 
    yalgo_math 
    yalgo_earth
)
add_test(NAME integration_modules COMMAND test_integration)

message(STATUS "  Integration tests configured: modules")
