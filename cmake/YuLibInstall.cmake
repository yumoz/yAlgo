# 功能：提供SDK库的安装功能
# 参数说明：
#   TARGET_NAME - 要安装的目标名称（必需）
#   INCLUDE_DIR - 头文件目录（可选）
#   INCLUDE_DEST - 头文件安装目的地（可选，默认为include/yUtils/组件名）
#   HEADER_FILES - 要安装的头文件列表（必需）
#
# 用法示例：
#   include(YuLibInstall)
#   set(MY_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/header1.h ${CMAKE_CURRENT_SOURCE_DIR}/header2.h)
#   yutils_install_lib(
#       TARGET_NAME my_target
#       HEADER_FILES ${MY_HEADER_FILES}
#   )

function(yutils_install_lib)
    # 解析参数
    cmake_parse_arguments(INSTALL
        ""  # 选项（无值）
        "TARGET_NAME;INCLUDE_DIR;INCLUDE_DEST"  # 单值参数
        "HEADER_FILES"  # 多值参数
        ${ARGN}
    )
    
    # 检查必需参数
    if(NOT DEFINED INSTALL_TARGET_NAME)
        message(FATAL_ERROR "yutils_install_lib 需要 TARGET_NAME 参数")
    endif()
    
    if(NOT DEFINED INSTALL_HEADER_FILES)
        message(FATAL_ERROR "yutils_install_lib 需要 HEADER_FILES 参数")
    endif()
    
    # 获取目标组件名（用于默认安装路径）
    get_filename_component(COMPONENT_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
    
    # 设置默认值
    if(NOT DEFINED INSTALL_INCLUDE_DEST)
        set(INSTALL_INCLUDE_DEST "include/yAlgo/${COMPONENT_NAME}")
    endif()
    
    # 安装库文件
    install(TARGETS ${INSTALL_TARGET_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
    
    # 安装头文件
    install(FILES ${INSTALL_HEADER_FILES}
        DESTINATION ${INSTALL_INCLUDE_DEST}
    )
    
    message(STATUS "已为 ${INSTALL_TARGET_NAME} 配置安装规则，头文件将安装到 ${INSTALL_INCLUDE_DEST}")
endfunction()

# 功能：提供应用程序的安装功能
# 参数说明：
#   TARGET_NAME - 要安装的目标名称（必需）
#   CONFIG_FILES - 要安装的配置文件列表（可选）
#   CONFIG_DEST - 配置文件安装目的地（可选，默认为etc）
#
# 用法示例：
#   include(YuLibInstall)
#   yutils_install_app(
#       TARGET_NAME my_app
#   )

function(yutils_install_app)
    # 解析参数
    cmake_parse_arguments(INSTALL
        ""  # 选项（无值）
        "TARGET_NAME;CONFIG_DEST"  # 单值参数
        "CONFIG_FILES"  # 多值参数
        ${ARGN}
    )
    
    # 检查必需参数
    if(NOT DEFINED INSTALL_TARGET_NAME)
        message(FATAL_ERROR "yutils_install_app 需要 TARGET_NAME 参数")
    endif()
    
    # 设置默认值
    if(NOT DEFINED INSTALL_CONFIG_DEST)
        set(INSTALL_CONFIG_DEST "etc")
    endif()
    
    # 安装可执行文件
    install(TARGETS ${INSTALL_TARGET_NAME}
        RUNTIME DESTINATION bin
    )
    
    # 安装配置文件（如果提供）
    if(DEFINED INSTALL_CONFIG_FILES)
        install(FILES ${INSTALL_CONFIG_FILES}
            DESTINATION ${INSTALL_CONFIG_DEST}
        )
    endif()
    
    message(STATUS "已为 ${INSTALL_TARGET_NAME} 配置安装规则")
endfunction()
