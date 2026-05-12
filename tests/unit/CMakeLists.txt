# 日志模块单元测试
add_executable(test_log test_log.cpp)
target_link_libraries(test_log PRIVATE yalgo_log)
add_test(NAME unit_log COMMAND test_log)

# 工具模块单元测试
add_executable(test_utils test_utils.cpp)
target_link_libraries(test_utils PRIVATE yalgo_utils)
add_test(NAME unit_utils COMMAND test_utils)

# 数学模块单元测试
add_executable(test_math test_math.cpp)
target_link_libraries(test_math PRIVATE yalgo_math)
add_test(NAME unit_math COMMAND test_math)

# 地球坐标模块单元测试
add_executable(test_earth test_earth.cpp)
target_link_libraries(test_earth PRIVATE yalgo_earth)
add_test(NAME unit_earth COMMAND test_earth)

message(STATUS "  Unit tests configured: log, utils, math, earth")
